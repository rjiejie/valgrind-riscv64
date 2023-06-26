
/*--------------------------------------------------------------------*/
/*--- begin                                 libvex_guest_riscv64.h ---*/
/*--------------------------------------------------------------------*/

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2020-2023 Petr Pavlu
      petr.pavlu@dagobah.cz

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

#ifndef __LIBVEX_PUB_GUEST_RISCV64_H
#define __LIBVEX_PUB_GUEST_RISCV64_H

#include "libvex_basictypes.h"

/*------------------------------------------------------------*/
/*--- Vex's representation of the riscv64 CPU state.       ---*/
/*------------------------------------------------------------*/

typedef struct {
   /*   0 */ ULong host_EvC_FAILADDR;
   /*   8 */ UInt  host_EvC_COUNTER;
   /*  12 */ UInt  pad0;
   /*  16 */ ULong guest_x0;
   /*  24 */ ULong guest_x1;
   /*  32 */ ULong guest_x2;
   /*  40 */ ULong guest_x3;
   /*  48 */ ULong guest_x4;
   /*  56 */ ULong guest_x5;
   /*  64 */ ULong guest_x6;
   /*  72 */ ULong guest_x7;
   /*  80 */ ULong guest_x8;
   /*  88 */ ULong guest_x9;
   /*  96 */ ULong guest_x10;
   /* 104 */ ULong guest_x11;
   /* 112 */ ULong guest_x12;
   /* 120 */ ULong guest_x13;
   /* 128 */ ULong guest_x14;
   /* 136 */ ULong guest_x15;
   /* 144 */ ULong guest_x16;
   /* 152 */ ULong guest_x17;
   /* 160 */ ULong guest_x18;
   /* 168 */ ULong guest_x19;
   /* 176 */ ULong guest_x20;
   /* 184 */ ULong guest_x21;
   /* 192 */ ULong guest_x22;
   /* 200 */ ULong guest_x23;
   /* 208 */ ULong guest_x24;
   /* 216 */ ULong guest_x25;
   /* 224 */ ULong guest_x26;
   /* 232 */ ULong guest_x27;
   /* 240 */ ULong guest_x28;
   /* 248 */ ULong guest_x29;
   /* 256 */ ULong guest_x30;
   /* 264 */ ULong guest_x31;
   /* 272 */ ULong guest_pc;

   /* Floating-point state. */
   /* 280 */ ULong guest_f0;
   /* 288 */ ULong guest_f1;
   /* 296 */ ULong guest_f2;
   /* 304 */ ULong guest_f3;
   /* 312 */ ULong guest_f4;
   /* 320 */ ULong guest_f5;
   /* 328 */ ULong guest_f6;
   /* 336 */ ULong guest_f7;
   /* 344 */ ULong guest_f8;
   /* 352 */ ULong guest_f9;
   /* 360 */ ULong guest_f10;
   /* 368 */ ULong guest_f11;
   /* 376 */ ULong guest_f12;
   /* 384 */ ULong guest_f13;
   /* 392 */ ULong guest_f14;
   /* 400 */ ULong guest_f15;
   /* 408 */ ULong guest_f16;
   /* 416 */ ULong guest_f17;
   /* 424 */ ULong guest_f18;
   /* 432 */ ULong guest_f19;
   /* 440 */ ULong guest_f20;
   /* 448 */ ULong guest_f21;
   /* 456 */ ULong guest_f22;
   /* 464 */ ULong guest_f23;
   /* 472 */ ULong guest_f24;
   /* 480 */ ULong guest_f25;
   /* 488 */ ULong guest_f26;
   /* 496 */ ULong guest_f27;
   /* 504 */ ULong guest_f28;
   /* 512 */ ULong guest_f29;
   /* 520 */ ULong guest_f30;
   /* 528 */ ULong guest_f31;
   /* 536 */ UInt  guest_fcsr;

   /* Various pseudo-regs mandated by Vex or Valgrind. */
   /* Emulation notes. */
   /* 540 */ UInt guest_EMNOTE;

   /* For clflush/clinval: record start and length of area. */
   /* 544 */ ULong guest_CMSTART;
   /* 552 */ ULong guest_CMLEN;

   /* Used to record the unredirected guest address at the start of a
      translation whose start has been redirected. By reading this
      pseudo-register shortly afterwards, the translation can find out what the
      corresponding no-redirection address was. Note, this is only set for
      wrap-style redirects, not for replace-style ones. */
   /* 560 */ ULong guest_NRADDR;

   /* Fallback LL/SC support. */
   /* 568 */ ULong guest_LLSC_SIZE; /* 0==no transaction, else 4 or 8. */
   /* 576 */ ULong guest_LLSC_ADDR; /* Address of the transaction. */
   /* 584 */ ULong guest_LLSC_DATA; /* Original value at ADDR, sign-extended. */

   /* 592 */ ULong guest_xthead_fxcr;

   /* Vector state. */
   /* 600 */ ULong guest_vstart;
   /* 608 */ ULong guest_vl;
   /* 616 */ ULong guest_vtype;
#define RV_VLEN_MAX 1024
   ULong guest_vreg[32 * RV_VLEN_MAX / 64];

   /* Padding to 16 bytes. */
} VexGuestRISCV64State;

