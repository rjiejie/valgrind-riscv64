
/*--------------------------------------------------------------------*/
/*--- Asm-only TransTab stuff.             pub_core_transtab_asm.h ---*/
/*--------------------------------------------------------------------*/

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2000-2017 Julian Seward
      jseward@acm.org

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, see <http://www.gnu.org/licenses/>.

   The GNU General Public License is contained in the file COPYING.
*/

#ifndef __PUB_CORE_TRANSTAB_ASM_H
#define __PUB_CORE_TRANSTAB_ASM_H

/* Constants for the fast translation lookup cache.  It is a 4 way associative
   cache, with more-or-less LRU replacement.  It contains 2^VG_TT_FAST_BITS
   sets.

   On all targets, the set number is computed from least significant 2 *
   VG_TT_FAST_BITS of the guest address.  This is a bit unusual in as much as
   it is more normal just to use a VG_TT_FAST_BITS-sized slice of the address
   as the set number.  Using twice as many bits (the two chunks are xor'd)
   spreads entries out (reduces aliasing) and significantly reduces the overall
   miss rate.  The cost is two extra cycles on the fast lookup path, to perform
   an extra shift and an xor.

   For each set there are 4 ways: way0, way1, way2 and way3.  way0 is intended
   to be the MRU and way3 the LRU.  Most lookups hit way0 and involve no
   modification of the line.  A hit at way1 causes way0 and way1 to be swapped.
   A hit at way2 causes way1 and way2 to be swapped; that is, way2 is moved one
   step closer to the front.  But not all the way to the front.  Similarly a
   hit at way3 causes way2 and way3 to be swapped.

   See VG_(lookupInFastCache) for a C implementation of this logic and
   dispatch-*-*.S, label VG_(disp_cp_xindir), for the handcoded assembly
   equivalents for each target.  Note that VG_(lookupInFastCache) is used in C
   land for some administrative lookups but isn't really performance critical.
   The dispatch-*-*.S implementations are used to process all indirect branches
   in the simulator and so *are* performance critical.

   Updates to the cache are rare.  These are performed by setFastCacheEntry.
   New entries are put into way0 and all others are shifted down one slot, so
   that the contents of way3 falls out of the cache.

   On x86/amd64, the cache index is computed as
   (address ^ (address >>u VG_TT_FAST_BITS))[VG_TT_FAST_BITS-1 : 0]'.

   On ppc32/ppc64/mips32/mips64/arm64, the bottom two bits of instruction
   addresses are zero, which means the above function causes only 1/4 of the
   sets to ever be used.  So instead the function is
   (address ^ (address >>u VG_TT_FAST_BITS))[VG_TT_FAST_BITS-1+2 : 0+2]'.

   On arm32, the minimum instruction size is 2, so we discard only the least
   significant bit of the address, hence:
   (address ^ (address >>u VG_TT_FAST_BITS))[VG_TT_FAST_BITS-1+1 : 0+1]'.

   On s390x the rightmost bit of an instruction address is zero, so the arm32
   scheme is used. */
/* TODO Comment on riscv64. */

#define VG_TT_FAST_BITS 13
#define VG_TT_FAST_SETS (1 << VG_TT_FAST_BITS)
#define VG_TT_FAST_MASK ((VG_TT_FAST_SETS) - 1)

// Log2(sizeof(FastCacheSet)).  This is needed in the handwritten assembly.

#if defined(VGA_amd64) || defined(VGA_arm64) \
    || defined(VGA_ppc64be) || defined(VGA_ppc64le) \
    || (defined(VGA_mips64) && defined(VGABI_64)) \
    || defined(VGA_s390x) || defined(VGA_riscv64)

/* On 64-bit host, FastCacheSet is composed of 4 ways. Each way has a
   8-byte guest addr, 8-byte bb flag, and 8-byte host addr. The total size
   is (8 + 8 + 8) * 4 = 96. As the total size cannot be reprensented by
   a simple shift operation and using multiplication may causes some 
   performance degradation, we align its address by 16-byte. Additionally,
   the sizeof(Addr) cannot be trivially known at compiliation time, we use
   the maximum alignment in 64-bit and 32-bit platforms. 
*/
# define VG_FAST_CACHE_SET_BITS 7
  // These FCS_{g,h}{0,1,2,3} are the values of
  // offsetof(FastCacheSet,{guest,host}{0,1,2,3}).
# define FCS_g0 0
# define FCS_f0 8
# define FCS_h0 16
# define FCS_g1 24
# define FCS_f1 32
# define FCS_h1 40
# define FCS_g2 48
# define FCS_f2 56
# define FCS_h2 64
# define FCS_g3 72
# define FCS_f3 80
# define FCS_h3 88

#elif defined(VGA_x86) || defined(VGA_arm) || defined(VGA_ppc32) \
      || defined(VGA_mips32) || defined(VGP_nanomips_linux) \
      || (defined(VGA_mips64) && defined(VGABI_N32))
/* On 32-bit host, FastCacheSet is composed of 4 ways. Each way has a
   4-byte guest addr, 8-byte bb flag, and 4-byte host addr. The total size
   is (4 + 8 + 4) * 4 = 64, which can be represented by (1 << 6) * 1. But
   currently FastCacheSet is 16-byte aligned, the VG_FAST_CACHE_SET_BITS is
   kept consistent with 64-bit platform.
*/
# define VG_FAST_CACHE_SET_BITS 7
# define FCS_g0 0
# define FCS_f0 4
# define FCS_h0 12
# define FCS_g1 16
# define FCS_f1 20
# define FCS_h1 28
# define FCS_g2 32
# define FCS_f2 36
# define FCS_h2 44
# define FCS_g3 48
# define FCS_f2 52
# define FCS_h3 60

#else
# error "VG_FAST_CACHE_SET_BITS not known"
#endif

#endif   // __PUB_CORE_TRANSTAB_ASM_H

/*--------------------------------------------------------------------*/
/*--- end                                                          ---*/
/*--------------------------------------------------------------------*/
