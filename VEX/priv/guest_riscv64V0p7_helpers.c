
/*--------------------------------------------------------------------*/
/*--- begin                             guest_riscv64V0p7_helpers.c --*/
/*--------------------------------------------------------------------*/

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2020-2022 T-HEAD of Alibaba
      rjiejie@linux.alibaba.com

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

#include "libvex_guest_riscv64.h"
#include "guest_riscv64V_defs.h"

#undef  GETN_VOp
#undef  GETA_VOp
#define GETN_VOp(insn, type, op, mask) "RVV0p7_"#type"_"#insn#op#mask
#define GETA_VOp(insn, type, op, mask) RVV0p7_##type##_##insn##op##mask

#include "guest_riscv64V0p7_helpers.def"

/*---------------------------------------------------------------*/
/*--- OPI special function definitions                        ---*/
/*---------------------------------------------------------------*/

UInt GETA_VUnopV(vid)(VexGuestRISCV64State *st,
                      ULong vd, ULong vs2, ULong vs1, ULong mask) {
   RVV_Config();
   UInt ret = 0;
   vd += (ULong)st;
   __asm__ __volatile__(
      "vle.v\tv8,(%0)\n\t"
      "vid.v\tv8\n\t"
      "csrw\tvstart,%1\n\t"
      "vse.v\tv8,(%0)\n\t"
      :
      : "r"(vd), "r"(vstart)
      : "memory");
   return ret;
}
UInt GETA_VUnopV_M(vid)(VexGuestRISCV64State *st,
                        ULong vd, ULong vs2, ULong vs1, ULong mask) {
   RVV_Config();
   UInt ret = 0;
   vd += (ULong)st;
   mask += (ULong)st;
   RVV_Mask()
   __asm__ __volatile__(
      "vle.v\tv8,(%0)\n\t"
      "vid.v\tv8,v0.t\n\t"
      "csrw\tvstart,%1\n\t"
      "vse.v\tv8,(%0)\n\t"
      :
      : "r"(vd), "r"(vstart)
      : "memory");
   return ret;
}

/*---------------------------------------------------------------*/
/*--- OPF special function definitions                        ---*/
/*---------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
/*--- end                               guest_riscv64V0p7_helpers.c --*/
/*--------------------------------------------------------------------*/
