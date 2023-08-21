
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

#include "guest_riscv64V_defs.h"

#define GET_VMASK() INSN(25, 25)
#define GET_MOP()   INSN(28, 26)
#define GET_NF()    INSN(31, 29)

/*------------------------------------------------------------*/
/*--- Globals                                              ---*/
/*------------------------------------------------------------*/

static ULong host_VLENB;
static ULong guest_VFLAG;

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

/* Vector CSRs offsets */
#define OFFB_VTYPE  offsetof(VexGuestRISCV64State, guest_vtype)
#define OFFB_VL     offsetof(VexGuestRISCV64State, guest_vl)
#define OFFB_VSTART offsetof(VexGuestRISCV64State, guest_vstart)
#define OFFB_VCSR   offsetof(VexGuestRISCV64State, guest_vcsr)

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

/* Vector CSRs get/put interfaces */
/* VType CSR read, return type is I64 */
static IRExpr* getVType(void) {
   return IRExpr_Get(OFFB_VTYPE, Ity_I64);
}

/* VType CSR write, the written value should be I64 */
static void putVType(IRSB* irsb, IRExpr* e) {
   IRType ty = typeOfIRExpr(irsb->tyenv, e);
   vassert(ty == Ity_I64);
   stmt(irsb, IRStmt_Put(OFFB_VTYPE, e));
}

/* VL CSR read, return type is I64 */
static IRExpr* getVL(void) {
   return IRExpr_Get(OFFB_VL, Ity_I64);
}

/* VL CSR write, the written value should be I64 */
static void putVL(IRSB* irsb, IRExpr* e) {
   IRType ty = typeOfIRExpr(irsb->tyenv, e);
   vassert(ty == Ity_I64);
   stmt(irsb, IRStmt_Put(OFFB_VL, e));
}

/* VStart CSR read, return type is I64 */
static IRExpr* getVStart(void) {
   return IRExpr_Get(OFFB_VSTART, Ity_I64);
}

/* VStart CSR write, the written value should be I64 */
static void putVStart(IRSB* irsb, IRExpr* e) {
   IRType ty = typeOfIRExpr(irsb->tyenv, e);
   vassert(ty == Ity_I64);
   stmt(irsb, IRStmt_Put(OFFB_VSTART, e));
}

/* VCSR CSR read, return type is I64 */
static IRExpr* getVCSR(void) {
   return IRExpr_Get(OFFB_VCSR, Ity_I64);
}

/* VCSR CSR write, the written value should be I64 */
static void putVCSR(IRSB* irsb, IRExpr* e) {
   IRType ty = typeOfIRExpr(irsb->tyenv, e);
   vassert(ty == Ity_I64);
   stmt(irsb, IRStmt_Put(OFFB_VCSR, e));
}

/* Read a value from VXSAT CSR */
static IRExpr* getVxsat(void) {
   IRExpr* vcsr = getVCSR();
   return binop(Iop_And32, unop(Iop_64to32, vcsr), mkU32(1));
}

/* Write a value to VXSAT CSR */
static void putVxsat(IRSB* irsb, IRExpr* e) {
   IRType ty = typeOfIRExpr(irsb->tyenv, e);
   vassert(ty == Ity_I32);
   IRExpr* vcsr = getVCSR();
   IRExpr* new_vcsr =
      binop(Iop_Or64, binop(Iop_And64, vcsr, mkU64(0xFFFFFFFFFFFFFFFE)),
            unop(Iop_32Uto64, e));
   putVCSR(irsb, new_vcsr);
}

/* Find the offset of the requested data type and vector register lane
   number. It is borrowed from ARM64 offsetQRegLane except that we
   do not support 128-sized type currently. Besides, RVV allows elements
   to cross the boundary of a single register to form a register group.
   It is useless to check maxOff < register length. */
static Int offsetVRegLane(UInt vregNo, IRType laneTy, UInt laneNo) {
   Int base = offsetVReg(vregNo);
   UInt laneSzB = 0;
   switch (laneTy) {
      case Ity_I8:                laneSzB = 1; break;
      case Ity_I16: case Ity_F16: laneSzB = 2; break;
      case Ity_I32: case Ity_F32: laneSzB = 4; break;
      case Ity_I64: case Ity_F64: laneSzB = 8; break;
      default: break;
   }
   /* assure that we fall into a reasonable type */
   vassert(laneSzB > 0);
   UInt minOff = laneNo * laneSzB;
   UInt maxOff = minOff + laneSzB - 1;
   /* maximal data offset up to host_VLENB */
   vassert(maxOff < (host_VLENB << 3));
   return base + minOff;
}

/* Write a value into vector register, the value type is determined by e */
static void putVRegLane(IRSB *irsb, UInt vregNo, UInt laneNo, IRExpr* e) {
   vassert(vregNo >= 0 && vregNo < 32);
   IRType ty  = typeOfIRExpr(irsb->tyenv, e);
   stmt(irsb, IRStmt_Put(offsetVRegLane(vregNo, ty, laneNo), e));
}

