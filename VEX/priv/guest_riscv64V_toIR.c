
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
#define GET_MOP()   INSN(27, 26)
#define GET_NF()    INSN(31, 29)
#define GET_MEW()   INSN(28, 28)
#define GET_UMOP()  INSN(24, 20)

/*------------------------------------------------------------*/
/*--- Globals                                              ---*/
/*------------------------------------------------------------*/

static Bool  host_VersionV;
ULong host_VLENB;
ULong guest_VFLAG;

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

/* Read a value from VXRM CSR */
static IRExpr* getVxrm(void) {
   IRExpr* vcsr = getVCSR();
   return binop(Iop_And32, binop(Iop_Shr32, unop(Iop_64to32, vcsr), mkU8(1)),
                mkU32(3));
}

/* Write a value to VXRM CSR */
static void putVxrm(IRSB* irsb, IRExpr* e) {
   IRType ty = typeOfIRExpr(irsb->tyenv, e);
   vassert(ty == Ity_I32);
   IRExpr* vcsr = getVCSR();
   IRExpr* new_vcsr = binop(Iop_Or64, binop(Iop_And64, vcsr, mkU64(0xFFFFFFFFFFFFFFF9)),
                            unop(Iop_32Uto64, binop(Iop_Shl32, e, mkU8(1))));
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

/* Vector load/store dirty information filler */
/* Details should be distinguished between RVV 1.0 and 0.7.1:
   (1) Mask elements in RVV 1.0 are lowest single bits in v0 register
       and independent of SEW/LMUL, while mask elements are determined
       by SEW/LMUL in RVV 0.7.1.
   (2) RVV 1.0 uses EEW to encode index values and SEW to encode data
       values. RVV 0.7.1 uniformly uses vtype-encoded SEW to calculate
       index and data.

   To conveniently switch from RVV 1.0 style to RVV 0.7.1 style, we use some
   function pointer arrays to hold what are varied across two spec versions.
*/

typedef void index_addr_filler_func(IRExpr**, UInt, UInt, UInt, UInt, UInt, UInt, UInt);
typedef ULong dirty_get_mask_func(VexGuestRISCV64State*, ULong, UInt, UInt, UInt);

typedef struct {
   index_addr_filler_func* index_addr;
   dirty_get_mask_func* mask_helper;
} AuxilaryHandlers;

static ULong dirty_get_mask(VexGuestRISCV64State *st, ULong mask,
                            __attribute__((unused)) UInt lmul,
                            __attribute__((unused)) UInt sew, UInt idx) {
   ULong res = 0;
   mask += (ULong) st;

   /* Figure out the required 8 mask elements locate in which 16-bit segment. */
   UInt seg16_idx = (idx >> 4);
   UShort mask_seg16 = *(((UShort *) mask) + seg16_idx);
   UInt seg16_sft = idx - (idx >> 4);

   /* Truncate it out of 16-bit segment to 8-bit mask*/
   UChar mask_8 = (UChar) (mask_seg16 >> seg16_sft);

   /* Extend mask_8 to ULong result */
   for (UInt i = 0; i < 8; i++) {
      res |= (((ULong) ((UChar) (mask_8 & 0x01))) << (56 - 8 * i));
      mask_8 = mask_8 >> 1;
   }
   return res;
}

/* Fill mMask array */
static inline IRExpr** calculate_dirty_mask(IRSB *irsb,  /* MOD */
                                            Bool mask, UInt vl,
                                            UInt lmul, UInt sew, UInt vstart,
                                            dirty_get_mask_func* mask_getter) {
   IRExpr** maskV = NULL;
   UInt n_addrs   = vl - vstart;
   if (mask)
      return maskV;
   else
      maskV = LibVEX_Alloc_inline(n_addrs * sizeof(IRExpr*));

   UInt idx = 0;
   while (idx < n_addrs) {
      /* Each iteration handles 8 mask elements until all elements consumed. */
      IRTemp   mask_segs = newTemp(irsb, Ity_I64);
      IRExpr** args = mkIRExprVec_5(IRExpr_GSPTR(), mkU64(offsetVReg(0)), mkU32(lmul),
                                    mkU32(sew), mkU32(vstart + idx));
      IRDirty *m_d = unsafeIRDirty_1_N(mask_segs, 0, "dirty_get_mask",
                                       mask_getter, args);
      /* The following claims are used to pass the dirty sanity check. As
         sanity check for dirty helpers with guest pointer as its argument
         compulsively require fxState.fx/offset/size, we point it to always
         defined x0 register in riscv64. Only to pass the sanity check,
         the actual v0 access is marked in GETD_Common_VLdSt. */
      vex_bzero(&m_d->fxState, sizeof(m_d->fxState));
      m_d->nFxState          = 1;
      m_d->fxState[0].fx     = Ifx_Read;
      m_d->fxState[0].offset = offsetIReg64(0);
      m_d->fxState[0].size   = 1;

      UInt n_mask = n_addrs - idx < 8 ? n_addrs - idx : 8;
      for (UInt i = 0; i < n_mask; i++)
         maskV[idx++] = unop(Iop_64to1, binop(Iop_Shr64, mkexpr(mask_segs), mkU8(i * 8)));
      stmt(irsb, IRStmt_Dirty(m_d));
   }
   return maskV;
}

/* The parameter sew is actually 'width' in RVV 1.0. It is real sew in RVV 0.7.1. */
static inline void index_addr_filler(IRExpr** addrV, /* OUT */
                                     UInt vstart, UInt vl, UInt r, UInt s2,
                                     __attribute__((unused)) UInt width,
                                     UInt sew, UInt idx) {
   UInt sew_lg2 = 31 - __builtin_clz((UInt) sew);
   IROp       ops[4] = {Iop_8Sto64, Iop_16Sto64, Iop_32Sto64, Iop_LAST};
   IRType off_tys[4] = {Ity_I8, Ity_I16, Ity_I32, Ity_I64};
   vassert(sew_lg2 >=0 && sew_lg2 <= 3);
   IRExpr* offset;
   for (UInt i = vstart; i < vl; i++) {
      if (ops[sew_lg2] != Iop_LAST)
         offset = unop(ops[sew_lg2], getVRegLane(s2, i, off_tys[sew_lg2]));
      else
         offset = getVRegLane(s2, i, off_tys[sew_lg2]);
      addrV[idx++] = binop(Iop_Add64, getIReg64(r), offset);
   }
}

static AuxilaryHandlers RVV_aux_handler = {&index_addr_filler, &dirty_get_mask};

/* Prepare Vload/Vstore dirty helper info */
static IRDirty*
GETD_Common_VLdSt(IRSB *irsb,                /* MOD */
                  IRDirty* d,                /* OUT */
                  UInt v, UInt r, UInt s2,   /* Inst oprd */
                  Bool mask, Bool isLD, UInt lmul, UInt sew,
                  UInt nf, ULong width,
                  VLdstT ldst_ty, AuxilaryHandlers handlers)
{
   UInt vstart = extract_vstart(guest_VFLAG);
   UInt vl     = extract_vl(guest_VFLAG);
   d->nFxState = 2;
   vex_bzero(&d->fxState, sizeof(d->fxState));
   IRExpr** addrV = NULL;

   /* Mark memory effect: address and mask */
   d->mFx         = isLD ? Ifx_Read : Ifx_Write;
   d->mSize       = width * nf;
   if (mask && ldst_ty == UnitStride) {
      d->mAddr = binop(Iop_Add64, getIReg64(r), mkU64(vstart * d->mSize));
      d->mSize *= vl - vstart;
   } else {
      d->mNAddrs = vl - vstart;
      addrV = LibVEX_Alloc_inline(d->mNAddrs * sizeof(IRExpr*));
   }

   /* Address info */
   UInt idx = 0;
   switch (ldst_ty) {
      case UnitStride:
         if (mask)
            break;
         for (UInt i = vstart; i < vl; i++)
            addrV[idx++] = binop(Iop_Add64, getIReg64(r), mkU64(i * d->mSize));
         break;
      case Strided: {
         for (UInt i = vstart; i < vl; i++)
            addrV[idx++] = binop(Iop_Add64, getIReg64(r),
                                 binop(Iop_Mul64, mkU64(i), getIReg64(s2)));
         break;
      }
      case Indexed: {
         /* Dispatch between RVV 1.0 and 0.7.1 occurs in argumant passing */
         handlers.index_addr(addrV, vstart, vl, r, s2, sew, width, idx);
         break;
      }
      default:
         vassert(0);
   }
   d->mAddrVec = addrV;

   /* Mask info */
   d->mMask = calculate_dirty_mask(irsb, mask, vl, lmul, sew, vstart, handlers.mask_helper);

   /* Mark vector register modified */
   d->fxState[0].fx        = isLD ? Ifx_Write : Ifx_Read;
   d->fxState[0].offset    = offsetVReg(v);
   d->fxState[0].size      = host_VLENB;
   d->fxState[0].nRepeats  = lmul * nf - 1;
   d->fxState[0].repeatLen = lmul * nf - 1 == 0 ? 0 : host_VLENB;

   /* Mark gpr state modified */
   d->fxState[1].fx        = Ifx_Read;
   d->fxState[1].offset    = offsetIReg64(r);
   d->fxState[1].size      = 8;

   switch (ldst_ty) {
      case Strided:
         d->fxState[2].fx     = Ifx_Read;
         d->fxState[2].offset = offsetIReg64(s2);
         d->fxState[2].size   = 8;
         d->nFxState += 1;
         break;
      case Indexed:
         d->fxState[2].fx        = Ifx_Read;
         d->fxState[2].offset    = offsetVReg(s2);
         d->fxState[2].size      = host_VLENB;
         d->fxState[2].nRepeats  = lmul - 1;
         d->fxState[2].repeatLen = lmul - 1 == 0 ? 0 : host_VLENB;
         d->nFxState += 1;
         break;
      default:
         break;
   }

   /* Mark v0 register is read if mask == 0 */
   if (!mask) {
      d->fxState[d->nFxState].fx     = Ifx_Read;
      d->fxState[d->nFxState].offset = offsetVReg(0);
      d->fxState[d->nFxState].size   = host_VLENB;
      d->nFxState += 1;
   }
   return d;
}

static inline Bool RVV_is_normal_load(UInt nf, Bool isLD) {
   return nf == 1 && isLD;
}

static inline Bool RVV_is_normal_store(UInt nf, Bool isLD) {
   return nf == 1 && !isLD;
}

static inline Bool RVV_is_seg_load(UInt nf, Bool isLD) {
   return nf != 1 && isLD;
}

static inline Bool RVV_is_seg_store(UInt nf, Bool isLD) {
   return nf != 1 && !isLD;
}

#include "guest_riscv64V_helpers.c"

static Bool dis_RV64V_csr(/*MB_OUT*/ DisResult* dres,
                          /*OUT*/ IRSB* irsb,
                          UInt insn)
{
   UInt rd  = GET_RD();
   UInt rs1 = GET_RS1();
   UInt csr = INSN(31, 20);

   if (GET_FUNCT3() == RV64_SOPC_CSRRW) {
      DIP("csrrw %s, %s, %s\n", nameIReg(rd), nameVCSR(csr), nameIReg(rs1));
      switch (csr) {
         case 0x008: {
            /* vstart: URW */
            IRExpr *eR1 = getIReg64(rs1);
            IRExpr *orig_vstart = getVStart();

            /* This is quoted from RVV 1.x spec 3.4:
               The vstart CSR is defined to have only enough writable bits to hold the
               largest element index (one less than the maximum VLMAX) or lg2(VLEN) bits.
               The upper bits of the vstart CSR are hardwired to zero (reads zero, writes
               ignored).*/
            ULong eff_mask = 0xFFFFFFFFFFFFFFFF >> (__builtin_clzll(host_VLENB) - 2);
            IRExpr* eff_vstart = binop(Iop_And64, mkU64(eff_mask), eR1);
            putVStart(irsb, eff_vstart);
            if (rd != 0)
               putIReg64(irsb, rd, orig_vstart);
            return True;
         }
         case 0x009: {
            /* vxsat: URW */
            IRExpr *eR1 = getIReg32(rs1);

            /* vxsat has a single effective bit. */
            IRExpr *eff_vxsat = binop(Iop_And32, mkU32(0x01), eR1);
            IRExpr *orig_vxsat = getVxsat();
            putVxsat(irsb, eff_vxsat);
            if (rd != 0)
               putIReg32(irsb, rd, orig_vxsat);
            return True;
         }
         case 0x00A: {
            /* vxrm: URW */
            IRExpr *eR1 = getIReg32(rs1);
            IRExpr *orig_vxrm = getVxrm();

            /* vxrm has 2 effective bits in [1:0], [XLEN-1:2] bits should be zeros. */
            IRExpr *eff_vxrm = binop(Iop_And32, mkU32(0x03), eR1);
            putVxrm(irsb, eff_vxrm);
            if (rd != 0)
               putIReg32(irsb, rd, orig_vxrm);
            return True;
         }
         case 0x00F: {
            /* vcsr: URW */
            IRExpr *eff_vcsr = getIReg64(rs1);
            IRExpr *orig_vcsr = getVCSR();
            putVCSR(irsb, eff_vcsr);
            if (rd != 0)
               putIReg64(irsb, rd, orig_vcsr);
            return True;
         }
         /* VL/VTYPE are read-only CSRs */
         default:
            return False;
      }
   }

   if (GET_FUNCT3() == RV64_SOPC_CSRRS) {
      DIP("csrrs %s, %s, %s\n", nameIReg(rd), nameVCSR(csr), nameIReg(rs1));
      switch (csr) {
         case 0x008: {
            /* vstart: URW */
            IRExpr *orig_vstart = getVStart();
            IRExpr *eR1 = getIReg64(rs1);
            ULong eff_mask = 0xFFFFFFFFFFFFFFFF >> (__builtin_clzll(host_VLENB) - 2);
            IRExpr* eff_bits = binop(Iop_And64, mkU64(eff_mask), eR1);
            IRExpr* eff_vstart = binop(Iop_Or64, eff_bits, orig_vstart);
            putVStart(irsb, eff_vstart);
            if (rd != 0)
               putIReg64(irsb, rd, orig_vstart);
            return True;
         }
         case 0x009: {
            /* vxsat: URW */
            IRExpr *orig_vxsat = getVxsat();
            IRExpr *eR1 = getIReg32(rs1);
            IRExpr* eff_bits = binop(Iop_And32, mkU32(0x01), eR1);
            IRExpr* eff_vxsat = binop(Iop_Or32, eff_bits, orig_vxsat);
            putVxsat(irsb, eff_vxsat);
            if (rd != 0)
               putIReg32(irsb, rd, orig_vxsat);
            return True;
         }
         case 0x00A: {
            /* vxrm: URW */
            IRExpr *orig_vxrm = getVxrm();
            IRExpr *eR1 = getIReg32(rs1);
            IRExpr* eff_bits = binop(Iop_And32, mkU32(0x03), eR1);
            IRExpr* eff_vxrm = binop(Iop_Or32, eff_bits, orig_vxrm);
            putVxrm(irsb, eff_vxrm);
            if (rd != 0)
               putIReg32(irsb, rd, orig_vxrm);
            return True;
         }
         case 0x00F: {
            /* vcsr: URW */
            IRExpr *eR1 = getIReg64(rs1);
            IRExpr *orig_vcsr = getVCSR();

            IRExpr *eff_vcsr = binop(Iop_Or64, orig_vcsr, eR1);
            putVCSR(irsb, eff_vcsr);
            if (rd != 0)
               putIReg64(irsb, rd, orig_vcsr);
            return True;
         }
         case 0xC20: {
            /* VL: URO */
            if (rs1 != 0)
               /* Attempt to write a value to URO CSR. */
               return False;
            if (rd != 0)
               putIReg64(irsb, rd, getVL());
            return True;
         }
         case 0xC21: {
            /* VTYPE: URO */
            if (rs1 != 0)
               /* Attempt to write a value to URO CSR. */
               return False;
            if (rd != 0)
               putIReg64(irsb, rd, getVType());
            return True;
         }
         case 0xC22: {
            /* VLENB: URO */
            if (rs1 != 0)
               /* Attempt to write a value to URO CSR. */
               return False;
            if (rd != 0)
               putIReg64(irsb, rd, mkU64(host_VLENB));
            return True;
         }
         default:
            return False;
      }
   }

   if (GET_FUNCT3() == RV64_SOPC_CSRRC) {
      DIP("csrrc %s, %s, %s\n", nameIReg(rd), nameVCSR(csr), nameIReg(rs1));
      switch (csr) {
         case 0x008: {
            /* vstart: URW */
            IRExpr *orig_vstart = getVStart();
            IRExpr *eR1 = getIReg64(rs1);
            ULong eff_mask = 0xFFFFFFFFFFFFFFFF >> (__builtin_clzll(host_VLENB) - 2);
            IRExpr* eff_bits = binop(Iop_And64, mkU64(eff_mask), eR1);
            IRExpr* eff_vstart = binop(Iop_And64, binop(Iop_Xor64, eff_bits, orig_vstart),
                                       orig_vstart);
            putVStart(irsb, eff_vstart);
            if (rd != 0)
               putIReg64(irsb, rd, orig_vstart);
            return True;
         }
         case 0x009: {
            /* vxsat: URW */
            IRExpr *orig_vxsat = getVxsat();
            IRExpr *eR1 = getIReg32(rs1);
            IRExpr* eff_bits = binop(Iop_And32, mkU32(0x01), eR1);
            IRExpr* eff_vxsat = binop(Iop_And32, binop(Iop_Xor32, eff_bits, orig_vxsat),
                                      orig_vxsat);
            putVxsat(irsb, eff_vxsat);
            if (rd != 0)
               putIReg32(irsb, rd, orig_vxsat);
            return True;
         }
         case 0x00A: {
            /* vxrm: URW */
            IRExpr *orig_vxrm = getVxrm();
            IRExpr *eR1 = getIReg32(rs1);
            IRExpr* eff_bits = binop(Iop_And32, mkU32(0x03), eR1);
            IRExpr* eff_vxrm = binop(Iop_And32, binop(Iop_Xor32, eff_bits, orig_vxrm),
                                     orig_vxrm);
            putVxrm(irsb, eff_vxrm);
            if (rd != 0)
               putIReg32(irsb, rd, orig_vxrm);
            return True;
         }
         case 0x00F: {
            /* vcsr: URW */
            IRExpr *eR1 = getIReg64(rs1);
            IRExpr *orig_vcsr = getVCSR();
            IRExpr *eff_vcsr = binop(Iop_And64, binop(Iop_Xor64, eR1, orig_vcsr),
                                     orig_vcsr);
            putVCSR(irsb, eff_vcsr);
            if (rd != 0)
               putIReg64(irsb, rd, orig_vcsr);
            return True;
         }
         case 0xC20: {
            /* VL: URO */
            if (rs1 != 0)
               /* Attempt to write a value to URO CSR. */
               return False;
            if (rd != 0)
               putIReg64(irsb, rd, getVL());
            return True;
         }
         case 0xC21: {
            /* VTYPE: URO */
            if (rs1 != 0)
               /* Attempt to write a value to URO CSR. */
               return False;
            if (rd != 0)
               putIReg64(irsb, rd, getVType());
            return True;
         }
         case 0xC22: {
            /* VLENB: URO */
            if (rs1 != 0)
               /* Attempt to write a value to URO CSR. */
               return False;
            if (rd != 0)
               putIReg64(irsb, rd, mkU64(host_VLENB));
            return True;
         }
         default:
            return False;
      }
   }
   return False;
}

static Bool dis_RV64V_cfg(/*MB_OUT*/ DisResult* dres,
                          /*OUT*/ IRSB*         irsb,
                          UInt                  insn)
{
   Bool is_vsetvl = GET_FUNCT7() == 0b1000000;
   Bool is_vsetivli = INSN(31, 30) == 0b11;
   UInt rd  = GET_RD();
   UInt rs1 = GET_RS1();

   if (is_vsetvl) {
      /* In vsetvl case, we cannot immediately get the written value from GPR,
         therefore we terminate translation and return to scheduler. */
      UInt   rs2 = GET_RS2();
      IRExpr* s2 = getIReg64(rs2);
      putVType(irsb, binop(Iop_And64, s2, mkU64(0xFF)));
   } else {
      UInt imm = is_vsetivli ? INSN(29, 20) : INSN(30, 20);
      putVType(irsb, binop(Iop_And64, mkU64(imm), mkU64(0xFF)));
   }

   /* Update VL */
   IRExpr* new_vl = is_vsetivli ? mkU64(rs1) : NULL;
   if (new_vl == NULL) {
      IRExpr* vtype  = getVType();
      IRExpr* lmul   = binop(Iop_And64, vtype, mkU64(0x07));
      IRExpr* sew    = binop(Iop_Shr64, binop(Iop_And64, vtype, mkU64(0x38)), mkU8(3));

      /* handle factional LMUL */
      IRExpr* lmul_fbit  = binop(Iop_And64, binop(Iop_Shr64, vtype, mkU8(2)), mkU64(0x01));
      IRExpr* lmul_shift = binop(Iop_Sub64, mkU64(8), lmul);
      IRExpr* vl_frac    = binop(Iop_Shr64, binop(Iop_Shr64, mkU64(host_VLENB),
                                                  unop(Iop_64to8, lmul_shift)),
                                 unop(Iop_64to8, sew));
      /* integer lmul generated VL_MAX */
      IRExpr* vl_int = binop(Iop_Shr64, binop(Iop_Shl64, mkU64(host_VLENB), unop(Iop_64to8, lmul)),
                             unop(Iop_64to8, sew));
      /* choose the right one */
      IRExpr* vl_max = IRExpr_ITE(unop(Iop_64to1, lmul_fbit), vl_frac, vl_int);

      if (rs1 == 0 && rd == 0)
         return True;
      if (rs1 == 0)
         new_vl = vl_max;
      else
         new_vl = IRExpr_ITE(binop(Iop_CmpLT64U, getIReg64(rs1), vl_max),
                             getIReg64(rs1), vl_max);
   }
   putVL(irsb, new_vl);

   if (rd != 0)
      putIReg64(irsb, rd, new_vl);

   return True;
}

static inline Bool RVV_is_unitstride(UInt insn, Bool isLD) {
   return GET_MOP() == 0b00;
}

static inline Bool RVV_is_strided(UInt insn, Bool isLD) {
   return GET_MOP() == 0b10;
}

static inline Bool RVV_is_indexed(UInt insn, Bool isLD, UInt nf) {
   return GET_MOP() == 0b11 || GET_MOP() == 0b01;
}

#define WHOLE_LOAD_NF_CASES(width)       \
   switch (nf) {                         \
      case 1: {                          \
         GETC_VWHOLELDST(vl1re##width);  \
         return True;                    \
      }                                  \
      case 2: {                          \
         GETC_VWHOLELDST(vl2re##width);  \
         return True;                    \
      }                                  \
      case 4: {                          \
         GETC_VWHOLELDST(vl4re##width);  \
         return True;                    \
      }                                  \
      case 8: {                          \
         GETC_VWHOLELDST(vl8re##width);  \
         return True;                    \
      }                                  \
      default:                           \
         return False;                   \
   }

static Bool dis_RV64V_ldst(/*MB_OUT*/ DisResult* dres,
                           /*OUT*/ IRSB*         irsb,
                           UInt                  insn)
{
   Bool isLD          = GET_OPCODE() == OPC_LOAD_FP;
   IRDirty *d         = NULL;
   void *fAddr        = NULL;
   const HChar *fName = NULL;
   IRExpr **args      = NULL;
   ULong width;

   UInt v     = GET_RD();     /* vd for load *or* vs3 for store */
   UInt rs    = GET_RS1();    /* base address register rs1 */
   UInt nf    = GET_NF() + 1; /* nf value */
   Bool mask  = GET_VMASK();  /* if mask is enabled? */
   UInt mew   = GET_MEW();
   UInt sew   = extract_sew(guest_VFLAG);
   UInt lmul  = extract_lmul(guest_VFLAG);
   Bool whole = (GET_UMOP() == 0b01000) && RVV_is_unitstride(insn, isLD);
   Bool mldst = (GET_UMOP() == 0b01011) && RVV_is_unitstride(insn, isLD);

   switch (GET_FUNCT3()) {
      case 0b000:
         width = 1;
         break;
      case 0b101:
         width = 2;
         break;
      case 0b110:
         width = 4;
         break;
      case 0b111:
         width = 8;
         break;
      default:
         return False;
   }

   if (mew)
      return False;

   if (whole) {
      if (isLD) {
         switch (width) {
            case 1: {
               WHOLE_LOAD_NF_CASES(8);
            }
            case 2: {
               WHOLE_LOAD_NF_CASES(16);
            }
            case 4: {
               WHOLE_LOAD_NF_CASES(32);
            }
            case 8: {
               WHOLE_LOAD_NF_CASES(64);
            }
            default:
               /* Not reached */
               return False;
         }
      } else {
         switch (nf) {
            case 1: {
               GETC_VWHOLELDST(vs1r);
               return True;
            }
            case 2: {
               GETC_VWHOLELDST(vs2r);
               return True;
            }
            case 4: {
               GETC_VWHOLELDST(vs4r);
               return True;
            }
            case 8: {
               GETC_VWHOLELDST(vs8r);
               return True;
            }
            default:
               return False;
         }
      }
   }

   if (mldst) {
      if (isLD) {
         GETC_VLDST(vlm);
         return True;
      }
      else {
         GETC_VLDST(vsm);
         return True;
      }
      return False;
   }

   /* unit stride */
   if (RVV_is_unitstride(insn, isLD)) {
      /* unit-stride normal load */
      if (RVV_is_normal_load(nf, isLD)) {
         switch (width) {
            case 1: {                 /* 8-bit load */
               if (GET_UMOP() == 0b10000) {
                  GETC_VLDST(vle8ff);
               } else if (GET_UMOP() == 0b00000) {
                  GETC_VLDST(vle8);
               } else
                  return False;
               return True;
            }
            case 2: {                 /* 16-bit load */
               if (GET_UMOP() == 0b10000) {
                  GETC_VLDST(vle16ff);
               } else if (GET_UMOP() == 0b00000) {
                  GETC_VLDST(vle16);
               } else
                  return False;
               return True;
            }
            case 4: {                 /* 32-bit load */
               if (GET_UMOP() == 0b10000) {
                  GETC_VLDST(vle32ff);
               } else if (GET_UMOP() == 0b00000) {
                  GETC_VLDST(vle32);
               } else
                  return False;
               return True;
            }
            case 8: {                 /* 64-bit load */
               if (GET_UMOP() == 0b10000) {
                  GETC_VLDST(vle64ff);
               } else if (GET_UMOP() == 0b00000) {
                  GETC_VLDST(vle64);
               } else
                  return False;
               return True;
            }
            default:
               return False;
         }
      }

      if (RVV_is_seg_load(nf, isLD)) { /* unit-stride segment load */
         switch (width) {
            case 1: {
               VSEG_DIS_NF_CASES(GETC_VSEGLDST, vlseg, e8);
               return True;
            }
            case 2: {
               VSEG_DIS_NF_CASES(GETC_VSEGLDST, vlseg, e16);
               return True;
            }
            case 4: {
               VSEG_DIS_NF_CASES(GETC_VSEGLDST, vlseg, e32);
               return True;
            }
            case 8: {
               VSEG_DIS_NF_CASES(GETC_VSEGLDST, vlseg, e64);
               return True;
            }
            default:
               return False;
         }
      }

      /* unit-stride normal store */
      if (RVV_is_normal_store(nf, isLD)){
         switch (width) {
            case 1: {                 /* 8-bit store */
               GETC_VLDST(vse8);
               return True;
            }
            case 2: {                 /* 16-bit store */
               GETC_VLDST(vse16);
               return True;
            }
            case 4: {                 /* 32-bit store */
               GETC_VLDST(vse32);
               return True;
            }
            case 8: {                 /* 64-bit store */
               GETC_VLDST(vse64);
               return True;
            }
            default:
               return False;
         }
      }

      if (RVV_is_seg_store(nf, isLD)) {  /* unit-stride segment store */
         switch (width) {
            case 1: {
               VSEG_DIS_NF_CASES(GETC_VSEGLDST, vsseg, e8);
               return True;
            }
            case 2: {
               VSEG_DIS_NF_CASES(GETC_VSEGLDST, vsseg, e16);
               return True;
            }
            case 4: {
               VSEG_DIS_NF_CASES(GETC_VSEGLDST, vsseg, e32);
               return True;
            }
            case 8: {
               VSEG_DIS_NF_CASES(GETC_VSEGLDST, vsseg, e64);
               return True;
            }
            default:
               return False;
         }
      }
      return False;
   }

   /* strided */
   if (RVV_is_strided(insn, isLD)) {      /* strided store */
      UInt rs2 = GET_RS2(); /* stride register */
      /* strided normal load */
      if (RVV_is_normal_load(nf, isLD)) {
         switch (width) {
            case 1: {                 /* 8-bit load */
               GETC_VSLDST(vlse8);
               return True;
            }
            case 2: {                 /* 16-bit load */
               GETC_VSLDST(vlse16);
               return True;
            }
            case 4: {                 /* 32-bit load */
               GETC_VSLDST(vlse32);
               return True;
            }
            case 8: {                 /* 64-bit load */
               GETC_VSLDST(vlse64);
               return True;
            }
         }
      }

      if (RVV_is_seg_load(nf, isLD)) { /* strided segment load */
         switch (width) {
            case 1: {
               VSEG_DIS_NF_CASES(GETC_VSSEGLDST, vlsseg, e8);
               return True;
            }
            case 2: {
               VSEG_DIS_NF_CASES(GETC_VSSEGLDST, vlsseg, e16);
               return True;
            }
            case 4: {
               VSEG_DIS_NF_CASES(GETC_VSSEGLDST, vlsseg, e32);
               return True;
            }
            case 8: {
               VSEG_DIS_NF_CASES(GETC_VSSEGLDST, vlsseg, e64);
               return True;
            }
            default:
               return False;
         }
      }

      /* strided normal store */
      if (RVV_is_normal_store(nf, isLD)) {
         switch (width) {
            case 1: {                 /* 8-bit store */
               GETC_VSLDST(vsse8);
               return True;
            }
            case 2: {                 /* 16-bit store */
               GETC_VSLDST(vsse16);
               return True;
            }
            case 4: {                 /* 32-bit store */
               GETC_VSLDST(vsse32);
               return True;
            }
            case 8: {                 /* 64-bit store */
               GETC_VSLDST(vsse64);
               return True;
            }
            default:
               return False;
         }
      }

      if (RVV_is_seg_store(nf, isLD)) { /* strided segment store */
         switch (width) {
            case 1: {
               VSEG_DIS_NF_CASES(GETC_VSSEGLDST, vssseg, e8);
               return True;
            }
            case 2: {
               VSEG_DIS_NF_CASES(GETC_VSSEGLDST, vssseg, e16);
               return True;
            }
            case 4: {
               VSEG_DIS_NF_CASES(GETC_VSSEGLDST, vssseg, e32);
               return True;
            }
            case 8: {
               VSEG_DIS_NF_CASES(GETC_VSSEGLDST, vssseg, e64);
               return True;
            }
            default:
               return False;
         }
      }
      return False;
   }

#define INDEX_ORDER_UNORDER(GET_C_MACRO, ldst_prefix, mid, suffix) \
   if (GET_MOP() == 0b11) {                                        \
      GET_C_MACRO(ldst_prefix##ox##mid##suffix);                   \
      return True;                                                 \
   } else if (GET_MOP() == 0b01) {                                 \
      GET_C_MACRO(ldst_prefix##ux##mid##suffix);                   \
      return True;                                                 \
   } else                                                          \
      return False;

#define INDEXED_NORMAL(GET_C_MACRO, ldst_prefix, suffix) \
INDEX_ORDER_UNORDER(GET_C_MACRO, ldst_prefix, ei, suffix)

#define VSEG_DIS_INDEX_NF_CASES(ldst_prefix, suffix)                          \
   switch (nf) {                                                              \
      case 2: {                                                               \
         INDEX_ORDER_UNORDER(GETC_VXSEGLDST, ldst_prefix, seg##2##ei, suffix) \
      }                                                                       \
      case 3: {                                                               \
         INDEX_ORDER_UNORDER(GETC_VXSEGLDST, ldst_prefix, seg##3##ei, suffix) \
      }                                                                       \
      case 4: {                                                               \
         INDEX_ORDER_UNORDER(GETC_VXSEGLDST, ldst_prefix, seg##4##ei, suffix) \
      }                                                                       \
      case 5: {                                                               \
         INDEX_ORDER_UNORDER(GETC_VXSEGLDST, ldst_prefix, seg##5##ei, suffix) \
      }                                                                       \
      case 6: {                                                               \
         INDEX_ORDER_UNORDER(GETC_VXSEGLDST, ldst_prefix, seg##6##ei, suffix) \
      }                                                                       \
      case 7: {                                                               \
         INDEX_ORDER_UNORDER(GETC_VXSEGLDST, ldst_prefix, seg##7##ei, suffix) \
      }                                                                       \
      case 8: {                                                               \
         INDEX_ORDER_UNORDER(GETC_VXSEGLDST, ldst_prefix, seg##8##ei, suffix) \
      }                                                                       \
      default:                                                                \
         return False;                                                        \
   }

   /* indexed */
   if (RVV_is_indexed(insn, isLD, nf)) {
      UInt vs2 = GET_RS2(); /* index register */
      /* indexed normal load */
      if (RVV_is_normal_load(nf, isLD)) {
         switch (width) {
            case 1: {
               INDEXED_NORMAL(GETC_VXLDST, vl, 8)
            }
            case 2: {
               INDEXED_NORMAL(GETC_VXLDST, vl, 16)
            }
            case 4: {
               INDEXED_NORMAL(GETC_VXLDST, vl, 32)
            }
            case 8: {
               INDEXED_NORMAL(GETC_VXLDST, vl, 64)
            }
            default:
               return False;
         }
      }

      if (RVV_is_seg_load(nf, isLD)) { /* indexed segment load */
         switch (width) {
            case 1: {
               VSEG_DIS_INDEX_NF_CASES(vl, 8);
            }
            case 2: {
               VSEG_DIS_INDEX_NF_CASES(vl, 16);
            }
            case 4: {
               VSEG_DIS_INDEX_NF_CASES(vl, 32);
            }
            case 8: {
               VSEG_DIS_INDEX_NF_CASES(vl, 64);
            }
            default:
               return False;
         }
      }

      /* indexed normal store */
      if (RVV_is_normal_store(nf, isLD)) {
         switch (width) {
            case 1: {
               INDEXED_NORMAL(GETC_VXLDST, vs, 8)
            }
            case 2: {
               INDEXED_NORMAL(GETC_VXLDST, vs, 16)
            }
            case 4: {
               INDEXED_NORMAL(GETC_VXLDST, vs, 32)
            }
            case 8: {
               INDEXED_NORMAL(GETC_VXLDST, vs, 64)
            }
            default:
               return False;
         }
      }

      if (RVV_is_seg_store(nf, isLD)) { /* indexed segment store */
         switch (width) {
            case 1: {
               VSEG_DIS_INDEX_NF_CASES(vs, 8);
            }
            case 2: {
               VSEG_DIS_INDEX_NF_CASES(vs, 16);
            }
            case 4: {
               VSEG_DIS_INDEX_NF_CASES(vs, 32);
            }
            case 8: {
               VSEG_DIS_INDEX_NF_CASES(vs, 64);
            }
            default:
               return False;
         }
      }
      return False;
   }
   return False;

#undef INDEX_ORDER_UNORDER
#undef INDEXED_NORMAL
#undef VSEG_DIS_INDEX_NF_CASES
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
         if (isVOpVI(GET_FUNCT3())) {
            /*
             * Whole Vector Register Move
             */
            switch (rs1) {
               case 0:
                  GETC_VUnopOP_T(vmv1r, NIL, NIL, V, GETR_VBinopOPI, GETV_VopUnknow, lmul);
                  return True;
               case 1:
                  GETC_VUnopOP_T(vmv2r, NIL, NIL, V, GETR_VBinopOPI, GETV_VopUnknow, lmul);
                  return True;
               case 3:
                  GETC_VUnopOP_T(vmv4r, NIL, NIL, V, GETR_VBinopOPI, GETV_VopUnknow, lmul);
                  return True;
               case 7:
                  GETC_VUnopOP_T(vmv8r, NIL, NIL, V, GETR_VBinopOPI, GETV_VopUnknow, lmul);
                  return True;
            }
            return False;
         }
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
         if (isVOpVV(GET_FUNCT3())) {
            GETC_VBinopOPI(vrgatherei16, V, NIL, NIL);
            return True;
         }
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
   IRTemp ret = newTemp(irsb, Ity_I32);
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
               case 0b00000: {
                  IRTemp dret = newTemp(irsb, Ity_I64);
                  fName = GETN_VUnopV(vmvx);
                  fAddr = GETA_VUnopV(vmvx);

                  args = mkIRExprVec_5(IRExpr_GSPTR(), mkU64(offsetVReg(rd)), mkU64(offsetVReg(rs2)),
                                       mkU64(offsetVReg(0)), mkU64(offsetVReg(0)));
                  d = unsafeIRDirty_1_N(dret, 0, fName, fAddr, args);

                  vex_bzero(&d->fxState, sizeof(d->fxState));
                  d->nFxState          = 1;
                  d->fxState[0].fx     = Ifx_Read;
                  d->fxState[0].offset = offsetVReg(rs2);
                  d->fxState[0].size   = host_VLENB;
                  stmt(irsb, IRStmt_Dirty(d));

                  if (rd != 0)
                     putIReg64(irsb, rd, mkexpr(dret));
                  DIP("%s(%s, %s)\n", fName, nameIReg(rd), nameVReg(rs2));
                  return True;
               }
               /*
               * Vector count population in mask vcpop.m
               */
               case 0b10000:
               /*
               * vfirst find-first-set mask bit
               */
               case 0b10001: {
                  IRTemp dret = newTemp(irsb, Ity_I64);
                  if (rs1 == 0b10000) {
                     fName = mask ? GETN_VUnopV(vcpop) : GETN_VUnopV_M(vcpop);
                     fAddr = mask ? GETA_VUnopV(vcpop) : GETA_VUnopV_M(vcpop);
                  } else {
                     fName = mask ? GETN_VUnopV(vfirst) : GETN_VUnopV_M(vfirst);
                     fAddr = mask ? GETA_VUnopV(vfirst) : GETA_VUnopV_M(vfirst);
                  }

                  args = mkIRExprVec_5(IRExpr_GSPTR(), mkU64(offsetVReg(rd)), mkU64(offsetVReg(rs2)),
                                       mkU64(offsetVReg(0)), mkU64(offsetVReg(0)));
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
      /*
       * VXUNARY0
       */
      case 0b010010:
         switch (rs1) {
            case 0b00010:
               GETC_VUnopOPI(vzextf8, V, NIL, NIL);
               return True;
            case 0b00011:
               GETC_VUnopOPI(vsextf8, V, NIL, NIL);
               return True;
            case 0b00100:
               GETC_VUnopOPI(vzextf4, V, NIL, NIL);
               return True;
            case 0b00101:
               GETC_VUnopOPI(vsextf4, V, NIL, NIL);
               return True;
            case 0b00110:
               GETC_VUnopOPI(vzextf2, V, NIL, NIL);
               return True;
            case 0b00111:
               GETC_VUnopOPI(vsextf2, V, NIL, NIL);
               return True;
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

               args = mkIRExprVec_6(IRExpr_GSPTR(), mkU64(offsetVReg(rd)), mkU64(offsetVReg(rs2)),
                                    mkU64(offsetVReg(0)), mkU64(offsetVReg(0)), mkU64(0));
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
      /*
       * Vector Floating-Point Slide1up Instructions
       */
      case 0b001110:
         GETC_VBinopOPF(vfslide1up, NIL, F);
         return True;
      /*
       * Vector Floating-Point Slide1down Instructions
       */
      case 0b001111:
         GETC_VBinopOPF(vfslide1down, NIL, F);
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

   if (host_VersionV) {
      if (dis_RV64V0p7(dres, irsb, insn, guest_pc_curr_instr))
         return True;
      return False;
   }

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