/*------------------------------------------------------------*/
/*--- Utility functions for riscv64 guest stuff.           ---*/
/*------------------------------------------------------------*/

/* ALL THE FOLLOWING ARE VISIBLE TO LIBRARY CLIENT */

/* Initialise all guest riscv64 state. */
void LibVEX_GuestRISCV64_initialise(/*OUT*/ VexGuestRISCV64State* vex_state);


/*------------------------------------------------------------------*/
/*---              BB flag information encoding                  ---*/
/*------------------------------------------------------------------*/

/*
  Some utilities to extract encoded vector CSR info from bb_flag. The bit field
  definitions are described below:

    Bit:    39          24 23           8  7   6  5   3 2    0
    Flag:  |--------------|--------------|---|---|-----|-----|
    Field:       VSTART          VL       VMA VTA  SEW   LMUL

  Use FIELD macro to define a new bit field in bb_flag and use FIELD_EX64 macro
  to extract encoded field to an ULong value.

  VSTART/VL/SEW/LMUL fields are shared across different RVV spec versions, while
  VMA/VTA fields are RVV 1.0 featured vtype CSR fields.
*/

#define MAKE_64BIT_MASK(shift, length) \
    (((~0ULL) >> (64 - (length))) << (shift))

#define FIELD(reg, field, shift, length)                                  \
    enum { R_ ## reg ## _ ## field ## _SHIFT = (shift)};                  \
    enum { R_ ## reg ## _ ## field ## _LENGTH = (length)};                \
    enum { R_ ## reg ## _ ## field ## _MASK =                             \
                                        MAKE_64BIT_MASK(shift, length)};


static inline ULong extract64(ULong value, Int start, Int length)
{
    return (value >> start) & (~0ULL >> (64 - length));
}

static inline ULong deposit64(ULong value, Int start, Int length,
                              ULong fieldval)
{
    ULong mask;
    mask = (~0ULL >> (64 - length)) << start;
    return (value & ~mask) | ((fieldval << start) & mask);
}

#define FIELD_EX64(storage, reg, field)                                   \
    extract64((storage), R_ ## reg ## _ ## field ## _SHIFT,               \
              R_ ## reg ## _ ## field ## _LENGTH)

#define FIELD_DP64(storage, reg, field, val) ({                           \
    struct {                                                              \
        UInt v:R_ ## reg ## _ ## field ## _LENGTH;                        \
    } _v = { .v = val };                                                  \
    ULong _d;                                                             \
    _d = deposit64((storage), R_ ## reg ## _ ## field ## _SHIFT,          \
                  R_ ## reg ## _ ## field ## _LENGTH, _v.v);              \
    _d; })

FIELD(BB_FLAG, LMUL,   0,  3)
FIELD(BB_FLAG, SEW,    3,  3)
FIELD(BB_FLAG, VTA,    6,  1)
FIELD(BB_FLAG, VMA,    7,  1)
FIELD(BB_FLAG, VL,     8,  16)
FIELD(BB_FLAG, VSTART, 24, 16)
/* vtype: cover all [0:7] bits, accommodating RVV 0.7.1 and 1.0 */
FIELD(BB_FLAG, VTYPE,  0,  8)

static inline UShort extract_sew(ULong flag) {
   return (UShort) FIELD_EX64(flag, BB_FLAG, SEW);
}

static inline UShort extract_lmul(ULong flag) {
   return (UShort) FIELD_EX64(flag, BB_FLAG, LMUL);
}

static inline UShort extract_vl(ULong flag) {
   return (UShort) FIELD_EX64(flag, BB_FLAG, VL);
}

static inline UShort extract_vstart(ULong flag) {
   return (UShort) FIELD_EX64(flag, BB_FLAG, VSTART);
}

/* RVV 1.0 featured fields */
static inline UShort extract_vta(ULong flag) {
   return (UShort) FIELD_EX64(flag, BB_FLAG, VTA);
}

static inline UShort extract_vma(ULong flag) {
   return (UShort) FIELD_EX64(flag, BB_FLAG, VMA);
}

static inline ULong get_flag_from_guest_state(volatile VexGuestRISCV64State* vex_state) {
   ULong flag = 0;
   flag = FIELD_DP64(flag, BB_FLAG, VTYPE,  extract64(vex_state->guest_vtype, 0, 8));
   flag = FIELD_DP64(flag, BB_FLAG, VL,     vex_state->guest_vl);
   flag = FIELD_DP64(flag, BB_FLAG, VSTART, vex_state->guest_vstart);
   return flag;
}

#endif /* ndef __LIBVEX_PUB_GUEST_RISCV64_H */

/*--------------------------------------------------------------------*/
/*---                                       libvex_guest_riscv64.h ---*/
/*--------------------------------------------------------------------*/