/* Read a value from vector register, the value type is determined by ty */
static IRExpr* getVRegLane(UInt vregNo, UInt laneNo, IRType ty) {
   vassert(vregNo >= 0 && vregNo < 32);
   return IRExpr_Get(offsetVRegLane(vregNo, ty, laneNo), ty);
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

static const HChar* nameVCSR(UInt csr) {
   switch (csr) {
      case 0x008:
         return "vstart";
      case 0x009:
         return "vxsat";
      case 0x00A:
         return "vxrm";
      case 0x00F:
         return "vcsr";
      case 0xC20:
         return "vl";
      case 0xC21:
         return "vtype";
      case 0xC22:
         return "vlenb";
      default:
         vpanic("nameVCSR(riscv64)");
   }
}

/*---------------------------------------------------------------*/
/*--- Get dirty info of helper functions                      ---*/
/*---------------------------------------------------------------*/

/* Binop */
static IRDirty*
GETD_VBinop(IRDirty* d, UInt vd, UInt vs2, UInt vs1, Bool mask, UInt sopc, UInt vtype, UInt lmul)
{
   d->nFxState = isVOpVV(sopc) ? 3 : 2;
   vex_bzero(&d->fxState, sizeof(d->fxState));

   UInt lmuls[3] = {vtype & GETV_VopWidenD ? lmul * 2
                    : vtype & GETV_VopM1D  ? 1
                                           : lmul,
                    vtype & GETV_VopWidenS2 ? lmul * 2
                    : vtype & GETV_VopM1S   ? 1
                                            : lmul,
                    vtype & GETV_VopM1S ? 1 : lmul};
   UInt regNos[3] = {vd, vs2, vs1};
   for (int i = 0; i < d->nFxState; i++) {
      d->fxState[i].fx =
         i == 0 ? (vtype & GETV_VopAccD ? Ifx_Modify : Ifx_Write) : Ifx_Read;
      d->fxState[i].offset = offsetVReg(regNos[i]);
      d->fxState[i].size   = host_VLENB;
      d->fxState[i].nRepeats  = lmuls[i] - 1;
      d->fxState[i].repeatLen = lmuls[i] - 1 == 0 ? 0 : host_VLENB;
   }

   if (isVOpVXorVF(sopc)) {
      d->fxState[d->nFxState].fx     = Ifx_Read;
      d->fxState[d->nFxState].offset = sopc == RV64_SOPC_OPFVF ? offsetFReg(vs1) : offsetIReg64(vs1);
      d->fxState[d->nFxState].size   = 8;
      d->nFxState += 1;
   }

   if (!mask || (vtype & GETV_VopMask)) {
      d->fxState[d->nFxState].fx     = Ifx_Read;
      d->fxState[d->nFxState].offset = offsetVReg(0);
      d->fxState[d->nFxState].size   = host_VLENB;
      d->nFxState += 1;
   }
   return d;
}

/* Unop */
static IRDirty*
GETD_VUnop(IRDirty* d, UInt vd, UInt src, Bool mask, UInt sopc, UInt vtype, UInt lmul)
{
   d->nFxState = isVOpVV(sopc) ? 2 : 1;
   vex_bzero(&d->fxState, sizeof(d->fxState));

   UInt lmuls[2] = {vtype & GETV_VopWidenD ? lmul * 2
                    : vtype & GETV_VopM1D  ? 1
                                           : lmul,
                    vtype & GETV_VopWidenS2 ? lmul * 2
                    : vtype & GETV_VopM1S   ? 1
                                            : lmul};
   UInt regNos[2] = {vd, src};
   for (int i = 0; i < d->nFxState; i++) {
      d->fxState[i].fx     = i == 0 ? Ifx_Write : Ifx_Read;
      d->fxState[i].offset = offsetVReg(regNos[i]);
      d->fxState[i].size   = host_VLENB;
      d->fxState[i].nRepeats  = lmuls[i] - 1;
      d->fxState[i].repeatLen = lmuls[i] - 1 == 0 ? 0 : host_VLENB;
   }

   if (isVOpVXorVF(sopc)) {
      d->fxState[d->nFxState].fx     = Ifx_Read;
      d->fxState[d->nFxState].offset = sopc == RV64_SOPC_OPFVF ? offsetFReg(src) : offsetIReg64(src);
      d->fxState[d->nFxState].size   = 8;
      d->nFxState += 1;
   }

   if (!mask || (vtype & GETV_VopMask)) {
      d->fxState[d->nFxState].fx     = Ifx_Read;
      d->fxState[d->nFxState].offset = offsetVReg(0);
      d->fxState[d->nFxState].size   = host_VLENB;
      d->nFxState += 1;
   }
   return d;
}

#include "guest_riscv64V_helpers.c"

static Bool dis_RV64V_csr(/*MB_OUT*/ DisResult* dres,
                          /*OUT*/ IRSB* irsb,
                          UInt insn)
{
   return False;
}

static Bool dis_RV64V_cfg(/*MB_OUT*/ DisResult* dres,
                          /*OUT*/ IRSB*         irsb,
                          UInt                  insn)
{
   return False;
}

static Bool dis_RV64V_ldst(/*MB_OUT*/ DisResult* dres,
                           /*OUT*/ IRSB*         irsb,
                           UInt                  insn)
{
   return False;
}

static Bool dis_RV64V_arith_OPI(/*MB_OUT*/ DisResult* dres,
                                /*OUT*/ IRSB*         irsb,
                                UInt                  insn)
{
   IRDirty *d  = NULL;
   void *fAddr = NULL;
   const HChar *fName = NULL;
   IRExpr **args = NULL;
   ULong temp = 0;
   UInt lmul   = extract_lmul(guest_VFLAG);
   IRTemp ret = newTemp(irsb, Ity_I32);
   IRTemp xrm = newTemp(irsb, Ity_I32);

   UInt rd   = GET_RD();
   UInt rs1  = GET_RS1();
   UInt rs2  = GET_RS2();
   Bool mask = GET_VMASK();

   switch (GET_FUNCT6()) {
      /*
       * Vector Single-Width Integer Add and Subtract
       */
      case 0b000000:
         GETC_VBinopOPI(vadd, V, X, I);
         return True;
      case 0b000010:
         GETC_VBinopOPI(vsub, V, X, NIL);
         return True;
      case 0b000011:
         GETC_VBinopOPI(vrsub, NIL, X, I);
         return True;
      /*
       * Vector Integer Add-with-Carry / Subtract-with-Borrow Instructions
       */
      case 0b010000:
         GETC_VBinopOPI_VAR(vadc, V, X, I, GETV_VopMask);
         return True;
      case 0b010001:
         GETC_VBinopOPI_VAR(vmadc, V, X, I, GETV_VopMask);
         return True;
      case 0b010010:
         GETC_VBinopOPI_VAR(vsbc, V, X, NIL, GETV_VopMask);
         return True;
      case 0b010011:
         GETC_VBinopOPI_VAR(vmsbc, V, X, NIL, GETV_VopMask);
         return True;
      /*
       * Vector Bitwise Logical Instructions
       */
      case 0b001001:
         GETC_VBinopOPI(vand, V, X, I);
         return True;
      case 0b001010:
         GETC_VBinopOPI(vor, V, X, I);
         return True;
      case 0b001011:
         GETC_VBinopOPI(vxor, V, X, I);
         return True;
      /*
       * Vector Single-Width Bit Shift Instructions
       */
      case 0b100101:
         GETC_VBinopOPI(vsll, V, X, I);
         return True;
      case 0b101000:
         GETC_VBinopOPI(vsrl, V, X, I);
         return True;
      case 0b101001:
         GETC_VBinopOPI(vsra, V, X, I);
         return True;
      /*
       * Vector Narrowing Integer Right Shift Instructions
       */
      case 0b101100:
         GETC_VBinopOPI_VAR(vnsrlw, V, X, I, GETV_VopWidenS2);
         return True;
      case 0b101101:
         GETC_VBinopOPI_VAR(vnsraw, V, X, I, GETV_VopWidenS2);
         return True;
      /*
       * Vector Integer Comparison Instructions
       */
      case 0b011000:
         GETC_VBinopOPI_VAR(vmseq, V, X, I, GETV_VopM1D);
         return True;
      case 0b011001:
         GETC_VBinopOPI_VAR(vmsne, V, X, I, GETV_VopM1D);
         return True;
      case 0b011010:
         GETC_VBinopOPI_VAR(vmsltu, V, X, NIL, GETV_VopM1D);
         return True;
      case 0b011011:
         GETC_VBinopOPI_VAR(vmslt, V, X, NIL, GETV_VopM1D);
         return True;
      case 0b011100:
         GETC_VBinopOPI_VAR(vmsleu, V, X, I, GETV_VopM1D);
         return True;
      case 0b011101:
         GETC_VBinopOPI_VAR(vmsle, V, X, I, GETV_VopM1D);
         return True;
      case 0b011110:
         GETC_VBinopOPI_VAR(vmsgtu, NIL, X, I, GETV_VopM1D);
         return True;
      case 0b011111:
         GETC_VBinopOPI_VAR(vmsgt, NIL, X, I, GETV_VopM1D);
         return True;
      /*
       * Vector Integer Min/Max Instructions
       */
      case 0b000100:
         GETC_VBinopOPI(vminu, V, X, NIL);
         return True;
      case 0b000101:
         GETC_VBinopOPI(vmin, V, X, NIL);
         return True;
      case 0b000110:
         GETC_VBinopOPI(vmaxu, V, X, NIL);
         return True;
      case 0b000111:
         GETC_VBinopOPI(vmax, V, X, NIL);
         return True;
      /*
       * Vector Integer Merge and Move Instructions
       */
      case 0b010111:
         if (!mask)
            GETC_VBinopOPI(vmerge, V, X, I);
         else {
            rs2 = rs1;
            GETC_VUnopOPI(vmv, V, X, I);
         }
         return True;
      /*
       * Vector Single-Width Saturating Add and Subtract
       */
      case 0b100000:
         GETC_VBinopSAT(vsaddu, V, X, I);
         return True;
      case 0b100001:
         GETC_VBinopSAT(vsadd, V, X, I);
         return True;
      case 0b100010:
         GETC_VBinopSAT(vssubu, V, X, NIL);
         return True;
      case 0b100011:
         GETC_VBinopSAT(vssub, V, X, NIL);
         return True;
      /*
       * Vector Single-Width Fractional Multiply with Rounding and Saturation
       */
      case 0b100111:
         GETC_VBinopSAT(vsmul, V, X, NIL);
         return True;
      /*
       * Vector Single-Width Scaling Shift Instructions
       */
      case 0b101010:
         GETC_VBinopSAT(vssrl, V, X, I);
         return True;
      case 0b101011:
         GETC_VBinopSAT(vssra, V, X, I);
         return True;
      /*
       * Vector Narrowing Fixed-Point Clip Instructions
       */
      case 0b101110:
         GETC_VBinopSAT_VAR(vnclipu, V, X, I, GETV_VopWidenS2);
         return True;
      case 0b101111:
         GETC_VBinopSAT_VAR(vnclip, V, X, I, GETV_VopWidenS2);
         return True;
      /*
       * Vector Slideup Instructions
       */
      case 0b001110:
         GETC_VBinopOPI(vslideup, NIL, X, I);
         return True;
      /*
       * Vector Slidedown Instructions
       */
      case 0b001111:
         GETC_VBinopOPI(vslidedown, NIL, X, I);
         return True;
      /*
       * Vector Register Gather Instruction
       */
      case 0b001100:
         GETC_VBinopOPI(vrgather, V, X, I);
         return True;
      /*
       * Vector Widening Integer Reduction Instructions
       */
      case 0b110000:
         GETC_VBinopOPI_VAR(vwredsumu, V, NIL, NIL, GETV_VopM1D);
         return True;
      case 0b110001:
         GETC_VBinopOPI_VAR(vwredsum, V, NIL, NIL, GETV_VopM1D);
         return True;
      default:
         break;
   }

   return False;
}

static Bool dis_RV64V_arith_OPM(/*MB_OUT*/ DisResult* dres,
                                /*OUT*/ IRSB*         irsb,
                                UInt                  insn)
{
   IRDirty *d  = NULL;
   void *fAddr = NULL;
   const HChar *fName = NULL;
   IRExpr **args = NULL;
   UInt temp = 0;
   UInt lmul   = extract_lmul(guest_VFLAG);
   IRTemp ret = newTemp(irsb, Ity_I64);
   IRTemp xrm = newTemp(irsb, Ity_I32);

   UInt rd   = GET_RD();
   UInt rs1  = GET_RS1();
   UInt rs2  = GET_RS2();
   Bool mask = GET_VMASK();

   switch (GET_FUNCT6()) {
      /*
       * Vector Widening Integer Add/Subtract
       */
      case 0b110000:
         GETC_VBinopOPI_VAR(vwaddu, V, X, NIL, GETV_VopWidenD);
         return True;
      case 0b110010:
         GETC_VBinopOPI_VAR(vwsubu, V, X, NIL, GETV_VopWidenD);
         return True;
      case 0b110001:
         GETC_VBinopOPI_VAR(vwadd, V, X, NIL, GETV_VopWidenD);
         return True;
      case 0b110011:
         GETC_VBinopOPI_VAR(vwsub, V, X, NIL, GETV_VopWidenD);
         return True;
      case 0b110100:
         GETC_VBinopOPI_VAR(vwadduw, V, X, NIL, GETV_VopWidenD | GETV_VopWidenS2);
         return True;
      case 0b110110:
         GETC_VBinopOPI_VAR(vwsubuw, V, X, NIL, GETV_VopWidenD | GETV_VopWidenS2);
         return True;
      case 0b110101:
         GETC_VBinopOPI_VAR(vwaddw, V, X, NIL, GETV_VopWidenD | GETV_VopWidenS2);
         return True;
      case 0b110111:
         GETC_VBinopOPI_VAR(vwsubw, V, X, NIL, GETV_VopWidenD | GETV_VopWidenS2);
         return True;
      /*
       * Vector Single-Width Integer Multiply Instructions
       */
      case 0b100101:
         GETC_VBinopOPI(vmul, V, X, NIL);
         return True;
      case 0b100111:
         GETC_VBinopOPI(vmulh, V, X, NIL);
         return True;
      case 0b100100:
         GETC_VBinopOPI(vmulhu, V, X, NIL);
         return True;
      case 0b100110:
         GETC_VBinopOPI(vmulhsu, V, X, NIL);
         return True;
      /*
       * Vector Integer Divide Instructions
       */
      case 0b100000:
         GETC_VBinopOPI(vdivu, V, X, NIL);
         return True;
      case 0b100001:
         GETC_VBinopOPI(vdiv, V, X, NIL);
         return True;
      case 0b100010:
         GETC_VBinopOPI(vremu, V, X, NIL);
         return True;
      case 0b100011:
         GETC_VBinopOPI(vrem, V, X, NIL);
         return True;
      /*
       * Vector Widening Integer Multiply Instructions
       */
      case 0b111011:
         GETC_VBinopOPI_VAR(vwmul, V, X, NIL, GETV_VopWidenD);
         return True;
      case 0b111000:
         GETC_VBinopOPI_VAR(vwmulu, V, X, NIL, GETV_VopWidenD);
         return True;
      case 0b111010:
         GETC_VBinopOPI_VAR(vwmulsu, V, X, NIL, GETV_VopWidenD);
         return True;
      /*
       * Vector Single-Width Integer Multiply-Add Instructions
       */
      case 0b101101:
         GETC_VBinopOPI_VAR(vmacc, V, X, NIL, GETV_VopAccD);
         return True;
      case 0b101111:
         GETC_VBinopOPI_VAR(vnmsac, V, X, NIL, GETV_VopAccD);
         return True;
      case 0b101001:
         GETC_VBinopOPI_VAR(vmadd, V, X, NIL, GETV_VopAccD);
         return True;
      case 0b101011:
         GETC_VBinopOPI_VAR(vnmsub, V, X, NIL, GETV_VopAccD);
         return True;
      /*
       * Vector Widening Integer Multiply-Add Instructions
       */
      case 0b111100:
         GETC_VBinopOPI_VAR(vwmaccu, V, X, NIL, GETV_VopAccD | GETV_VopWidenD);
         return True;
      case 0b111101:
         GETC_VBinopOPI_VAR(vwmacc, V, X, NIL, GETV_VopAccD | GETV_VopWidenD);
         return True;
      case 0b111111:
         GETC_VBinopOPI_VAR(vwmaccsu, V, X, NIL, GETV_VopAccD | GETV_VopWidenD);
         return True;
      case 0b111110:
         GETC_VBinopOPI_VAR(vwmaccus, NIL, X, NIL, GETV_VopAccD | GETV_VopWidenD);
         return True;
      /*
       * Vector Slide1up Instructions
       */
      case 0b001110:
         GETC_VBinopOPI(vslide1up, NIL, X, NIL);
         return True;
      /*
       * Vector Slide1down Instructions
       */
      case 0b001111:
         GETC_VBinopOPI(vslide1down, NIL, X, NIL);
         return True;
      /*
       * Vector Compress Instruction
       */
      case 0b010111:
         GETC_VBinopOPI(vcompress, V, NIL, NIL);
         return True;
      /*
       * Vector Single-Width Integer Reduction Instructions
       */
      case 0b000000:
         GETC_VBinopOPI_VAR(vredsum, V, NIL, NIL, GETV_VopM1D);
         return True;
      case 0b000110:
         GETC_VBinopOPI_VAR(vredmaxu, V, NIL, NIL, GETV_VopM1D);
         return True;
      case 0b000111:
         GETC_VBinopOPI_VAR(vredmax, V, NIL, NIL, GETV_VopM1D);
         return True;
      case 0b000100:
         GETC_VBinopOPI_VAR(vredminu, V, NIL, NIL, GETV_VopM1D);
         return True;
      case 0b000101:
         GETC_VBinopOPI_VAR(vredmin, V, NIL, NIL, GETV_VopM1D);
         return True;
      case 0b000001:
         GETC_VBinopOPI_VAR(vredand, V, NIL, NIL, GETV_VopM1D);
         return True;
      case 0b000010:
         GETC_VBinopOPI_VAR(vredor, V, NIL, NIL, GETV_VopM1D);
         return True;
      case 0b000011:
         GETC_VBinopOPI_VAR(vredxor, V, NIL, NIL, GETV_VopM1D);
         return True;
      /*
       * Vector Mask-Register Logical Instructions
       */
      case 0b011001:
         GETC_VBinopOPI_VAR(vmand, V, NIL, NIL, GETV_VopM1D | GETV_VopM1S);
         return True;
      case 0b011101:
         GETC_VBinopOPI_VAR(vmnand, V, NIL, NIL, GETV_VopM1D | GETV_VopM1S);
         return True;
      case 0b011000:
         GETC_VBinopOPI_VAR(vmandnot, V, NIL, NIL, GETV_VopM1D | GETV_VopM1S);
         return True;
      case 0b011011:
         GETC_VBinopOPI_VAR(vmxor, V, NIL, NIL, GETV_VopM1D | GETV_VopM1S);
         return True;
      case 0b011010:
         GETC_VBinopOPI_VAR(vmor, V, NIL, NIL, GETV_VopM1D | GETV_VopM1S);
         return True;
      case 0b011110:
         GETC_VBinopOPI_VAR(vmnor, V, NIL, NIL, GETV_VopM1D | GETV_VopM1S);
         return True;
      case 0b011100:
         GETC_VBinopOPI_VAR(vmornot, V, NIL, NIL, GETV_VopM1D | GETV_VopM1S);
         return True;
      case 0b011111:
         GETC_VBinopOPI_VAR(vmxnor, V, NIL, NIL, GETV_VopM1D | GETV_VopM1S);
         return True;
      /*
       * VMUNARY0
       */
      case 0b010100:
         switch (rs1) {
            /*
             * vmsbf.m set-before-first mask bit
             */
            case 0b00001:
               GETC_VUnopOPI_VAR(vmsbf, V, NIL, NIL, GETV_VopM1D | GETV_VopM1S);
               return True;
            /*
             * vmsif.m set-including-first mask bit
             */
            case 0b00011:
               GETC_VUnopOPI_VAR(vmsif, V, NIL, NIL, GETV_VopM1D | GETV_VopM1S);
               return True;
            /*
             * vmsof.m set-only-first mask bit
             */
            case 0b00010:
               GETC_VUnopOPI_VAR(vmsof, V, NIL, NIL, GETV_VopM1D | GETV_VopM1S);
               return True;
            /*
             * Vector Iota Instruction
             */
            case 0b10000:
               GETC_VUnopOPI_VAR(viota, V, NIL, NIL, GETV_VopM1S);
               return True;
            /*
             * Vector Element Index Instruction
             */
            case 0b10001:
               GETC_VUnopOPI(vid, V, NIL, NIL);
               return True;
            default:
               break;
         }
         break;
      /*
       * Vector Single-Width Averaging Add and Subtract
       */
      case 0b001001:
         GETC_VBinopSAT(vaadd, V, X, NIL);
         return True;
      case 0b001011:
         GETC_VBinopSAT(vasub, V, X, NIL);
         return True;
      case 0b001000:
         GETC_VBinopSAT(vaaddu, V, X, NIL);
         return True;
      case 0b001010:
         GETC_VBinopSAT(vasubu, V, X, NIL);
         return True;
      /*
       * VWXUNARY0/VRXUNARY0
       */
      case 0b010000:
         if (isVOpVV(GET_FUNCT3())) {
            switch (rs1) {
               /*
               * Integer Extract Instruction
               */
               case 0b00000:
                  fName = GETN_VUnopV(vmvx);
                  fAddr = GETA_VUnopV(vmvx);

                  GETR_VUnopOPI()
                  d = unsafeIRDirty_1_N(ret, 0, fName, fAddr, args);

                  vex_bzero(&d->fxState, sizeof(d->fxState));
                  d->nFxState          = 1;
                  d->fxState[0].fx     = Ifx_Read;
                  d->fxState[0].offset = offsetVReg(rs2);
                  d->fxState[0].size   = host_VLENB;
                  stmt(irsb, IRStmt_Dirty(d));

                  if (rd != 0)
                     putIReg64(irsb, rd, mkexpr(ret));
                  DIP("%s(%s, %s)\n", fName, nameIReg(rd), nameVReg(rs2));
                  return True;
               /*
               * Vector count population in mask vcpop.m
               */
               case 0b10000:
               /*
               * vfirst find-first-set mask bit
               */
               case 0b10001: {
                  IRTemp dret = newTemp(irsb, Ity_I64);
                  if (GET_FUNCT6() == 0b10000) {
                     fName = mask ? GETN_VUnopV(vcpop) : GETN_VUnopV_M(vcpop);
                     fAddr = mask ? GETA_VUnopV(vcpop) : GETA_VUnopV_M(vcpop);
                  } else {
                     fName = mask ? GETN_VUnopV(vfirst) : GETN_VUnopV_M(vfirst);
                     fAddr = mask ? GETA_VUnopV(vfirst) : GETA_VUnopV_M(vfirst);
                  }

                  GETR_VUnopOPI()
                  d = unsafeIRDirty_1_N(dret, 0, fName, fAddr, args);

                  vex_bzero(&d->fxState, sizeof(d->fxState));
                  d->nFxState          = mask ? 1 : 2;
                  d->fxState[0].fx     = Ifx_Read;
                  d->fxState[0].offset = offsetVReg(rs2);
                  d->fxState[0].size   = host_VLENB;
                  d->fxState[1].fx     = Ifx_Read;
                  d->fxState[1].offset = offsetVReg(0);
                  d->fxState[1].size   = host_VLENB;
                  stmt(irsb, IRStmt_Dirty(d));

                  putIReg64(irsb, rd, mkexpr(dret));
                  DIP("%s(%s, %s)\n", fName, nameIReg(rd), nameVReg(rs2));
                  return True;
               }
            }
         } else {
            if (rs2 == 0) {
               GETC_VUnopOPI_VAR(vmvs, NIL, X, NIL, GETV_VopM1D);
               return True;
            }
         }
         break;
      default:
         break;
   }

   return False;
}

#pragma push_macro("RVV_REGO")
#pragma push_macro("RVV_REGN")
#undef  RVV_REGO
#undef  RVV_REGN
#define RVV_REGO offsetFReg
#define RVV_REGN nameFReg
static Bool dis_RV64V_arith_OPF(/*MB_OUT*/ DisResult* dres,
                                /*OUT*/ IRSB*         irsb,
                                UInt                  insn)
{
   IRDirty *d  = NULL;
   void *fAddr = NULL;
   const HChar *fName = NULL;
   IRExpr **args = NULL;
   UInt temp = 0;
   UInt lmul   = extract_lmul(guest_VFLAG);
   IRTemp ret = newTemp(irsb, Ity_I32);
   IRTemp frm = newTemp(irsb, Ity_I32);

   UInt rd   = GET_RD();
   UInt rs1  = GET_RS1();
   UInt rs2  = GET_RS2();
   Bool mask = GET_VMASK();

   switch (GET_FUNCT6()) {
      /*
       * Vector Single-Width Floating-Point Add/Subtract Instructions
       */
      case 0b000000:
         GETC_VBinopOPF(vfadd, V, F);
         return True;
      case 0b000010:
         GETC_VBinopOPF(vfsub, V, F);
         return True;
      case 0b100111:
         GETC_VBinopOPF(vfrsub, NIL, F);
         return True;

      /*
       * Vector Widening Floating-Point Add/Subtract Instructions
       */
      case 0b110000:
         GETC_VBinopOPF_VAR(vfwadd, V, F, GETV_VopWidenD);
         return True;
      case 0b110010:
         GETC_VBinopOPF_VAR(vfwsub, V, F, GETV_VopWidenD);
         return True;
      case 0b110100:
         GETC_VBinopOPF_VAR(vfwaddw, V, F, GETV_VopWidenD | GETV_VopWidenS2);
         return True;
      case 0b110110:
         GETC_VBinopOPF_VAR(vfwsubw, V, F, GETV_VopWidenD | GETV_VopWidenS2);
         return True;
      /*
       * Vector Single-Width Floating-Point Multiply/Divide Instructions
       */
      case 0b100100:
         GETC_VBinopOPF(vfmul, V, F);
         return True;
      case 0b100000:
         GETC_VBinopOPF(vfdiv, V, F);
         return True;
      case 0b100001:
         GETC_VBinopOPF(vfrdiv, NIL, F);
         return True;
      /*
       * Vector Widening Floating-Point Multiply
       */
      case 0b111000:
         GETC_VBinopOPF_VAR(vfwmul, V, F, GETV_VopWidenD);
         return True;
      /*
       * Vector Single-Width Floating-Point Fused Multiply-Add Instructions
       */
      case 0b101100:
         GETC_VBinopOPF_VAR(vfmacc, V, F, GETV_VopAccD);
         return True;
      case 0b101101:
         GETC_VBinopOPF_VAR(vfnmacc, V, F, GETV_VopAccD);
         return True;
      case 0b101110:
         GETC_VBinopOPF_VAR(vfmsac, V, F, GETV_VopAccD);
         return True;
      case 0b101111:
         GETC_VBinopOPF_VAR(vfnmsac, V, F, GETV_VopAccD);
         return True;
      case 0b101000:
         GETC_VBinopOPF_VAR(vfmadd, V, F, GETV_VopAccD);
         return True;
      case 0b101001:
         GETC_VBinopOPF_VAR(vfnmadd, V, F, GETV_VopAccD);
         return True;
      case 0b101010:
         GETC_VBinopOPF_VAR(vfmsub, V, F, GETV_VopAccD);
         return True;
      case 0b101011:
         GETC_VBinopOPF_VAR(vfnmsub, V, F, GETV_VopAccD);
         return True;
      /*
       * Vector Widening Floating-Point Fused Multiply-Add Instructions
       */
      case 0b111100:
         GETC_VBinopOPF_VAR(vfwmacc, V, F, GETV_VopAccD | GETV_VopWidenD);
         return True;
      case 0b111101:
         GETC_VBinopOPF_VAR(vfwnmacc, V, F, GETV_VopAccD | GETV_VopWidenD);
         return True;
      case 0b111110:
         GETC_VBinopOPF_VAR(vfwmsac, V, F, GETV_VopAccD | GETV_VopWidenD);
         return True;
      case 0b111111:
         GETC_VBinopOPF_VAR(vfwnmsac, V, F, GETV_VopAccD | GETV_VopWidenD);
         return True;
      /* VFUnary1 */
      case 0b010011:
         switch (rs1) {
            /*
             * Vector Floating-Point Square-Root Instruction
             */
            case 0b00000:
               GETC_VUnopOPF(vfsqrt, V, NIL);
               return True;
            /*
             * Vector Floating-Point Reciprocal Square-Root Estimate Instruction
             */
            case 0b00100:
               GETC_VUnopOPF(vfrsqrt7, V, NIL);
               return True;
            /*
             * Vector Floating-Point Reciprocal Estimate Instruction
             */
            case 0b00101:
               GETC_VUnopOPF(vfrec7, V, NIL);
               return True;
            /*
             * Vector Floating-Point Classify Instruction
             */
            case 0b10000:
               GETC_VUnopOPF(vfclass, V, NIL);
               return True;
         }
         break;
      /*
       * Vector Floating-Point MIN/MAX Instructions
       */
      case 0b000100:
         GETC_VBinopOPF(vfmin, V, F);
         return True;
      case 0b000110:
         GETC_VBinopOPF(vfmax, V, F);
         return True;
      /*
       * Vector Floating-Point Sign-Injection Instructions
       */
      case 0b001000:
         GETC_VBinopOPF(vfsgnj, V, F);
         return True;
      case 0b001001:
         GETC_VBinopOPF(vfsgnjn, V, F);
         return True;
      case 0b001010:
         GETC_VBinopOPF(vfsgnjx, V, F);
         return True;
      /*
       * Vector Floating-Point Compare Instructions
       */
      case 0b011000:
         GETC_VBinopOPF_VAR(vmfeq, V, F, GETV_VopM1D);
         return True;
      case 0b011100:
         GETC_VBinopOPF_VAR(vmfne, V, F, GETV_VopM1D);
         return True;
      case 0b011011:
         GETC_VBinopOPF_VAR(vmflt, V, F, GETV_VopM1D);
         return True;
      case 0b011001:
         GETC_VBinopOPF_VAR(vmfle, V, F, GETV_VopM1D);
         return True;
      case 0b011101:
         GETC_VBinopOPF_VAR(vmfgt, NIL, F, GETV_VopM1D);
         return True;
      case 0b011111:
         GETC_VBinopOPF_VAR(vmfge, NIL, F, GETV_VopM1D);
         return True;
      /*
       * Vector Floating-Point Merge Instruction
       */
      case 0b010111:
         if (!mask) {
            GETC_VBinopOPF(vfmerge, NIL, F);
         } else {
            GETC_VUnopOPF(vfmerge, NIL, F);
         }
         return True;

      /* VFUnary0 */
      case 0b010010:
         switch (rs1) {
            /*
             * Single-Width Floating-Point/Integer Type-Convert Instructions
             */
            case 0b00000:
               GETC_VUnopOPF(vfcvt_xu_f, V, NIL);
               return True;
            case 0b00001:
               GETC_VUnopOPF(vfcvt_x_f, V, NIL);
               return True;
            case 0b00010:
               GETC_VUnopOPF(vfcvt_f_xu, V, NIL);
               return True;
            case 0b00011:
               GETC_VUnopOPF(vfcvt_f_x, V, NIL);
               return True;
            case 0b00110:
               GETC_VUnopOPF(vfcvt_rtz_xu_f, V, NIL);
               return True;
            case 0b00111:
               GETC_VUnopOPF(vfcvt_rtz_x_f, V, NIL);
               return True;
            /*
             * Widening Floating-Point/Integer Type-Convert Instructions
             */
            case 0b01000:
               GETC_VUnopOPF_VAR(vfwcvt_xu_f, V, NIL, GETV_VopWidenD);
               return True;
            case 0b01001:
               GETC_VUnopOPF_VAR(vfwcvt_x_f, V, NIL, GETV_VopWidenD);
               return True;
            case 0b01010:
               GETC_VUnopOPF_VAR(vfwcvt_f_xu, V, NIL, GETV_VopWidenD);
               return True;
            case 0b01011:
               GETC_VUnopOPF_VAR(vfwcvt_f_x, V, NIL, GETV_VopWidenD);
               return True;
            case 0b01100:
               GETC_VUnopOPF_VAR(vfwcvt_f_f, V, NIL, GETV_VopWidenD);
               return True;
            case 0b01110:
               GETC_VUnopOPF_VAR(vfwcvt_rtz_xu_f, V, NIL, GETV_VopWidenD);
               return True;
            case 0b01111:
               GETC_VUnopOPF_VAR(vfwcvt_rtz_x_f, V, NIL, GETV_VopWidenD);
               return True;
            /*
             * Narrowing Floating-Point/Integer Type-Convert Instructions
             */
            case 0b10000:
               GETC_VUnopOPF_VAR(vfncvt_xu_f, V, NIL, GETV_VopWidenS2);
               return True;
            case 0b10001:
               GETC_VUnopOPF_VAR(vfncvt_x_f, V, NIL, GETV_VopWidenS2);
               return True;
            case 0b10010:
               GETC_VUnopOPF_VAR(vfncvt_f_xu, V, NIL, GETV_VopWidenS2);
               return True;
            case 0b10011:
               GETC_VUnopOPF_VAR(vfncvt_f_x, V, NIL, GETV_VopWidenS2);
               return True;
            case 0b10100:
               GETC_VUnopOPF_VAR(vfncvt_f_f, V, NIL, GETV_VopWidenS2);
               return True;
            case 0b10101:
               GETC_VUnopOPF_VAR(vfncvt_rod_f_f, V, NIL, GETV_VopWidenS2);
               return True;
            case 0b10110:
               GETC_VUnopOPF_VAR(vfncvt_rtz_xu_f, V, NIL, GETV_VopWidenS2);
               return True;
            case 0b10111:
               GETC_VUnopOPF_VAR(vfncvt_rtz_x_f, V, NIL, GETV_VopWidenS2);
               return True;
            default:
               break;
         }
         break;

      /*
       * VWFUNARY0/VRFUNARY0
       */
      case 0b010000:
         if (isVOpVV(GET_FUNCT3())) {
            if (rs1 == 0) {
               IRTemp dret = newTemp(irsb, Ity_I64);
               fName       = GETN_VUnopV(vfmv);
               fAddr       = GETA_VUnopV(vfmv);

               GETR_VUnopOPF()
               d = unsafeIRDirty_1_N(dret, 0, fName, fAddr, args);

               vex_bzero(&d->fxState, sizeof(d->fxState));
               d->nFxState          = 1;
               d->fxState[0].fx     = Ifx_Read;
               d->fxState[0].offset = offsetVReg(rs2);
               d->fxState[0].size   = host_VLENB;
               stmt(irsb, IRStmt_Dirty(d));

               putFReg64(irsb, rd, unop(Iop_ReinterpI64asF64, mkexpr(dret)));
               DIP("%s(%s, %s)\n", fName, nameFReg(rd), nameVReg(rs2));
               return True;
            }
         } else {
            if (rs2 == 0) {
               GETC_VUnopOPF_VAR(vfmvs, NIL, F, GETV_VopM1D);
               return True;
            }
         }
         break;

      /*
       * Vector Single-Width Floating-Point Reduction Instructions
       */
      case 0b000011:
         GETC_VBinopOPF_VAR(vfredosum, V, NIL, GETV_VopM1D);
         return True;
      case 0b000001:
         GETC_VBinopOPF_VAR(vfredusum, V, NIL, GETV_VopM1D);
         return True;
      case 0b000111:
         GETC_VBinopOPF_VAR(vfredmax, V, NIL, GETV_VopM1D);
         return True;
      case 0b000101:
         GETC_VBinopOPF_VAR(vfredmin, V, NIL, GETV_VopM1D);
         return True;
      /*
       * Vector Widening Floating-Point Reduction Instructions
       */
      case 0b110011:
         GETC_VBinopOPF_VAR(vfwredosum, V, NIL, GETV_VopM1D);
         return True;
      case 0b110001:
         GETC_VBinopOPF_VAR(vfwredusum, V, NIL, GETV_VopM1D);
         return True;
      default:
         break;
   }

   return False;
}
#pragma pop_macro("RVV_REGO")
#pragma pop_macro("RVV_REGN")

static Bool dis_RV64V_arith(/*MB_OUT*/ DisResult* dres,
                            /*OUT*/ IRSB*         irsb,
                            UInt                  insn)
{
   Bool ok = False;

   switch (GET_FUNCT3()) {
      case RV64_SOPC_OPIVV:
      case RV64_SOPC_OPIVX:
      case RV64_SOPC_OPIVI:
         ok = dis_RV64V_arith_OPI(dres, irsb, insn);
         break;
      case RV64_SOPC_OPMVV:
      case RV64_SOPC_OPMVX:
         ok = dis_RV64V_arith_OPM(dres, irsb, insn);
         break;
      case RV64_SOPC_OPFVV:
      case RV64_SOPC_OPFVF:
         ok = dis_RV64V_arith_OPF(dres, irsb, insn);
         break;
      default:
         vassert(0);
   }
   return ok;
}

#include "guest_riscv64V0p7_toIR.c"

static Bool dis_RV64V(/*MB_OUT*/ DisResult* dres,
                      /*OUT*/ IRSB*         irsb,
                      UInt                  insn,
                      Addr                  guest_pc_curr_instr)
{
   if (host_VLENB == 0)
      return False;

   if (dis_RV64V0p7(dres, irsb, insn, guest_pc_curr_instr))
      return True;

   Bool ok = False;
   UInt nf = 1;

   switch (GET_OPCODE()) {
      case OPC_OP_V:
         if (GET_FUNCT3() == RV64_SOPC_OPCFG) {
            ok = dis_RV64V_cfg(dres, irsb, insn);
            /* There is a potential vector CSR changing, we stop here */
            if (extract_vstart(guest_VFLAG))
               goto ResetVstart;
            goto ExitBB;
         }
         ok = dis_RV64V_arith(dres, irsb, insn);
         break;
      case OPC_LOAD_FP:
      case OPC_STORE_FP:
         ok = dis_RV64V_ldst(dres, irsb, insn);
         nf += GET_NF();
         break;
      case OPC_SYSTEM:
         ok = dis_RV64V_csr(dres, irsb, insn);
         break;
      default:
         break;
   }
   if (!ok)
      return ok;

   if (GET_OPCODE() == OPC_SYSTEM)
      goto ExitBB;

   /* Reset vstart if necessary and stop the translation if reset occurs. */
   if (extract_vstart(guest_VFLAG) && GET_OPCODE() != OPC_SYSTEM) {
      /* Reset occurs, terminate translation. As cfg instructions will
         directly return, there is no need to handle it. */
      goto ResetVstart;
   }

   if (extract_lmul(guest_VFLAG) * nf > 4) {
      /* FIXME:
         LMUL > 4 will consume more TEMPORARY and need more spilled space, it will cause
         two FATALs:
         a, VEX temporary storage exhausted.
         b, N_SPILL64S is too low in VEX. */
      goto ExitBB;
   }

   return ok;

ResetVstart:
   putVStart(irsb, mkU64(0));
ExitBB:
   putPC(irsb, mkU64(guest_pc_curr_instr + 4));
   dres->whatNext    = Dis_StopHere;
   dres->jk_StopHere = Ijk_ExitBB;
   return ok;
}

/*--------------------------------------------------------------------*/
/*--- end                                  guest_riscv64V_toIR.c ---*/
/*--------------------------------------------------------------------*/
