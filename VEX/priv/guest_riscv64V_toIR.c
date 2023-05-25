
/*--------------------------------------------------------------------*/
/*--- begin                                guest_riscv64V_toIR.c ---*/
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

#define GET_VMASK() INSN(25, 25)
#define isVOpVV(type) (type == RV64_SOPC_OPIVV || type == RV64_SOPC_OPMVV || type == RV64_SOPC_OPFVV)
#define isVOpVXorVF(type) (type == RV64_SOPC_OPIVX || type == RV64_SOPC_OPMVX || type == RV64_SOPC_OPFVF)
#define isVOpVI(type) (type == RV64_SOPC_OPIVI)

/*------------------------------------------------------------*/
/*--- Globals                                              ---*/
/*------------------------------------------------------------*/

static ULong host_VLENB;

/*------------------------------------------------------------*/
/*--- Offsets of various parts of the riscv64 guest state  ---*/
/*------------------------------------------------------------*/

#define OFFB_V0  (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 0)
#define OFFB_V1  (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 1)
#define OFFB_V2  (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 2)
#define OFFB_V3  (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 3)
#define OFFB_V4  (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 4)
#define OFFB_V5  (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 5)
#define OFFB_V6  (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 6)
#define OFFB_V7  (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 7)
#define OFFB_V8  (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 8)
#define OFFB_V9  (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 9)
#define OFFB_V10 (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 10)
#define OFFB_V11 (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 11)
#define OFFB_V12 (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 12)
#define OFFB_V13 (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 13)
#define OFFB_V14 (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 14)
#define OFFB_V15 (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 15)
#define OFFB_V16 (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 16)
#define OFFB_V17 (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 17)
#define OFFB_V18 (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 18)
#define OFFB_V19 (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 19)
#define OFFB_V20 (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 20)
#define OFFB_V21 (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 21)
#define OFFB_V22 (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 22)
#define OFFB_V23 (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 23)
#define OFFB_V24 (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 24)
#define OFFB_V25 (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 25)
#define OFFB_V26 (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 26)
#define OFFB_V27 (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 27)
#define OFFB_V28 (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 28)
#define OFFB_V29 (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 29)
#define OFFB_V30 (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 30)
#define OFFB_V31 (offsetof(VexGuestRISCV64State, guest_vreg) + host_VLENB * 31)

static Int offsetVReg(UInt regNo)
{
   switch (regNo) {
      case 0:  return OFFB_V0;
      case 1:  return OFFB_V1;
      case 2:  return OFFB_V2;
      case 3:  return OFFB_V3;
      case 4:  return OFFB_V4;
      case 5:  return OFFB_V5;
      case 6:  return OFFB_V6;
      case 7:  return OFFB_V7;
      case 8:  return OFFB_V8;
      case 9:  return OFFB_V9;
      case 10: return OFFB_V10;
      case 11: return OFFB_V11;
      case 12: return OFFB_V12;
      case 13: return OFFB_V13;
      case 14: return OFFB_V14;
      case 15: return OFFB_V15;
      case 16: return OFFB_V16;
      case 17: return OFFB_V17;
      case 18: return OFFB_V18;
      case 19: return OFFB_V19;
      case 20: return OFFB_V20;
      case 21: return OFFB_V21;
      case 22: return OFFB_V22;
      case 23: return OFFB_V23;
      case 24: return OFFB_V24;
      case 25: return OFFB_V25;
      case 26: return OFFB_V26;
      case 27: return OFFB_V27;
      case 28: return OFFB_V28;
      case 29: return OFFB_V29;
      case 30: return OFFB_V30;
      case 31: return OFFB_V31;
      default: vassert(0);
   }
}

/* Obtain ABI name of a register. */
static const HChar* nameVReg(UInt regNo)
{
   vassert(regNo < 32);
   static const HChar* names[32] = {
      "v0",  "v1",  "v2",  "v3",  "v4",  "v5",  "v6",  "v7",
      "v8",  "v9",  "v10", "v11", "v12", "v13", "v14", "v15",
      "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
      "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31",
   };
   return names[regNo];
}

#include "guest_riscv64V0p7_toIR.c"

static Bool dis_RV64V(/*MB_OUT*/ DisResult* dres,
                      /*OUT*/ IRSB*         irsb,
                      UInt                  insn)
{
   if (dis_RV64V0p7(dres, irsb, insn))
      return True;
   return False;
}

/*--------------------------------------------------------------------*/
/*--- end                                  guest_riscv64V_toIR.c ---*/
/*--------------------------------------------------------------------*/
