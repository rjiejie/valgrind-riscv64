
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

#define VInsn(insn, op) #insn"."#op
#define VInsnVV(insn)   VInsn(insn, vv)
#define VInsnVX(insn)   VInsn(insn, vx)
#define VInsnVI         VInsnVX
#define VInsnVS(insn)   VInsn(insn, vs)
#define VInsnWV(insn)   VInsn(insn, wv)
#define VInsnWX(insn)   VInsn(insn, wx)

#define VInsnVVM(insn)  VInsn(insn, vvm)
#define VInsnVXM(insn)  VInsn(insn, vxm)
#define VInsnVIM        VInsnVXM

#define VInsnMM(insn)   VInsn(insn, mm)
#define VInsnM(insn)    VInsn(insn, m)
#define VInsnV(insn)    VInsn(insn, v)
#define VInsnX(insn)    VInsn(insn, x)
#define VInsnI          VInsnX

#define VInsnVF(insn)   VInsn(insn, vf)
#define VInsnWF(insn)   VInsn(insn, wf)
#define VInsnF(insn)    VInsn(insn, f)

/*---------------------------------------------------------------*/
/*--- Get Name/Address of helper functions                    ---*/
/*---------------------------------------------------------------*/

#define GETN_VOp(insn, type, op, mask) "RVV_"#type"_"#insn#op#mask
#define GETA_VOp(insn, type, op, mask) RVV_##type##_##insn##op##mask

/* Binop */
#define GETN_VBinopVV_M(insn)  GETN_VOp(insn, Binop, vv, _m)
#define GETA_VBinopVV_M(insn)  GETA_VOp(insn, Binop, vv, _m)
#define GETN_VBinopVV(insn)    GETN_VOp(insn, Binop, vv,)
#define GETA_VBinopVV(insn)    GETA_VOp(insn, Binop, vv,)
#define GETN_VBinopVS_M        GETN_VBinopVV_M
#define GETA_VBinopVS_M        GETA_VBinopVV_M
#define GETN_VBinopVS          GETN_VBinopVV
#define GETA_VBinopVS          GETA_VBinopVV
#define GETN_VBinopWV_M        GETN_VBinopVV_M
#define GETA_VBinopWV_M        GETA_VBinopVV_M
#define GETN_VBinopWV          GETN_VBinopVV
#define GETA_VBinopWV          GETA_VBinopVV
#define GETN_VBinopVVM_M       GETN_VBinopVV_M
#define GETA_VBinopVVM_M       GETA_VBinopVV_M
#define GETN_VBinopVVM         GETN_VBinopVV
#define GETA_VBinopVVM         GETA_VBinopVV
#define GETN_VBinopMM_M        GETN_VBinopVV_M
#define GETA_VBinopMM_M        GETA_VBinopVV_M
#define GETN_VBinopMM          GETN_VBinopVV
#define GETA_VBinopMM          GETA_VBinopVV

#define GETN_VBinopVX_M(insn)  GETN_VOp(insn, Binop, vx, _m)
#define GETA_VBinopVX_M(insn)  GETA_VOp(insn, Binop, vx, _m)
#define GETN_VBinopVX(insn)    GETN_VOp(insn, Binop, vx,)
#define GETA_VBinopVX(insn)    GETA_VOp(insn, Binop, vx,)
#define GETN_VBinopWX_M        GETN_VBinopVX_M
#define GETA_VBinopWX_M        GETA_VBinopVX_M
#define GETN_VBinopWX          GETN_VBinopVX
#define GETA_VBinopWX          GETA_VBinopVX
#define GETN_VBinopVXM_M       GETN_VBinopVX_M
#define GETA_VBinopVXM_M       GETA_VBinopVX_M
#define GETN_VBinopVXM         GETN_VBinopVX
#define GETA_VBinopVXM         GETA_VBinopVX

#define GETN_VBinopVI_M(insn)  GETN_VOp(insn, Binop, vi, _m)
#define GETA_VBinopVI_M(insn)  GETA_VOp(insn, Binop, vi, _m)
#define GETN_VBinopVI(insn)    GETN_VOp(insn, Binop, vi,)
#define GETA_VBinopVI(insn)    GETA_VOp(insn, Binop, vi,)
#define GETN_VBinopVIM_M       GETN_VBinopVI_M
#define GETA_VBinopVIM_M       GETA_VBinopVI_M
#define GETN_VBinopVIM         GETN_VBinopVI
#define GETA_VBinopVIM         GETA_VBinopVI

#define GETN_VBinopVF_M(insn)  GETN_VOp(insn, Binop, vf, _m)
#define GETA_VBinopVF_M(insn)  GETA_VOp(insn, Binop, vf, _m)
#define GETN_VBinopVF(insn)    GETN_VOp(insn, Binop, vf,)
#define GETA_VBinopVF(insn)    GETA_VOp(insn, Binop, vf,)
#define GETN_VBinopWF_M        GETN_VBinopVF_M
#define GETA_VBinopWF_M        GETA_VBinopVF_M
#define GETN_VBinopWF          GETN_VBinopVF
#define GETA_VBinopWF          GETA_VBinopVF

#define GETN_VBinopVNIL_M(insn)  NULL
#define GETA_VBinopVNIL_M(insn)  NULL
#define GETN_VBinopVNIL(insn)    NULL
#define GETA_VBinopVNIL(insn)    NULL

/* Unop */
#define GETN_VUnopV_M(insn)    GETN_VOp(insn, Unop, v, _m)
#define GETA_VUnopV_M(insn)    GETA_VOp(insn, Unop, v, _m)
#define GETN_VUnopV(insn)      GETN_VOp(insn, Unop, v,)
#define GETA_VUnopV(insn)      GETA_VOp(insn, Unop, v,)
#define GETN_VUnopM_M          GETN_VUnopV_M
#define GETA_VUnopM_M          GETA_VUnopV_M
#define GETN_VUnopM            GETN_VUnopV
#define GETA_VUnopM            GETA_VUnopV

#define GETN_VUnopX_M(insn)    GETN_VOp(insn, Unop, x, _m)
#define GETA_VUnopX_M(insn)    GETA_VOp(insn, Unop, x, _m)
#define GETN_VUnopX(insn)      GETN_VOp(insn, Unop, x,)
#define GETA_VUnopX(insn)      GETA_VOp(insn, Unop, x,)

#define GETN_VUnopI_M(insn)    GETN_VOp(insn, Unop, i, _m)
#define GETA_VUnopI_M(insn)    GETA_VOp(insn, Unop, i, _m)
#define GETN_VUnopI(insn)      GETN_VOp(insn, Unop, i,)
#define GETA_VUnopI(insn)      GETA_VOp(insn, Unop, i,)

#define GETN_VUnopF_M(insn)    GETN_VOp(insn, Unop, f, _m)
#define GETA_VUnopF_M(insn)    GETA_VOp(insn, Unop, f, _m)
#define GETN_VUnopF(insn)      GETN_VOp(insn, Unop, f,)
#define GETA_VUnopF(insn)      GETA_VOp(insn, Unop, f,)

#define GETN_VUnopNIL_M(insn)  NULL
#define GETA_VUnopNIL_M(insn)  NULL
#define GETN_VUnopNIL(insn)    NULL
#define GETA_VUnopNIL(insn)    NULL

/*---------------------------------------------------------------*/
/*--- Get variant type of helper functions                    ---*/
/*---------------------------------------------------------------*/

#define GETV_VopUnknow  0
#define GETV_VopAccD    (1 << 0) /* Indicate VD  is readed   by OP, e.g. VFMACC  */
#define GETV_VopWidenD  (1 << 1) /* Indicate VD  is widened  by OP, e.g. VFWADD  */
#define GETV_VopWidenS2 (1 << 2) /* Indicate VS2 is widened  by OP, e.g. VFWADDW/VFNCVT */
#define GETV_VopM1D     (1 << 3) /* Indicate VD  is 1 LMUL   by OP, e.g. VMFEQ   */
#define GETV_VopM1S     (1 << 4) /* Indicate VS* is 1 LMUL   by OP, e.g. VMAND   */
#define GETV_VopMask    (1 << 5) /* Indicate V0  is readed   by OP, e.g. VADC    */

/*---------------------------------------------------------------*/
/*--- Get call of helper functions                            ---*/
/*---------------------------------------------------------------*/

#define RVV_REGO offsetIReg64
#define RVV_REGN nameIReg

/* Binop */
#define GETC_VBinopOP_T(insn, V, X, I, ARGS, VARIANT)                          \
   do {                                                                        \
      if (isVOpVV(GET_FUNCT3())) {                                             \
         fName = mask ? GETN_VBinopV##V(insn) : GETN_VBinopV##V##_M(insn);     \
         fAddr = mask ? GETA_VBinopV##V(insn) : GETA_VBinopV##V##_M(insn);     \
         temp  = offsetVReg(rs1);                                              \
      } else if (isVOpVXorVF(GET_FUNCT3())) {                                  \
         fName = mask ? GETN_VBinopV##X(insn) : GETN_VBinopV##X##_M(insn);     \
         fAddr = mask ? GETA_VBinopV##X(insn) : GETA_VBinopV##X##_M(insn);     \
         temp  = RVV_REGO(rs1);                                                \
      } else {                                                                 \
         fName = mask ? GETN_VBinopV##I(insn) : GETN_VBinopV##I##_M(insn);     \
         fAddr = mask ? GETA_VBinopV##I(insn) : GETA_VBinopV##I##_M(insn);     \
         temp  = ((Long) rs1 << 59) >> 59;                                     \
      }                                                                        \
      vassert(fAddr != NULL);                                                  \
      ARGS()                                                                   \
      d = unsafeIRDirty_1_N(ret, 0, fName, fAddr, args);                       \
      d = GETD_VBinop(d, rd, rs2, rs1, mask, GET_FUNCT3(), VARIANT);           \
      stmt(irsb, IRStmt_Dirty(d));                                             \
                                                                               \
      if (isVOpVI(GET_FUNCT3()))                                               \
         DIP("%s(%s, %s, %u)\n", fName, nameVReg(rd), nameVReg(rs2), rs1);     \
      else                                                                     \
         DIP("%s(%s, %s, %s)\n", fName, nameVReg(rd), nameVReg(rs2),           \
             isVOpVV(GET_FUNCT3()) ? nameVReg(rs1) : RVV_REGN(rs1));           \
   } while (0)

// OPI
#define GETR_VBinopOPI()                                                       \
   args = mkIRExprVec_5(IRExpr_GSPTR(), mkU64(offsetVReg(rd)),                 \
                        mkU64(offsetVReg(rs2)), mkU64(temp),                   \
                        mkU64(offsetVReg(0)));

#define GETC_VBinopOPI_VAR(insn, v, x, i, var)                                 \
   GETC_VBinopOP_T(insn, v, x, i, GETR_VBinopOPI, var)
#define GETC_VBinopOPI(insn, v, x, i)                                          \
   GETC_VBinopOPI_VAR(insn, v, x, i, GETV_VopUnknow)

// SAT
#define GETR_VBinopSAT()                                                       \
   assign(irsb, xrm,                                                           \
          binop(Iop_And32, binop(Iop_Shr32, getFCSR(), mkU8(9)), mkU32(3)));   \
   args = mkIRExprVec_6(IRExpr_GSPTR(), mkU64(offsetVReg(rd)),                 \
                        mkU64(offsetVReg(rs2)), mkU64(temp),                   \
                        mkU64(offsetVReg(0)), mkexpr(xrm));

#define GETC_VBinopSAT_VAR(insn, v, x, i, var)                                 \
   GETC_VBinopOP_T(insn, v, x, i, GETR_VBinopSAT, var);                        \
   putVxsat0p7(irsb, mkexpr(ret));
#define GETC_VBinopSAT(insn, v, x, i)                                          \
   GETC_VBinopSAT_VAR(insn, v, x, i, GETV_VopUnknow)

// OPF
#define GETR_VBinopOPF()                                                       \
   assign(irsb, frm,                                                           \
          binop(Iop_And32, binop(Iop_Shr32, getFCSR(), mkU8(5)), mkU32(7)));   \
   args = mkIRExprVec_6(IRExpr_GSPTR(), mkU64(offsetVReg(rd)),                 \
                        mkU64(offsetVReg(rs2)), mkU64(temp),                   \
                        mkU64(offsetVReg(0)), mkexpr(frm));

#define GETC_VBinopOPF_VAR(insn, v, x, var)                                    \
   GETC_VBinopOP_T(insn, v, x, NIL, GETR_VBinopOPF, var);                      \
   accumulateFFLAGS(irsb, mkexpr(ret));
#define GETC_VBinopOPF(insn, v, x)                                             \
   GETC_VBinopOPF_VAR(insn, v, x, GETV_VopUnknow)

/* Unop */
#define GETC_VUnopOP_T(insn, V, X, I, ARGS, VARIANT)                           \
   do {                                                                        \
      if (isVOpVV(GET_FUNCT3())) {                                             \
         fName = mask ? GETN_VUnop##V(insn) : GETN_VUnop##V##_M(insn);         \
         fAddr = mask ? GETA_VUnop##V(insn) : GETA_VUnop##V##_M(insn);         \
         temp  = offsetVReg(rs2);                                              \
      } else if (isVOpVXorVF(GET_FUNCT3())) {                                  \
         fName = mask ? GETN_VUnop##X(insn) : GETN_VUnop##X##_M(insn);         \
         fAddr = mask ? GETA_VUnop##X(insn) : GETA_VUnop##X##_M(insn);         \
         temp  = RVV_REGO(rs1);                                                \
      } else {                                                                 \
         fName = mask ? GETN_VUnop##I(insn) : GETN_VUnop##I##_M(insn);         \
         fAddr = mask ? GETA_VUnop##I(insn) : GETA_VUnop##I##_M(insn);         \
         temp  = ((Long) rs1 << 59) >> 59;                                     \
      }                                                                        \
      vassert(fAddr != NULL);                                                  \
      ARGS()                                                                   \
      d = unsafeIRDirty_1_N(ret, 0, fName, fAddr, args);                       \
      d = GETD_VUnop(d, rd, isVOpVV(GET_FUNCT3()) ? rs2 : rs1, mask,           \
                     GET_FUNCT3(), VARIANT);                                   \
      stmt(irsb, IRStmt_Dirty(d));                                             \
                                                                               \
      if (isVOpVI(GET_FUNCT3()))                                               \
         DIP("%s(%s, %u)\n", fName, nameVReg(rd), rs1);                        \
      else                                                                     \
         DIP("%s(%s, %s)\n", fName, nameVReg(rd),                              \
             isVOpVV(GET_FUNCT3()) ? nameVReg(rs2) : RVV_REGN(rs1));           \
   } while (0)

// OPI
#define GETR_VUnopOPI()                                                        \
   args = mkIRExprVec_5(IRExpr_GSPTR(), mkU64(offsetVReg(rd)), mkU64(temp),    \
                        mkU64(0), mkU64(offsetVReg(0)));

#define GETC_VUnopOPI_VAR(insn, v, x, i, var)                                 \
   GETC_VUnopOP_T(insn, v, x, i, GETR_VUnopOPI, var)
#define GETC_VUnopOPI(insn, v, x, i)                                          \
   GETC_VUnopOPI_VAR(insn, v, x, i, GETV_VopUnknow)

// OPF
#define GETR_VUnopOPF()                                                        \
   assign(irsb, frm,                                                           \
          binop(Iop_And32, binop(Iop_Shr32, getFCSR(), mkU8(5)), mkU32(7)));   \
   args = mkIRExprVec_5(IRExpr_GSPTR(), mkU64(offsetVReg(rd)), mkU64(temp),    \
                        mkU64(offsetVReg(0)), mkexpr(frm));

#define GETC_VUnopOPF_VAR(insn, v, x, var)                                    \
   GETC_VUnopOP_T(insn, v, x, NIL, GETR_VUnopOPF, var);                       \
   accumulateFFLAGS(irsb, mkexpr(ret));
#define GETC_VUnopOPF(insn, v, x)                                             \
   GETC_VUnopOPF_VAR(insn, v, x, GETV_VopUnknow)

/*---------------------------------------------------------------*/
/*--- Get dirty info of helper functions                      ---*/
/*---------------------------------------------------------------*/

/* Binop */
static IRDirty*
GETD_VBinop(IRDirty* d, UInt vd, UInt vs2, UInt vs1, Bool mask, UInt sopc, UInt vtype)
{
   UInt lmul   = extract_lmul_0p7(guest_VFLAG);
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
GETD_VUnop(IRDirty* d, UInt vd, UInt src, Bool mask, UInt sopc, UInt vtype)
{
   UInt lmul   = extract_lmul_0p7(guest_VFLAG);
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

/*---------------------------------------------------------------*/
/*--- Special instruction templates                           ---*/
/*---------------------------------------------------------------*/

#define RVV_Config()                                                           \
   UShort vstart = st->guest_vstart;                                           \
   do {                                                                        \
      UShort vl    = st->guest_vl;                                             \
      UShort vtype = st->guest_vtype;                                          \
      __asm__ __volatile__("vsetvl\tx0,%0,%1\n\t" ::"r"(vl), "r"(vtype) :);    \
   } while (0)

#define RVV_ConfigVstart()                                                     \
   do {                                                                        \
      if (vstart)                                                              \
         __asm__ __volatile__("csrw\tvstart,%0\n\t" ::"r"(vstart) :);          \
   } while (0)

// Push vl/vtype
// Set whole register du to current vtype
#define RVV_PushCfg()         \
   __asm__ __volatile__(      \
      "csrr\tt1,vl\n\t"       \
      "csrr\tt2,vtype\n\t"    \
      "vsetvl\tx0,x0,t2\n\t"  \
      ::: "t1", "t2");

// Pop vl/vtype
#define RVV_PopCfg()          \
   __asm__ __volatile__(      \
      "vsetvl\tx0,t1,t2\n\t"  \
      :::);

// Push vl/vtype
// Set whole register with widened LMUL of current vtype
#define RVV_PushWCfg()        \
   __asm__ __volatile__(      \
      "csrr\tt1,vl\n\t"       \
      "csrr\tt2,vtype\n\t"    \
      "addi\tt0,t2,1\n\t"     \
      "vsetvl\tx0,x0,t0\n\t"  \
      ::: "t0", "t1", "t2");

// Get mask
#define RVV_Mask()                  \
   __asm__ __volatile__(            \
      "csrr\tt1,vl\n\t"             \
      "csrr\tt2,vtype\n\t"          \
      "vsetvli\tx0,x0,e8,m1\n\t"    \
      "vle.v\tv0,(%0)\n\t"          \
      "vsetvl\tx0,t1,t2\n\t"        \
      :                             \
      : "r"(mask)                   \
      : "t1", "t2");

// Get X/I/F register
#define RVV_VX() \
   vs1 += (ULong)st;\
   __asm__ __volatile__("ld\tt0,0(%0)\n\t"::"r"(vs1):"t0");
#define RVV_VI() \
   __asm__ __volatile__("mv\tt0,%0\n\t"::"r"(vs1):"t0");
#define RVV_VF() \
   vs1 += (ULong)st;\
   __asm__ __volatile__("fld\tft0,0(%0)\n\t"::"r"(vs1):"ft0");

// Push fcsr
// Set frm
#define RVV_PushFCSR()              \
   __asm__ __volatile__(            \
      "csrr\tt1,fcsr\n\t"           \
      "csrw\tfrm,%0\n\t"            \
      "csrw\tfflags,x0\n\t"         \
      :                             \
      : "r"(frm)                    \
      : "t1");
// Get fflags
// Pop fcsr
#define RVV_PopFCSR()               \
   __asm__ __volatile__(            \
      "csrr\t%0,fflags\n\t"         \
      "csrw\tfcsr,t1\n\t"           \
      : "=r"(ret)                   \
      :                             \
      :);

// Push fcsr
// Set xrm
#define RVV_PushXSAT()              \
   __asm__ __volatile__(            \
      "csrr\tt1,fcsr\n\t"           \
      "csrw\tvxrm,%0\n\t"           \
      "csrw\tvxsat,x0\n\t"          \
      :                             \
      : "r"(xrm)                    \
      : "t1");
// Get xsat
// Pop fcsr
#define RVV_PopXSAT()               \
   __asm__ __volatile__(            \
      "csrr\t%0,vxsat\n\t"          \
      "csrw\tfcsr,t1\n\t"           \
      : "=r"(ret)                   \
      :                             \
      :);

#define RVV_Push()
#define RVV_Pop()
#define RVV_Pre()
#define RVV_Post()

/*---------------------------------------------------------------*/
/*--- VV Instruction templates                                ---*/
/*---------------------------------------------------------------*/

// v8-v15, v16-v23, v24-v31
#define RVV_BinopVV_Tpl(insn, vd, vs2, vs1, imask, mreg)    \
   do {                                                     \
      RVV_Config();                                         \
      UInt ret = 0;                                         \
                                                            \
      vd  += (ULong)st;                                     \
      vs2 += (ULong)st;                                     \
      vs1 += (ULong)st;                                     \
      mask += (ULong)st;                                    \
                                                            \
      RVV_Push()                                            \
      __asm__ __volatile__(                                 \
         "vle.v\tv8,(%0)\n\t"                               \
         "vle.v\tv16,(%1)\n\t"                              \
         "vle.v\tv24,(%2)\n\t"                              \
         :                                                  \
         : "r"(vd), "r"(vs2), "r"(vs1)                      \
         :);                                                \
      RVV_Pop()                                             \
                                                            \
      imask                                                 \
                                                            \
      RVV_Pre()                                             \
      __asm__ __volatile__(insn "\tv8,v16,v24" mreg);       \
      RVV_Post()                                            \
                                                            \
      RVV_Push()                                            \
      RVV_ConfigVstart();                                   \
      __asm__ __volatile__(                                 \
         "vse.v\tv8,(%0)\n\t"                               \
         :                                                  \
         : "r"(vd)                                          \
         : "memory");                                       \
      RVV_Pop()                                             \
                                                            \
      return ret;                                           \
   } while (0)

// v8-v15, v16-v23
#define RVV_UnopV_Tpl(insn, vd, vs, imask, mreg)            \
   do {                                                     \
      RVV_Config();                                         \
      UInt ret = 0;                                         \
                                                            \
      vd  += (ULong)st;                                     \
      vs  += (ULong)st;                                     \
      mask += (ULong)st;                                    \
                                                            \
      RVV_Push()                                            \
      __asm__ __volatile__(                                 \
         "vle.v\tv8,(%0)\n\t"                               \
         "vle.v\tv16,(%1)\n\t"                              \
         :                                                  \
         : "r"(vd), "r"(vs)                                 \
         :);                                                \
      RVV_Pop()                                             \
                                                            \
      imask                                                 \
                                                            \
      RVV_Pre()                                             \
      __asm__ __volatile__(insn "\tv8,v16" mreg);           \
      RVV_Post()                                            \
                                                            \
      RVV_Push()                                            \
      RVV_ConfigVstart();                                   \
      __asm__ __volatile__(                                 \
         "vse.v\tv8,(%0)\n\t"                               \
         :                                                  \
         : "r"(vd)                                          \
         : "memory");                                       \
      RVV_Pop()                                             \
                                                            \
      return ret;                                           \
   } while (0)

/*---------------------------------------------------------------*/
/*--- OPIVV/OPFVV Instruction templates helper                ---*/
/*---------------------------------------------------------------*/

/* Binop */
#define RVV_BinopVV(insn, vd, vs2, vs1)    RVV_BinopVV_Tpl(insn, vd, vs2, vs1, ,)
#define RVV_BinopVV_M(insn, vd, vs2, vs1)  RVV_BinopVV_Tpl(insn, vd, vs2, vs1, RVV_Mask(), ",v0.t")
#define RVV_BinopVV2(insn, vd, vs2, vs1)   RVV_BinopVV_Tpl(insn, vd, vs1, vs2, ,)
#define RVV_BinopVV2_M(insn, vd, vs2, vs1) RVV_BinopVV_Tpl(insn, vd, vs1, vs2, RVV_Mask(), ",v0.t")

#define RVV_BinopVVM(insn, vd, vs2, vs1)   RVV_BinopVV_Tpl(insn, vd, vs2, vs1, RVV_Mask(), ",v0")
#define RVV_BinopVVM_M  RVV_BinopVVM

/* Unop */
#define RVV_UnopV(insn, vd, vs2, nil)    RVV_UnopV_Tpl(insn, vd, vs2, ,)
#define RVV_UnopV_M(insn, vd, vs2, nil)  RVV_UnopV_Tpl(insn, vd, vs2, RVV_Mask(), ",v0.t")

#define RVV_UnopV3(insn, vd, vs2, nil)    RVV_UnopV_Tpl3(insn, vd, vs2, nil, ,)
#define RVV_UnopV3_M(insn, vd, vs2, nil)  RVV_UnopV_Tpl3(insn, vd, vs2, nil, RVV_Mask(), ",v0.t")

/*---------------------------------------------------------------*/
/*--- VX/VI/VF Instruction templates                          ---*/
/*---------------------------------------------------------------*/

// v8-v15, v16-v23, t0/ft0
#define RVV_BinopXIF_iTpl(insn, treg, mreg)\
   __asm__ __volatile__(insn "\tv8,v16," treg mreg);
// v8-v15, t0/ft0, v16-v23
#define RVV_BinopXIF_iTpl2(insn, treg, mreg)\
   __asm__ __volatile__(insn "\tv8," treg ",v16" mreg);
// ret, v16-v23, t0/ft0
#define RVV_BinopXIF_iTpl3(insn, treg, mreg)\
   __asm__ __volatile__(insn "\t%0,v16" treg mreg :"=r"(ret)::);

#define RVV_BinopXIF_LD \
   __asm__ __volatile__(                                    \
      "vle.v\tv8,(%0)\n\t"                                  \
      "vle.v\tv16,(%1)\n\t"                                 \
      :                                                     \
      : "r"(vd), "r"(vs2)                                   \
      :);

#define RVV_BinopXIF_ST \
   __asm__ __volatile__("vse.v\tv8,(%0)\n\t" : : "r"(vd) : "memory");

#define RVV_BinopXIF_Generic(insn, vd, vs2, vs1, imask, mreg, sopc, treg, itpl)\
   do {                                                     \
      RVV_Config();                                         \
      UInt ret = 0;                                         \
                                                            \
      vd  += (ULong)st;                                     \
      vs2 += (ULong)st;                                     \
      mask += (ULong)st;                                    \
                                                            \
      RVV_Push()                                            \
      RVV_BinopXIF_LD                                       \
      RVV_Pop()                                             \
                                                            \
      imask                                                 \
                                                            \
      RVV_Pre()                                             \
      sopc                                                  \
      itpl(insn, treg, mreg)                                \
      RVV_Post()                                            \
                                                            \
      RVV_Push()                                            \
      RVV_ConfigVstart();                                   \
      RVV_BinopXIF_ST                                       \
      RVV_Pop()                                             \
                                                            \
      return ret;                                           \
   } while (0)

#define RVV_BinopXIF_Tpl(insn, vd, vs2, vs1, imask, mreg, sopc, treg)\
   RVV_BinopXIF_Generic(insn, vd, vs2, vs1, imask, mreg, sopc, treg, RVV_BinopXIF_iTpl)
#define RVV_BinopVX_Tpl(insn, vd, vs2, vs1, imask, mreg)\
   RVV_BinopXIF_Tpl(insn, vd, vs2, vs1, imask, mreg, RVV_VX(), "t0")
#define RVV_BinopVI_Tpl(insn, vd, vs2, vs1, imask, mreg)\
   RVV_BinopXIF_Tpl(insn, vd, vs2, vs1, imask, mreg, RVV_VI(), "t0")
#define RVV_BinopVF_Tpl(insn, vd, vs2, vs1, imask, mreg)\
   RVV_BinopXIF_Tpl(insn, vd, vs2, vs1, imask, mreg, RVV_VF(), "ft0")

#define RVV_BinopXIF_Tpl2(insn, vd, vs2, vs1, imask, mreg, sopc, treg)\
   RVV_BinopXIF_Generic(insn, vd, vs2, vs1, imask, mreg, sopc, treg, RVV_BinopXIF_iTpl2)
#define RVV_BinopVX_Tpl2(insn, vd, vs2, vs1, imask, mreg)\
   RVV_BinopXIF_Tpl2(insn, vd, vs2, vs1, imask, mreg, RVV_VX(), "t0")
#define RVV_BinopVF_Tpl2(insn, vd, vs2, vs1, imask, mreg)\
   RVV_BinopXIF_Tpl2(insn, vd, vs2, vs1, imask, mreg, RVV_VF(), "ft0")

#define RVV_BinopXIF_Tpl3(insn, vd, vs2, vs1, imask, mreg, sopc, treg)\
   RVV_BinopXIF_Generic(insn, vd, vs2, vs1, imask, mreg, sopc, treg, RVV_BinopXIF_iTpl3)
#define RVV_BinopVX_Tpl3(insn, vd, vs2, vs1, imask, mreg)\
   RVV_BinopXIF_Tpl3(insn, vd, vs2, vs1, imask, mreg, RVV_VX(), ",t0")
#define RVV_UnopV_Tpl3(insn, vd, vs2, vs1, imask, mreg)\
   RVV_BinopXIF_Tpl3(insn, vd, vs2, vs1, imask, mreg, , )

// v8-v15, t0/ft0
#define RVV_UnopXIF_Tpl(insn, vd, vs1, imask, mreg, sopc, treg)\
   do {                                                     \
      RVV_Config();                                         \
      UInt ret = 0;                                         \
                                                            \
      vd  += (ULong)st;                                     \
      mask += (ULong)st;                                    \
                                                            \
      RVV_Push()                                            \
      __asm__ __volatile__(                                 \
         "vle.v\tv8,(%0)\n\t"                               \
         :                                                  \
         : "r"(vd)                                          \
         :);                                                \
      RVV_Pop()                                             \
                                                            \
      imask                                                 \
                                                            \
      RVV_Pre()                                             \
      sopc                                                  \
      __asm__ __volatile__(insn "\tv8," treg mreg);         \
      RVV_Post()                                            \
                                                            \
      RVV_Push()                                            \
      RVV_ConfigVstart();                                   \
      __asm__ __volatile__(                                 \
         "vse.v\tv8,(%0)\n\t"                               \
         :                                                  \
         : "r"(vd)                                          \
         : "memory");                                       \
      RVV_Pop()                                             \
                                                            \
      return ret;                                           \
   } while (0)

#define RVV_UnopX_Tpl(insn, vd, vs1, imask, mreg)\
   RVV_UnopXIF_Tpl(insn, vd, vs1, imask, mreg, RVV_VX(), "t0")
#define RVV_UnopI_Tpl(insn, vd, vs1, imask, mreg)\
   RVV_UnopXIF_Tpl(insn, vd, vs1, imask, mreg, RVV_VI(), "t0")
#define RVV_UnopF_Tpl(insn, vd, vs1, imask, mreg)\
   RVV_UnopXIF_Tpl(insn, vd, vs1, imask, mreg, RVV_VF(), "ft0")

/*---------------------------------------------------------------*/
/*--- VX/VI/VF Instruction templates helper                   ---*/
/*---------------------------------------------------------------*/

/* Binop */
// OPI
#define RVV_BinopVX(insn, vd, vs2, vs1)    RVV_BinopVX_Tpl(insn, vd, vs2, vs1, ,)
#define RVV_BinopVX_M(insn, vd, vs2, vs1)  RVV_BinopVX_Tpl(insn, vd, vs2, vs1, RVV_Mask(), ",v0.t")
#define RVV_BinopVI(insn, vd, vs2, vs1)    RVV_BinopVI_Tpl(insn, vd, vs2, vs1, ,)
#define RVV_BinopVI_M(insn, vd, vs2, vs1)  RVV_BinopVI_Tpl(insn, vd, vs2, vs1, RVV_Mask(), ",v0.t")
#define RVV_BinopVX2(insn, vd, vs2, vs1)   RVV_BinopVX_Tpl2(insn, vd, vs2, vs1, ,)
#define RVV_BinopVX2_M(insn, vd, vs2, vs1) RVV_BinopVX_Tpl2(insn, vd, vs2, vs1, RVV_Mask(), ",v0.t")

#define RVV_BinopVXM(insn, vd, vs2, vs1)   RVV_BinopVX_Tpl(insn, vd, vs2, vs1, RVV_Mask(), ",v0")
#define RVV_BinopVXM_M  RVV_BinopVXM
#define RVV_BinopVIM(insn, vd, vs2, vs1)   RVV_BinopVI_Tpl(insn, vd, vs2, vs1, RVV_Mask(), ",v0")
#define RVV_BinopVIM_M  RVV_BinopVIM

// OPF
#define RVV_BinopVF(insn, vd, vs2, vs1)    RVV_BinopVF_Tpl(insn, vd, vs2, vs1, ,)
#define RVV_BinopVF_M(insn, vd, vs2, vs1)  RVV_BinopVF_Tpl(insn, vd, vs2, vs1, RVV_Mask(), ",v0.t")
#define RVV_BinopVF2(insn, vd, vs2, vs1)   RVV_BinopVF_Tpl2(insn, vd, vs2, vs1, ,)
#define RVV_BinopVF2_M(insn, vd, vs2, vs1) RVV_BinopVF_Tpl2(insn, vd, vs2, vs1, RVV_Mask(), ",v0.t")

#define RVV_BinopVFM(insn, vd, vs2, vs1)   RVV_BinopVF_Tpl(insn, vd, vs2, vs1, RVV_Mask(), ",v0")
#define RVV_BinopVFM_M  RVV_BinopVFM

/* Unop */
// OPI
#define RVV_UnopX(insn, vd, vs1, nil)      RVV_UnopX_Tpl(insn, vd, vs1, ,)
#define RVV_UnopX_M                        RVV_UnopX
#define RVV_UnopI(insn, vd, vs1, nil)      RVV_UnopI_Tpl(insn, vd, vs1, ,)
#define RVV_UnopI_M                        RVV_UnopI

// OPF
#define RVV_UnopF(insn, vd, vs1, nil)      RVV_UnopF_Tpl(insn, vd, vs1, ,)
#define RVV_UnopF_M                        RVV_UnopF

/*---------------------------------------------------------------*/
/*--- OPI function templates                                  ---*/
/*---------------------------------------------------------------*/

#define RVV_NIL_FT(insn)

#define RVV_FT_DEF(insn, vv, vx, vi)   \
   RVV_##vv##_FT(insn)                 \
   RVV_##vx##_FT(insn)                 \
   RVV_##vi##_FT(insn)

#define RVV_FT_VAR_T(type, name, insn, op, body, args...)                      \
   static UInt GETA_V##type##op##_M(name)(VexGuestRISCV64State * st, ##args)   \
   {                                                                           \
      RVV_##type##body##_M(VInsn##op(insn), vd, vs2, vs1);                     \
   }                                                                           \
   static UInt GETA_V##type##op(name)(VexGuestRISCV64State * st, ##args)       \
   {                                                                           \
      RVV_##type##body(VInsn##op(insn), vd, vs2, vs1);                         \
   }

#define RVV_BinopOPI_FT_VAR_T(name, insn, op, body)                            \
   RVV_FT_VAR_T(Binop, name, insn, op, body,                                   \
                ULong vd, ULong vs2, ULong vs1, ULong mask)
#define RVV_BinopOPI_FT_VAR(insn, op, body)  RVV_BinopOPI_FT_VAR_T(insn, insn, op, body)

#define RVV_BinopSAT_FT_VAR(insn, op, body)                                    \
   RVV_FT_VAR_T(Binop, insn, insn, op, body,                                   \
                ULong vd, ULong vs2, ULong vs1, ULong mask, UInt xrm)

#define RVV_UnopOPI_FT_VAR_T(name, insn, op, body)                             \
   RVV_FT_VAR_T(Unop, name, insn, op, body,                                    \
                ULong vd, ULong vs2, ULong vs1, ULong mask)
#define RVV_UnopOPI_FT_VAR(insn, op, body) RVV_UnopOPI_FT_VAR_T(insn, insn, op, body)

/* Binop */
#define RVV_BinopOPIVV_FT(insn)     RVV_BinopOPI_FT_VAR(insn, VV, VV)
#define RVV_BinopOPIVX_FT(insn)     RVV_BinopOPI_FT_VAR(insn, VX, VX)
#define RVV_BinopOPIVI_FT(insn)     RVV_BinopOPI_FT_VAR(insn, VI, VI)
#define RVV_BinopOPIVV2_FT(insn)    RVV_BinopOPI_FT_VAR(insn, VV, VV2)
#define RVV_BinopOPIVX2_FT(insn)    RVV_BinopOPI_FT_VAR(insn, VX, VX2)
#define RVV_BinopOPIVS_FT(insn)     RVV_BinopOPI_FT_VAR(insn, VS, VV)

#define RVV_BinopOPIWV_FT(insn)     RVV_BinopOPI_FT_VAR_T(insn##w, insn, WV, VV)
#define RVV_BinopOPIWX_FT(insn)     RVV_BinopOPI_FT_VAR_T(insn##w, insn, WX, VX)

#define RVV_BinopOPIVVM_FT(insn)    RVV_BinopOPI_FT_VAR(insn, VVM, VVM)
#define RVV_BinopOPIVXM_FT(insn)    RVV_BinopOPI_FT_VAR(insn, VXM, VXM)
#define RVV_BinopOPIVIM_FT(insn)    RVV_BinopOPI_FT_VAR(insn, VIM, VIM)

#define RVV_BinopOPIMM_FT(insn)     RVV_BinopOPI_FT_VAR(insn, MM, VV)

#define RVV_BinopSATVV_FT(insn)     RVV_BinopSAT_FT_VAR(insn, VV, VV)
#define RVV_BinopSATVX_FT(insn)     RVV_BinopSAT_FT_VAR(insn, VX, VX)
#define RVV_BinopSATVI_FT(insn)     RVV_BinopSAT_FT_VAR(insn, VI, VI)
#define RVV_BinopSATVV2_FT(insn)    RVV_BinopSAT_FT_VAR(insn, VV, VV2)
#define RVV_BinopSATVX2_FT(insn)    RVV_BinopSAT_FT_VAR(insn, VX, VX2)

/* Unop */
#define RVV_UnopOPIM_FT(insn)       RVV_UnopOPI_FT_VAR(insn, M, V)

/*---------------------------------------------------------------*/
/*--- OPI function definitions                                ---*/
/*---------------------------------------------------------------*/

RVV_FT_DEF(vadd,   BinopOPIVV,  BinopOPIVX,  BinopOPIVI)
RVV_FT_DEF(vsub,   BinopOPIVV,  BinopOPIVX,  NIL)
RVV_FT_DEF(vrsub,  NIL,         BinopOPIVX,  BinopOPIVI)

#pragma push_macro("RVV_Push")
#pragma push_macro("RVV_Pop")
#undef  RVV_Push
#undef  RVV_Pop
#define RVV_Push   RVV_PushWCfg
#define RVV_Pop    RVV_PopCfg
RVV_FT_DEF(vwaddu, BinopOPIVV, BinopOPIVX, NIL)
RVV_FT_DEF(vwaddu, BinopOPIWV, BinopOPIWX, NIL)
RVV_FT_DEF(vwsubu, BinopOPIVV, BinopOPIVX, NIL)
RVV_FT_DEF(vwsubu, BinopOPIWV, BinopOPIWX, NIL)
RVV_FT_DEF(vwadd,  BinopOPIVV, BinopOPIVX, NIL)
RVV_FT_DEF(vwadd,  BinopOPIWV, BinopOPIWX, NIL)
RVV_FT_DEF(vwsub,  BinopOPIVV, BinopOPIVX, NIL)
RVV_FT_DEF(vwsub,  BinopOPIWV, BinopOPIWX, NIL)
#pragma pop_macro("RVV_Push")
#pragma pop_macro("RVV_Pop")

RVV_FT_DEF(vadc,   BinopOPIVVM, BinopOPIVXM, BinopOPIVIM)
RVV_FT_DEF(vmadc,  BinopOPIVVM, BinopOPIVXM, BinopOPIVIM)
RVV_FT_DEF(vsbc,   BinopOPIVVM, BinopOPIVXM, NIL)
RVV_FT_DEF(vmsbc,  BinopOPIVVM, BinopOPIVXM, NIL)

RVV_FT_DEF(vand,   BinopOPIVV,  BinopOPIVX,  BinopOPIVI)
RVV_FT_DEF(vor,    BinopOPIVV,  BinopOPIVX,  BinopOPIVI)
RVV_FT_DEF(vxor,   BinopOPIVV,  BinopOPIVX,  BinopOPIVI)
RVV_FT_DEF(vsll,   BinopOPIVV,  BinopOPIVX,  BinopOPIVI)
RVV_FT_DEF(vsrl,   BinopOPIVV,  BinopOPIVX,  BinopOPIVI)
RVV_FT_DEF(vsra,   BinopOPIVV,  BinopOPIVX,  BinopOPIVI)
#pragma push_macro("RVV_Push")
#pragma push_macro("RVV_Pop")
#undef  RVV_Push
#undef  RVV_Pop
#define RVV_Push   RVV_PushWCfg
#define RVV_Pop    RVV_PopCfg
RVV_FT_DEF(vnsrl,  BinopOPIVV, BinopOPIVX, BinopOPIVI)
RVV_FT_DEF(vnsra,  BinopOPIVV, BinopOPIVX, BinopOPIVI)
#pragma pop_macro("RVV_Push")
#pragma pop_macro("RVV_Pop")

#pragma push_macro("RVV_Push")
#pragma push_macro("RVV_Pop")
#undef  RVV_Push
#undef  RVV_Pop
#define RVV_Push   RVV_PushCfg
#define RVV_Pop    RVV_PopCfg
RVV_FT_DEF(vmseq,  BinopOPIVV,  BinopOPIVX,  BinopOPIVI)
RVV_FT_DEF(vmsne,  BinopOPIVV,  BinopOPIVX,  BinopOPIVI)
RVV_FT_DEF(vmsleu, BinopOPIVV,  BinopOPIVX,  BinopOPIVI)
RVV_FT_DEF(vmsle,  BinopOPIVV,  BinopOPIVX,  BinopOPIVI)
RVV_FT_DEF(vmsltu, BinopOPIVV,  BinopOPIVX,  NIL)
RVV_FT_DEF(vmslt,  BinopOPIVV,  BinopOPIVX,  NIL)
RVV_FT_DEF(vmsgtu, NIL,         BinopOPIVX,  BinopOPIVI)
RVV_FT_DEF(vmsgt,  NIL,         BinopOPIVX,  BinopOPIVI)
#pragma pop_macro("RVV_Push")
#pragma pop_macro("RVV_Pop")

RVV_FT_DEF(vminu,  BinopOPIVV,  BinopOPIVX,  NIL)
RVV_FT_DEF(vmin,   BinopOPIVV,  BinopOPIVX,  NIL)
RVV_FT_DEF(vmaxu,  BinopOPIVV,  BinopOPIVX,  NIL)
RVV_FT_DEF(vmax,   BinopOPIVV,  BinopOPIVX,  NIL)

RVV_FT_DEF(vmul,   BinopOPIVV,  BinopOPIVX,  NIL)
RVV_FT_DEF(vmulh,  BinopOPIVV,  BinopOPIVX,  NIL)
RVV_FT_DEF(vmulhu, BinopOPIVV,  BinopOPIVX,  NIL)
RVV_FT_DEF(vmulhsu,BinopOPIVV,  BinopOPIVX,  NIL)
RVV_FT_DEF(vdivu,  BinopOPIVV,  BinopOPIVX,  NIL)
RVV_FT_DEF(vdiv,   BinopOPIVV,  BinopOPIVX,  NIL)
RVV_FT_DEF(vremu,  BinopOPIVV,  BinopOPIVX,  NIL)
RVV_FT_DEF(vrem,   BinopOPIVV,  BinopOPIVX,  NIL)

#pragma push_macro("RVV_Push")
#pragma push_macro("RVV_Pop")
#undef  RVV_Push
#undef  RVV_Pop
#define RVV_Push   RVV_PushWCfg
#define RVV_Pop    RVV_PopCfg
RVV_FT_DEF(vwmul,  BinopOPIVV, BinopOPIVX, NIL)
RVV_FT_DEF(vwmulu, BinopOPIVV, BinopOPIVX, NIL)
RVV_FT_DEF(vwmulsu,BinopOPIVV, BinopOPIVX, NIL)
#pragma pop_macro("RVV_Push")
#pragma pop_macro("RVV_Pop")

RVV_FT_DEF(vmacc,  BinopOPIVV2, BinopOPIVX2, NIL)
RVV_FT_DEF(vnmsac, BinopOPIVV2, BinopOPIVX2, NIL)
RVV_FT_DEF(vmadd,  BinopOPIVV2, BinopOPIVX2, NIL)
RVV_FT_DEF(vnmsub, BinopOPIVV2, BinopOPIVX2, NIL)

#pragma push_macro("RVV_Push")
#pragma push_macro("RVV_Pop")
#undef  RVV_Push
#undef  RVV_Pop
#define RVV_Push     RVV_PushWCfg
#define RVV_Pop      RVV_PopCfg
RVV_FT_DEF(vwmaccu,  BinopOPIVV2, BinopOPIVX2, NIL)
RVV_FT_DEF(vwmacc,   BinopOPIVV2, BinopOPIVX2, NIL)
RVV_FT_DEF(vwmaccsu, BinopOPIVV2, BinopOPIVX2, NIL)
RVV_FT_DEF(vwmaccus, NIL,         BinopOPIVX2, NIL)
#pragma pop_macro("RVV_Push")
#pragma pop_macro("RVV_Pop")

RVV_FT_DEF(vmerge, BinopOPIVVM, BinopOPIVXM, BinopOPIVIM)

#pragma push_macro("RVV_Push")
#pragma push_macro("RVV_Pop")
#undef  RVV_Push
#undef  RVV_Pop
#define RVV_Push       RVV_PushCfg
#define RVV_Pop        RVV_PopCfg
RVV_FT_DEF(vslideup,   NIL,         BinopOPIVX, BinopOPIVI)
RVV_FT_DEF(vslidedown, NIL,         BinopOPIVX, BinopOPIVI)
RVV_FT_DEF(vslide1up,  NIL,         BinopOPIVX, NIL)
RVV_FT_DEF(vslide1down,NIL,         BinopOPIVX, NIL)
RVV_FT_DEF(vrgather,   BinopOPIVV,  BinopOPIVX, BinopOPIVI)
#pragma pop_macro("RVV_Push")
#pragma pop_macro("RVV_Pop")

RVV_FT_DEF(vredsum,   BinopOPIVS,   NIL, NIL)
RVV_FT_DEF(vredmaxu,  BinopOPIVS,   NIL, NIL)
RVV_FT_DEF(vredmax,   BinopOPIVS,   NIL, NIL)
RVV_FT_DEF(vredminu,  BinopOPIVS,   NIL, NIL)
RVV_FT_DEF(vredmin,   BinopOPIVS,   NIL, NIL)
RVV_FT_DEF(vredand,   BinopOPIVS,   NIL, NIL)
RVV_FT_DEF(vredor,    BinopOPIVS,   NIL, NIL)
RVV_FT_DEF(vredxor,   BinopOPIVS,   NIL, NIL)
#pragma push_macro("RVV_Push")
#pragma push_macro("RVV_Pop")
#undef  RVV_Push
#undef  RVV_Pop
#define RVV_Push      RVV_PushCfg
#define RVV_Pop       RVV_PopCfg
RVV_FT_DEF(vwredsumu, BinopOPIVS, NIL, NIL)
RVV_FT_DEF(vwredsum,  BinopOPIVS, NIL, NIL)
#pragma pop_macro("RVV_Push")
#pragma pop_macro("RVV_Pop")

#pragma push_macro("RVV_Pre")
#pragma push_macro("RVV_Post")
#undef  RVV_Pre
#undef  RVV_Post
#define RVV_Pre      RVV_PushXSAT
#define RVV_Post     RVV_PopXSAT

RVV_FT_DEF(vsaddu,  BinopSATVV,  BinopSATVX,  BinopSATVI)
RVV_FT_DEF(vsadd,   BinopSATVV,  BinopSATVX,  BinopSATVI)
RVV_FT_DEF(vssubu,  BinopSATVV,  BinopSATVX,  NIL)
RVV_FT_DEF(vssub,   BinopSATVV,  BinopSATVX,  NIL)
RVV_FT_DEF(vaadd,   BinopSATVV,  BinopSATVX,  BinopSATVI)
RVV_FT_DEF(vasub,   BinopSATVV,  BinopSATVX,  NIL)
RVV_FT_DEF(vsmul,   BinopSATVV,  BinopSATVX,  NIL)
RVV_FT_DEF(vssrl,   BinopSATVV,  BinopSATVX,  BinopSATVI)
RVV_FT_DEF(vssra,   BinopSATVV,  BinopSATVX,  BinopSATVI)
#pragma push_macro("RVV_Push")
#pragma push_macro("RVV_Pop")
#undef  RVV_Push
#undef  RVV_Pop
#define RVV_Push      RVV_PushWCfg
#define RVV_Pop       RVV_PopCfg
RVV_FT_DEF(vwsmaccu,  BinopSATVV2,  BinopSATVX2,  NIL)
RVV_FT_DEF(vwsmacc,   BinopSATVV2,  BinopSATVX2,  NIL)
RVV_FT_DEF(vwsmaccsu, BinopSATVV2,  BinopSATVX2,  NIL)
RVV_FT_DEF(vwsmaccus, NIL,          BinopSATVX2,  NIL)
RVV_FT_DEF(vnclipu,   BinopSATVV,   BinopSATVX,   BinopSATVI)
RVV_FT_DEF(vnclip,    BinopSATVV,   BinopSATVX,   BinopSATVI)
#pragma pop_macro("RVV_Push")
#pragma pop_macro("RVV_Pop")

#pragma pop_macro("RVV_Pre")
#pragma pop_macro("RVV_Post")

#pragma push_macro("RVV_BinopVV_M")
#undef  RVV_BinopVV_M
#define RVV_BinopVV_M RVV_BinopVV
RVV_FT_DEF(vmand,    BinopOPIMM, NIL, NIL)
RVV_FT_DEF(vmnand,   BinopOPIMM, NIL, NIL)
RVV_FT_DEF(vmandnot, BinopOPIMM, NIL, NIL)
RVV_FT_DEF(vmxor,    BinopOPIMM, NIL, NIL)
RVV_FT_DEF(vmor,     BinopOPIMM, NIL, NIL)
RVV_FT_DEF(vmnor,    BinopOPIMM, NIL, NIL)
RVV_FT_DEF(vmornot,  BinopOPIMM, NIL, NIL)
RVV_FT_DEF(vmxnor,   BinopOPIMM, NIL, NIL)
#pragma pop_macro("RVV_BinopVV_M")

#pragma push_macro("VInsnVV")
#pragma push_macro("RVV_BinopVV_M")
#undef  VInsnVV
#undef  RVV_BinopVV_M
#define VInsnVV(insn) #insn
#define RVV_BinopVV_M RVV_BinopVV
RVV_BinopOPI_FT_VAR_T(vcompress, vcompress.vm, VV, VV)
#pragma pop_macro("VInsnVV")
#pragma pop_macro("RVV_BinopVV_M")

#pragma push_macro("RVV_Push")
#pragma push_macro("RVV_Pop")
#undef  RVV_Push
#undef  RVV_Pop
#define RVV_Push     RVV_PushCfg
#define RVV_Pop      RVV_PopCfg
RVV_FT_DEF(vmsbf,    UnopOPIM, NIL, NIL)
RVV_FT_DEF(vmsif,    UnopOPIM, NIL, NIL)
RVV_FT_DEF(vmsof,    UnopOPIM, NIL, NIL)
RVV_FT_DEF(viota,    UnopOPIM, NIL, NIL)
#pragma pop_macro("RVV_Push")
#pragma pop_macro("RVV_Pop")

#pragma push_macro("RVV_UnopV_M")
#undef  RVV_UnopV_M
#define RVV_UnopV_M RVV_UnopV
RVV_UnopOPI_FT_VAR_T(vmv, vmv.v, V, V)
#pragma pop_macro("RVV_UnopV_M")
RVV_UnopOPI_FT_VAR_T(vmv, vmv.v, X, X)
RVV_UnopOPI_FT_VAR_T(vmv, vmv.v, I, I)

#pragma push_macro("RVV_Push")
#pragma push_macro("RVV_Pop")
#undef  RVV_Push
#undef  RVV_Pop
#define RVV_Push     RVV_PushCfg
#define RVV_Pop      RVV_PopCfg
RVV_UnopOPI_FT_VAR_T(vmvs, vmv.s, X, X)
#pragma pop_macro("RVV_Push")
#pragma pop_macro("RVV_Pop")

#pragma push_macro("RVV_BinopXIF_ST")
#undef  RVV_BinopXIF_ST
#define RVV_BinopXIF_ST
RVV_UnopOPI_FT_VAR(vmpopc, M, V3)
RVV_UnopOPI_FT_VAR(vmfirst, M, V3)
#pragma pop_macro("RVV_BinopXIF_ST")

/*---------------------------------------------------------------*/
/*--- OPI special function definitions                        ---*/
/*---------------------------------------------------------------*/

#pragma push_macro("RVV_BinopXIF_ST")
#undef  RVV_BinopXIF_ST
#define RVV_BinopXIF_ST
static ULong GETA_VBinopVX(vext)(VexGuestRISCV64State *st,
                                 ULong vd, ULong vs2, ULong vs1, ULong mask)
{
   RVV_BinopVX_Tpl3("vext.x.v", vd, vs2, vs1, ,);
}
#pragma pop_macro("RVV_BinopXIF_ST")

static UInt GETA_VUnopV(vid)(VexGuestRISCV64State *st,
                             ULong vd, ULong vs2, ULong mask) {
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
static UInt GETA_VUnopV_M(vid)(VexGuestRISCV64State *st,
                               ULong vd, ULong vs2, ULong mask) {
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
/*--- OPF function templates                                  ---*/
/*---------------------------------------------------------------*/

#define RVV_BinopOPF_FT_VAR_T(name, insn, op, body)                            \
   RVV_FT_VAR_T(Binop, name, insn, op, body,                                   \
                ULong vd, ULong vs2, ULong vs1, ULong mask, UInt frm)
#define RVV_BinopOPF_FT_VAR(insn, op, body)  RVV_BinopOPF_FT_VAR_T(insn, insn, op, body)

#define RVV_UnopOPF_FT_VAR_T(name, insn, op, body)                             \
   RVV_FT_VAR_T(Unop, name, insn, op, body,                                    \
                ULong vd, ULong vs2, ULong vs1, ULong mask, UInt frm)

/* Binop */
#define RVV_BinopOPFVV_FT(insn)     RVV_BinopOPF_FT_VAR(insn, VV, VV)
#define RVV_BinopOPFVF_FT(insn)     RVV_BinopOPF_FT_VAR(insn, VF, VF)
#define RVV_BinopOPFVV2_FT(insn)    RVV_BinopOPF_FT_VAR(insn, VV, VV2)
#define RVV_BinopOPFVF2_FT(insn)    RVV_BinopOPF_FT_VAR(insn, VF, VF2)
#define RVV_BinopOPFVS_FT(insn)     RVV_BinopOPF_FT_VAR(insn, VS, VV)

#define RVV_BinopOPFWV_FT(insn)     RVV_BinopOPF_FT_VAR_T(insn##w, insn, WV, VV)
#define RVV_BinopOPFWF_FT(insn)     RVV_BinopOPF_FT_VAR_T(insn##w, insn, WF, VF)

/* Unop */
#define RVV_UnopOPFV_FT(name, insn)  RVV_UnopOPF_FT_VAR_T(name, insn, V, V)

/*---------------------------------------------------------------*/
/*--- OPF function definitions                                ---*/
/*---------------------------------------------------------------*/

#define RVV_FFT_DEF(insn, vv, vf) RVV_FT_DEF(insn, vv, vf, NIL)

#pragma push_macro("RVV_Pre")
#pragma push_macro("RVV_Post")
#undef  RVV_Pre
#undef  RVV_Post
#define RVV_Pre      RVV_PushFCSR
#define RVV_Post     RVV_PopFCSR

RVV_FFT_DEF(vfadd,   BinopOPFVV,  BinopOPFVF)
RVV_FFT_DEF(vfsub,   BinopOPFVV,  BinopOPFVF)
RVV_FFT_DEF(vfrsub,  NIL,         BinopOPFVF)
RVV_FFT_DEF(vfmul,   BinopOPFVV,  BinopOPFVF)
RVV_FFT_DEF(vfdiv,   BinopOPFVV,  BinopOPFVF)
RVV_FFT_DEF(vfrdiv,  NIL,         BinopOPFVF)
RVV_FFT_DEF(vfmin,   BinopOPFVV,  BinopOPFVF)
RVV_FFT_DEF(vfmax,   BinopOPFVV,  BinopOPFVF)
RVV_FFT_DEF(vfsgnj,  BinopOPFVV,  BinopOPFVF)
RVV_FFT_DEF(vfsgnjn, BinopOPFVV,  BinopOPFVF)
RVV_FFT_DEF(vfsgnjx, BinopOPFVV,  BinopOPFVF)

#pragma push_macro("RVV_Push")
#pragma push_macro("RVV_Pop")
#undef  RVV_Push
#undef  RVV_Pop
#define RVV_Push     RVV_PushWCfg
#define RVV_Pop      RVV_PopCfg
RVV_FFT_DEF(vfwadd,  BinopOPFVV, BinopOPFVF)
RVV_FFT_DEF(vfwsub,  BinopOPFVV, BinopOPFVF)
RVV_FFT_DEF(vfwmul,  BinopOPFVV, BinopOPFVF)
RVV_FFT_DEF(vfwadd,  BinopOPFWV, BinopOPFWF)
RVV_FFT_DEF(vfwsub,  BinopOPFWV, BinopOPFWF)
#pragma pop_macro("RVV_Push")
#pragma pop_macro("RVV_Pop")

RVV_FFT_DEF(vfmacc,  BinopOPFVV2, BinopOPFVF2)
RVV_FFT_DEF(vfnmacc, BinopOPFVV2, BinopOPFVF2)
RVV_FFT_DEF(vfmsac,  BinopOPFVV2, BinopOPFVF2)
RVV_FFT_DEF(vfnmsac, BinopOPFVV2, BinopOPFVF2)
RVV_FFT_DEF(vfmadd,  BinopOPFVV2, BinopOPFVF2)
RVV_FFT_DEF(vfnmadd, BinopOPFVV2, BinopOPFVF2)
RVV_FFT_DEF(vfmsub,  BinopOPFVV2, BinopOPFVF2)
RVV_FFT_DEF(vfnmsub, BinopOPFVV2, BinopOPFVF2)
#pragma push_macro("RVV_Push")
#pragma push_macro("RVV_Pop")
#undef  RVV_Push
#undef  RVV_Pop
#define RVV_Push     RVV_PushWCfg
#define RVV_Pop      RVV_PopCfg
RVV_FFT_DEF(vfwmacc, BinopOPFVV2,BinopOPFVF2)
RVV_FFT_DEF(vfwnmacc,BinopOPFVV2,BinopOPFVF2)
RVV_FFT_DEF(vfwmsac, BinopOPFVV2,BinopOPFVF2)
RVV_FFT_DEF(vfwnmsac,BinopOPFVV2,BinopOPFVF2)
#pragma pop_macro("RVV_Push")
#pragma pop_macro("RVV_Pop")

#pragma push_macro("RVV_Push")
#pragma push_macro("RVV_Pop")
#undef  RVV_Push
#undef  RVV_Pop
#define RVV_Push     RVV_PushCfg
#define RVV_Pop      RVV_PopCfg
RVV_FFT_DEF(vmfeq,   BinopOPFVV,  BinopOPFVF)
RVV_FFT_DEF(vmfne,   BinopOPFVV,  BinopOPFVF)
RVV_FFT_DEF(vmflt,   BinopOPFVV,  BinopOPFVF)
RVV_FFT_DEF(vmfle,   BinopOPFVV,  BinopOPFVF)
RVV_FFT_DEF(vmford,  BinopOPFVV,  BinopOPFVF)
RVV_FFT_DEF(vmfgt,   NIL,         BinopOPFVF)
RVV_FFT_DEF(vmfge,   NIL,         BinopOPFVF)
#pragma pop_macro("RVV_Push")
#pragma pop_macro("RVV_Pop")

RVV_FFT_DEF(vfredosum,  BinopOPFVS,    NIL)
RVV_FFT_DEF(vfredsum,   BinopOPFVS,    NIL)
RVV_FFT_DEF(vfredmax,   BinopOPFVS,    NIL)
RVV_FFT_DEF(vfredmin,   BinopOPFVS,    NIL)
#pragma push_macro("RVV_Push")
#pragma push_macro("RVV_Pop")
#undef  RVV_Push
#undef  RVV_Pop
#define RVV_Push     RVV_PushCfg
#define RVV_Pop      RVV_PopCfg
RVV_FFT_DEF(vfwredosum, BinopOPFVS,  NIL)
RVV_FFT_DEF(vfwredsum,  BinopOPFVS,  NIL)
#pragma pop_macro("RVV_Push")
#pragma pop_macro("RVV_Pop")

RVV_UnopOPFV_FT(vfsqrt,       vfsqrt)
RVV_UnopOPFV_FT(vfclass,      vfclass)
RVV_UnopOPFV_FT(vfcvt_xu_f,   vfcvt.xu.f)
RVV_UnopOPFV_FT(vfcvt_x_f,    vfcvt.x.f)
RVV_UnopOPFV_FT(vfcvt_f_xu,   vfcvt.f.xu)
RVV_UnopOPFV_FT(vfcvt_f_x,    vfcvt.f.x)
#pragma push_macro("RVV_Push")
#pragma push_macro("RVV_Pop")
#undef  RVV_Push
#undef  RVV_Pop
#define RVV_Push     RVV_PushWCfg
#define RVV_Pop      RVV_PopCfg
RVV_UnopOPFV_FT(vfwcvt_xu_f, vfwcvt.xu.f)
RVV_UnopOPFV_FT(vfwcvt_x_f,  vfwcvt.x.f)
RVV_UnopOPFV_FT(vfwcvt_f_xu, vfwcvt.f.xu)
RVV_UnopOPFV_FT(vfwcvt_f_x,  vfwcvt.f.x)
RVV_UnopOPFV_FT(vfwcvt_f_f,  vfwcvt.f.f)
RVV_UnopOPFV_FT(vfncvt_xu_f, vfncvt.xu.f)
RVV_UnopOPFV_FT(vfncvt_x_f,  vfncvt.x.f)
RVV_UnopOPFV_FT(vfncvt_f_xu, vfncvt.f.xu)
RVV_UnopOPFV_FT(vfncvt_f_x,  vfncvt.f.x)
RVV_UnopOPFV_FT(vfncvt_f_f,  vfncvt.f.f)
#pragma pop_macro("RVV_Push")
#pragma pop_macro("RVV_Pop")

RVV_UnopOPF_FT_VAR_T(vfmerge, vfmv.v, F, F)

#pragma push_macro("VInsnVF")
#undef  VInsnVF
#define VInsnVF(insn) #insn
RVV_BinopOPF_FT_VAR_T(vfmerge, vfmerge.vfm, VF, VFM)
#pragma pop_macro("VInsnVF")

#pragma push_macro("RVV_Push")
#pragma push_macro("RVV_Pop")
#undef  RVV_Push
#undef  RVV_Pop
#define RVV_Push     RVV_PushCfg
#define RVV_Pop      RVV_PopCfg
RVV_UnopOPF_FT_VAR_T(vfmvs, vfmv.s, F, F)
#pragma pop_macro("RVV_Push")
#pragma pop_macro("RVV_Pop")

/*---------------------------------------------------------------*/
/*--- OPF special function definitions                        ---*/
/*---------------------------------------------------------------*/

#pragma push_macro("RVV_BinopXIF_ST")
#pragma push_macro("RVV_BinopXIF_iTpl3")
#undef  RVV_BinopXIF_ST
#undef  RVV_BinopXIF_iTpl3
#define RVV_BinopXIF_ST
#define RVV_BinopXIF_iTpl3(insn, treg, mreg)\
   __asm__ __volatile__(insn "\tft0,v16" treg mreg "\n\t"   \
                        "fmv.x.d\t%0,ft0\n\t"               \
                        :"=r"(ret)::"ft0");
static ULong GETA_VUnopV(vfmv)(VexGuestRISCV64State *st,
                               ULong vd, ULong vs2, ULong vs1, ULong mask, UInt frm) {
   RVV_UnopV_Tpl3("vfmv.f.s", vd, vs2, vs1, ,);
}
#pragma pop_macro("RVV_BinopXIF_ST")
#pragma pop_macro("RVV_BinopXIF_iTpl3")

#pragma pop_macro("RVV_Pre")
#pragma pop_macro("RVV_Post")

/* Load/store dirty helpers

   Vector load/store instructions are categorized into 4 types where
   segment load/store is a rvv extension supporting array-of-structures
   load/store with unit-stride/strided/indexed variants:

      1. Unit-stride normal load/store
         1.1 Vector load
         1.2 Vector store
      2. Strided load/store
         2.1 Strided load
         2.2 Strided store
      3. Indexed load/store
         3.1 Indexed load
         3.2 Indexed store
      4. Segment load/store
         4.1 Segment load
            4.1.1 Segment unit-stride load
            4.1.2 Segment stride load
            4.1.3 Segment indexed load
         4.2 Segment store
            4.2.1 Segment unit-stride store
            4.2.2 Segment stride store
            4.2.3 Segment indexed store

   The dirty helpers fulfill load by:
             vload1                  vstore1
      Memory -----> Host vector regs ------> GuestState vector regs
   vload1 gets its address from vCPU gpr and possible indexes from
   vCPU vector regs. Other info such as LMUL, SEW, nfield, stride are also
   obtained from vCPU. As elements loaded from memory are casted to SEW,
   vstore1 is a direct vse.v.

   vstore1 puts host vector reg contents to vCPU and the reg number is
   encoded in the disassembled instruction.

   The dirty helpers fulfill store by:
                             vload2                  vstore2
      GuestState vector regs -----> Host vector regs ------> Memory

   The following macros are annotated by different memory accessing phases
   and these phases are described above as vload{1,2} and vstore{1,2}.
*/

/*----------------------------------------------------------*/
/*---  Some utilities for all load/store                 ---*/
/*----------------------------------------------------------*/

/* Distinguish different load/store types */
typedef enum {
   UnitStride,
   Strided,
   Indexed,
} VLdstT;

/* Use following macros to get *name* and *address* of a specific dirty helper */
#define GETA_VLdst_M(insn)  RVV_VLdst_##insn##_vm
#define GETN_VLdst_M(insn)  "RVV_VLdst_"#insn"_vm"
#define GETA_VLdst(insn)    RVV_VLdst_##insn
#define GETN_VLdst(insn)    "RVV_VLdst_"#insn

/* Generic template for all vector loads */
#define RVV_Load_Tpl(insn, vm,               \
                     loadMask,               \
                     loadMemory,             \
                     storeGuestState)        \
do {                                         \
      RVV_Config();                          \
      /* Get rs1/vd absolute offset */       \
      ULong rs1_offs = (ULong) st + rs1;     \
      ULong rs1_addr = *((ULong *) rs1_offs);\
      ULong vd_offs  = (ULong) st + v;       \
      mask           = (ULong) st + mask;    \
                                             \
      loadMask                               \
                                             \
      /* vload1: Load from memory */         \
      loadMemory                             \
                                             \
      /* vstore1: Store to GuestState */     \
      storeGuestState                        \
} while (0)

/* Generic template for all vector stores */
#define RVV_Store_Tpl(insn, vm,              \
                      loadMask,              \
                      loadGuestState,        \
                      storeMemory)           \
do {                                         \
      RVV_Config();                          \
      /* Get rd/vs absolute offset */        \
      ULong rs1_offs = (ULong) st + rs1;     \
      ULong rs1_addr = *((ULong *) rs1_offs);\
      ULong vs_offs  = (ULong) st + v;       \
      mask           = (ULong) st + mask;    \
                                             \
      loadMask                               \
                                             \
      /* vload2: Load from GuestState */     \
      loadGuestState                         \
                                             \
      /* vstore2: Store to memory */         \
      storeMemory                            \
} while (0)

/* Dirty helper for converting v0 to a real mask */
/* This is the tricky part about converting a v0 mask to multiple segments.
   Here is the outline:
      As each mask element in v0 has a width of SEW/LMUL, we need to convert masks to
      SEW width for better addressing them. We zero out v8 and then add 1 to v8
      with vm on. Therefore, all masked elements keep 0 while others are 1,
      meaning that v8 has all masks represented in SEW width. The next step is
      sliding down N elements to get the required mask. Finally, we store these
      masks in SEW width down to res (64-bit).

   By doing this, we get an ULong result where each 8-bit segment is a mask in v0.
   Totally, it contains 5 + 9 = 14 lines of inline assembly code. */
static ULong dirty_get_mask(VexGuestRISCV64State *st,
                            ULong mask, UInt n_slidedown) {
   ULong res      = 0;
   ULong res_addr = (ULong) &res; 

   mask += (ULong) st;
   RVV_Mask();

   __asm__ __volatile__ (
      "vmv.v.i\tv8,0\n\t"            /* create a full-zero v8 */
      "vadd.vi\tv8,v8,1,v0.t\n\t"    /* elements are incremented by 1 in unmasked places */
      "vslidedown.vx\tv8,v8,%0\n\t"  /* slide down v8 to what we want in the lowest lane */
      "csrr\tt0,vl\n\t"
      "csrr\tt1,vtype\n\t"
      "addi\tt2,x0,8\n\t"
      "vsetvl\tx0,t2,t1\n\t"         /* set a safe vl before storing v8 down to res */
      "vsb.v\tv8,(%1)\n\t"           /* 8-bit mask elements are stored to res */
      "vsetvl\tx0,t0,t1\n\t"         /* restore previous vl */
      ::"r"(n_slidedown), "r"(res_addr)
      : "t0", "t1", "t2", "memory"
   );
   return res;
}

/* Fill mMask array */
static inline IRExpr** calculate_dirty_mask(IRSB *irsb     /* MOD */,
                                            Bool mask,
                                            UInt vl,
                                            UInt vstart) {
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
      IRExpr** args = mkIRExprVec_3(IRExpr_GSPTR(), mkU64(offsetVReg(0)),
                                    mkU32(vstart + idx));
      IRDirty *m_d = unsafeIRDirty_1_N(mask_segs, 0, "dirty_get_mask",
                                       dirty_get_mask, args);
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

/* Prepare Vload/Vstore dirty helper info */
static IRDirty*
GETD_Common_VLdSt(IRSB *irsb,                /* MOD */
                  IRDirty* d,                /* OUT */
                  UInt v, UInt r, UInt s2,   /* Inst oprd */
                  Bool mask, Bool isLD,
                  UInt nf, ULong width,
                  VLdstT ldst_ty)
{
   UInt lmul   = extract_lmul_0p7(guest_VFLAG);
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
         UInt sew = 31 - __builtin_clz((UInt) extract_sew_0p7(guest_VFLAG));
         IROp       ops[4] = {Iop_8Sto64, Iop_16Sto64, Iop_32Sto64, Iop_LAST};
         IRType off_tys[4] = {Ity_I8, Ity_I16, Ity_I32, Ity_I64};
         vassert(sew >=0 && sew <= 3);
         IRExpr* offset;
         for (UInt i = vstart; i < vl; i++) {
            if (ops[sew] != Iop_LAST)
               offset = unop(ops[sew], getVRegLane(s2, i, off_tys[sew]));
            else
               offset = getVRegLane(s2, i, off_tys[sew]);
            addrV[idx++] = binop(Iop_Add64, getIReg64(r), offset);
         }
         break;
      }
      default:
         vassert(0);
   }
   d->mAddrVec = addrV;

   /* Mask info */
   d->mMask = calculate_dirty_mask(irsb, mask, vl, vstart);

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

/* Prepare dirty helper arguments */
#define UNIT_STRIDE_C_ARGS                                           \
args = mkIRExprVec_4(IRExpr_GSPTR(),           /* arg0: GS pointer*/ \
                     mkU64(offsetVReg(v)),     /* arg1: vector reg*/ \
                     mkU64(offsetIReg64(rs)),  /* arg2: gpr */       \
                     mkU64(offsetVReg(0)));    /* arg3: mask v0 */

#define STRIDED_C_ARGS                                               \
args = mkIRExprVec_5(IRExpr_GSPTR(),           /* arg0: GS pointer*/ \
                     mkU64(offsetVReg(v)),     /* arg1: vector reg*/ \
                     mkU64(offsetIReg64(rs)),  /* arg2: gpr */       \
                     mkU64(offsetIReg64(rs2)), /* arg3: rs2 */       \
                     mkU64(offsetVReg(0)));    /* arg4: mask v0 */

#define INDEXED_C_ARGS                                               \
args = mkIRExprVec_5(IRExpr_GSPTR(),           /* arg0: GS pointer*/ \
                     mkU64(offsetVReg(v)),     /* arg1: vector reg*/ \
                     mkU64(offsetIReg64(rs)),  /* arg2: gpr */       \
                     mkU64(offsetVReg(vs2)),   /* arg3: vs2 */       \
                     mkU64(offsetVReg(0)));    /* arg4: mask v0 */

/* Macros for creating a proper dirty helper according to insn types */
#define MK_GETD(s2, nf, ldst_ty) \
   GETD_Common_VLdSt(irsb, d, v, rs, s2, mask, isLD, nf, width, ldst_ty);

#define UNIT_STRIDE_GETD        MK_GETD(0,   1,  UnitStride)
#define STRIDED_GETD            MK_GETD(rs2, 1,  Strided)
#define INDEXED_GETD            MK_GETD(vs2, 1,  Indexed)
#define SEG_UNIT_STRIDE_GETD    MK_GETD(0,   nf, UnitStride)
#define SEG_STRIDED_GETD        MK_GETD(rs2, nf, Strided)
#define SEG_INDEXED_GETD        MK_GETD(vs2, nf, Indexed)

/* Macros for debug printing according to insn types */
#define UNIT_STRIDE_DIP \
   DIP("%s(%s, %s)\n", fName, nameVReg(v), nameIReg(rs));
#define STRIDED_DIP \
   DIP("%s(%s, %s, %s)\n", fName, nameVReg(v), nameIReg(rs), nameIReg(rs2));
#define INDEXED_DIP \
   DIP("%s(%s, %s, %s)\n", fName, nameVReg(v), nameIReg(rs), nameVReg(vs2));

/* Setup required dirty call with args and info */
#define GETC_VLdSt(insn, mk_c_args, mk_getd, mk_dip)         \
   do {                                                      \
      UInt vstart = extract_vstart(guest_VFLAG);             \
      UInt vl     = extract_vl(guest_VFLAG);                 \
      fName = mask ? GETN_VLdst(insn) : GETN_VLdst_M(insn);  \
      fAddr = mask ? GETA_VLdst(insn) : GETA_VLdst_M(insn);  \
      if (vstart >= vl) {                                    \
         mk_dip                                              \
         break;                                              \
      }                                                      \
      mk_c_args                                              \
      d = unsafeIRDirty_0_N(0, fName, fAddr, args);          \
      d = mk_getd                                            \
      stmt(irsb, IRStmt_Dirty(d));                           \
                                                             \
      mk_dip                                                 \
   } while (0)

/* Factory for all load/stores, vertically list macro parameters for better readability */
#define RVV_Load_Memory(insn, vm)
#define RVV_Store_GuestState(nf, vm)
#define RVV_Load_GuestState(nf, vm)
#define RVV_Store_Memory(insn, vm)

#define DIRTY_VLOAD_BODY_VM(insn, nf)                                     \
   RVV_Load_Tpl(insn, ",v0.t",                         /* insn info */    \
                RVV_Mask(),                            /* load mask*/     \
                RVV_Load_Memory(insn, ",v0.t"),        /* load memory */  \
                RVV_Store_GuestState(nf, ",v0.t"));    /* store GS */     \

#define DIRTY_VLOAD_BODY(insn, nf)                                        \
   RVV_Load_Tpl(insn, ,                                /* insn info */    \
                ,                                      /* load mask*/     \
                RVV_Load_Memory(insn, ),               /* load memory */  \
                RVV_Store_GuestState(nf, ));           /* store GS */     \

#define DIRTY_VSTORE_BODY_VM(insn, nf)                                    \
   RVV_Store_Tpl(insn, ",v0.t",                                           \
                 RVV_Mask(),                                              \
                 RVV_Load_GuestState(nf, ",v0.t"),                        \
                 RVV_Store_Memory(insn, ",v0.t"));

#define DIRTY_VSTORE_BODY(insn, nf)                                       \
   RVV_Store_Tpl(insn, ,                                                  \
                 ,                                                        \
                 RVV_Load_GuestState(nf,),                                \
                 RVV_Store_Memory(insn, ));

#define RVV_VLdst_Generic(insn, body, nf)                                 \
   static void RVV_VLdst_##insn##_vm(VexGuestRISCV64State *st,            \
                               ULong v, ULong rs1, ULong mask) {          \
      body##_VM(insn, nf)                                                 \
   }                                                                      \
   static void RVV_VLdst_##insn(VexGuestRISCV64State *st,                 \
                               ULong v, ULong rs1, ULong mask) {          \
      body(insn, nf)                                                      \
   }

#define RVV_VSXLdst_Generic(insn, body, nf)                               \
   static void RVV_VLdst_##insn##_vm(VexGuestRISCV64State *st,            \
                            ULong v, ULong rs1, ULong s2, ULong mask) {   \
      body##_VM(insn, nf)                                                 \
   }                                                                      \
   static void RVV_VLdst_##insn(VexGuestRISCV64State *st,                 \
                            ULong v, ULong rs1, ULong s2, ULong mask) {   \
      body(insn, nf)                                                      \
   }

#define RVV_VLdst(insn, body)           RVV_VLdst_Generic(insn, body, )
#define RVV_VSXLdst(insn, body)         RVV_VSXLdst_Generic(insn, body, )
#define RVV_VSEGLdst(insn, body, nf)    RVV_VLdst_Generic(insn, body, nf)
#define RVV_VSEGSXLdst(insn, body, nf)  RVV_VSXLdst_Generic(insn, body, nf)

/* The following macros are directly used in toIR stage */
#define GETC_VLDST(insn) \
   GETC_VLdSt(insn, UNIT_STRIDE_C_ARGS, UNIT_STRIDE_GETD, UNIT_STRIDE_DIP)
#define GETC_VSLDST(insn) \
   GETC_VLdSt(insn, STRIDED_C_ARGS, STRIDED_GETD, STRIDED_DIP)
#define GETC_VXLDST(insn) \
   GETC_VLdSt(insn, INDEXED_C_ARGS, INDEXED_GETD, INDEXED_DIP)
#define GETC_VSEGLDST(insn) \
   GETC_VLdSt(insn, UNIT_STRIDE_C_ARGS, SEG_UNIT_STRIDE_GETD, UNIT_STRIDE_DIP)
#define GETC_VSSEGLDST(insn) \
   GETC_VLdSt(insn, STRIDED_C_ARGS, SEG_STRIDED_GETD, STRIDED_DIP)
#define GETC_VXSEGLDST(insn) \
   GETC_VLdSt(insn, INDEXED_C_ARGS, SEG_INDEXED_GETD, INDEXED_DIP)

/*----------------------------------------------------------*/
/*---   1.1 Unit-stride vector load dirty helpers        ---*/
/*----------------------------------------------------------*/

#define RVV_Unit_Stride_Load_Store_Memory(insn, vm)    \
   RVV_ConfigVstart();                                 \
   /* vload1: Load from memory */                      \
   __asm__ __volatile__ (                              \
      #insn ".v\tv8,(%0)\t" vm "\n\t"                  \
      ::"r" (rs1_addr)                                 \
      :                                                \
   );

/* Store to guest state for unit-stride(U), strided(S), and indexed(X) */
#define RVV_USX_Store_GuestState(nf, vm)     \
   RVV_ConfigVstart();                       \
   /* vstore1: Store to GuestState */        \
   __asm__ __volatile__ (                    \
      "vse.v\tv8,(%0)\t" vm "\n\t"           \
      ::"r" (vd_offs)                        \
      :                                      \
   );

#undef  RVV_Load_Memory
#undef  RVV_Store_GuestState
#define RVV_Load_Memory       RVV_Unit_Stride_Load_Store_Memory
#define RVV_Store_GuestState  RVV_USX_Store_GuestState

RVV_VLdst(vlb,  DIRTY_VLOAD_BODY)
RVV_VLdst(vlh,  DIRTY_VLOAD_BODY)
RVV_VLdst(vlw,  DIRTY_VLOAD_BODY)
RVV_VLdst(vle,  DIRTY_VLOAD_BODY)
RVV_VLdst(vlbu, DIRTY_VLOAD_BODY)
RVV_VLdst(vlhu, DIRTY_VLOAD_BODY)
RVV_VLdst(vlwu, DIRTY_VLOAD_BODY)

/*----------------------------------------------------------*/
/*---   1.2 Unit-stride vector store dirty helpers       ---*/
/*----------------------------------------------------------*/

#define RVV_USX_Load_GuestState(nf, vm)        \
   __asm__ __volatile__ (                      \
      "vle.v\tv8,(%0)\t" vm "\n\t"             \
      ::"r" (vs_offs)                          \
      :                                        \
   );

#undef  RVV_Load_GuestState
#undef  RVV_Store_Memory
#define RVV_Load_GuestState  RVV_USX_Load_GuestState
#define RVV_Store_Memory     RVV_Unit_Stride_Load_Store_Memory

RVV_VLdst(vsb, DIRTY_VSTORE_BODY)
RVV_VLdst(vsh, DIRTY_VSTORE_BODY)
RVV_VLdst(vsw, DIRTY_VSTORE_BODY)
RVV_VLdst(vse, DIRTY_VSTORE_BODY)

/*----------------------------------------------------------*/
/*---   2.1 Strided vector load dirty helpers            ---*/
/*----------------------------------------------------------*/

#define RVV_Strided_Load_Store_Memory(insn, vm)       \
   RVV_ConfigVstart();                                \
   /* vload1/vstore2: Load from or store to memory */ \
   ULong rs2_offs = (ULong) st + s2;                  \
   ULong rs2_addr = *((ULong *) rs2_offs);            \
   __asm__ __volatile__ (                             \
      #insn ".v\tv8,(%1),%0\t" vm "\n\t"              \
      ::"r"(rs2_addr), "r" (rs1_addr)                 \
      :                                               \
   );

#undef  RVV_Load_Memory
#define RVV_Load_Memory     RVV_Strided_Load_Store_Memory

RVV_VSXLdst(vlsb,  DIRTY_VLOAD_BODY)
RVV_VSXLdst(vlsh,  DIRTY_VLOAD_BODY)
RVV_VSXLdst(vlsw,  DIRTY_VLOAD_BODY)
RVV_VSXLdst(vlse,  DIRTY_VLOAD_BODY)
RVV_VSXLdst(vlsbu, DIRTY_VLOAD_BODY)
RVV_VSXLdst(vlshu, DIRTY_VLOAD_BODY)
RVV_VSXLdst(vlswu, DIRTY_VLOAD_BODY)

/*----------------------------------------------------------*/
/*---   2.2 Strided vector store dirty helpers           ---*/
/*----------------------------------------------------------*/

#undef  RVV_Store_Memory
#define RVV_Store_Memory    RVV_Strided_Load_Store_Memory

RVV_VSXLdst(vssb, DIRTY_VSTORE_BODY)
RVV_VSXLdst(vssh, DIRTY_VSTORE_BODY)
RVV_VSXLdst(vssw, DIRTY_VSTORE_BODY)
RVV_VSXLdst(vsse, DIRTY_VSTORE_BODY)

/*----------------------------------------------------------*/
/*---   3.1 Indexed vector load dirty helpers            ---*/
/*----------------------------------------------------------*/

#define RVV_Indexed_Load_Store_Memory(insn, vm)       \
   ULong vs2_offs = (ULong) st + s2;                  \
   /* vload1/vstore2: Load from or store to memory */ \
   __asm__ __volatile__ (                             \
      "vle.v\tv16,(%0)\t" vm "\n\t"                   \
      ::"r"(vs2_offs)                                 \
      :                                               \
   );                                                 \
   RVV_ConfigVstart();                                \
   __asm__ __volatile__ (                             \
      #insn ".v\tv8,(%0),v16" vm "\n\t"               \
      ::"r" (rs1_addr)                                \
      :                                               \
   );

#undef  RVV_Load_Memory
#define RVV_Load_Memory      RVV_Indexed_Load_Store_Memory

RVV_VSXLdst(vlxb,  DIRTY_VLOAD_BODY)
RVV_VSXLdst(vlxh,  DIRTY_VLOAD_BODY)
RVV_VSXLdst(vlxw,  DIRTY_VLOAD_BODY)
RVV_VSXLdst(vlxe,  DIRTY_VLOAD_BODY)
RVV_VSXLdst(vlxbu, DIRTY_VLOAD_BODY)
RVV_VSXLdst(vlxhu, DIRTY_VLOAD_BODY)
RVV_VSXLdst(vlxwu, DIRTY_VLOAD_BODY)

/*----------------------------------------------------------*/
/*---   3.2 Indexed vector store dirty helpers           ---*/
/*----------------------------------------------------------*/

#undef  RVV_Store_Memory
#define RVV_Store_Memory    RVV_Indexed_Load_Store_Memory

RVV_VSXLdst(vsxb,  DIRTY_VSTORE_BODY)
RVV_VSXLdst(vsuxb, DIRTY_VSTORE_BODY)
RVV_VSXLdst(vsxh,  DIRTY_VSTORE_BODY)
RVV_VSXLdst(vsuxh, DIRTY_VSTORE_BODY)
RVV_VSXLdst(vsxw,  DIRTY_VSTORE_BODY)
RVV_VSXLdst(vsuxw, DIRTY_VSTORE_BODY)
RVV_VSXLdst(vsxe,  DIRTY_VSTORE_BODY)
RVV_VSXLdst(vsuxe, DIRTY_VSTORE_BODY)

/*----------------------------------------------------------*/
/*---   4.1 Segment vector load dirty helpers (Zvlsseg)  ---*/
/*----------------------------------------------------------*/

/* For generating all nf variants of switch-case in dis_RV64V_ldst */
#define VSEG_DIS_NF_CASES(get_c_macro, insn_prefix, \
                          insn_suffix)              \
   switch (nf) {                                    \
      case 2:                                       \
         get_c_macro(insn_prefix##2##insn_suffix);  \
         return True;                               \
      case 3:                                       \
         get_c_macro(insn_prefix##3##insn_suffix);  \
         return True;                               \
      case 4:                                       \
         get_c_macro(insn_prefix##4##insn_suffix);  \
         return True;                               \
      case 5:                                       \
         get_c_macro(insn_prefix##5##insn_suffix);  \
         return True;                               \
      case 6:                                       \
         get_c_macro(insn_prefix##6##insn_suffix);  \
         return True;                               \
      case 7:                                       \
         get_c_macro(insn_prefix##7##insn_suffix);  \
         return True;                               \
      case 8:                                       \
         get_c_macro(insn_prefix##8##insn_suffix);  \
         return True;                               \
      default:                                      \
         return False;                              \
   }

/* For generating all nf(2-8) variants in a batch */
#define RVV_VSEG_NF_DEFS(ldst_macro, insn_prefix,    \
                            body, insn_suffix)       \
ldst_macro(insn_prefix##2##insn_suffix, body, 2)     \
ldst_macro(insn_prefix##3##insn_suffix, body, 3)     \
ldst_macro(insn_prefix##4##insn_suffix, body, 4)     \
ldst_macro(insn_prefix##5##insn_suffix, body, 5)     \
ldst_macro(insn_prefix##6##insn_suffix, body, 6)     \
ldst_macro(insn_prefix##7##insn_suffix, body, 7)     \
ldst_macro(insn_prefix##8##insn_suffix, body, 8)

/* Load/store vector registers and update the address 
   for accessing virtual guest state in the next step
   to the correct location. */
#define LDST_VEC_UPDATE_ADDR(insn, v_reg, vm) \
   __asm__ __volatile__(                      \
      #insn ".v\tv" #v_reg ",(%0)" vm "\n\t"  \
      "sub\t%0,%1,%2\n\t"                     \
      :"=r"(v_end)                            \
      :"0"(v_end), "r"(reg_len):              \
   );                                         \
   __attribute__ ((fallthrough));

/* Load/store guest state
   Segment load/store guest state should take both nf and
   lmul into account, making it more complicated than normal
   load/store. We achieve guest state load/store by iterating
   all virtual registers that a segment instruction touches.

   We split vector registers by lmul using the outmost switch,
   and further determine how many register groups to load/store
   by nf using the nested switch. For example, in the condition
   of LMUL=2 and NF=3 in a seg load, there are 6 registers accessed
   in a single instruction. Accordingly, we fall into the lmul
   case 1, and nf case 3, where the memory contents will be stored
   to vCPU registers from v13(v12), v11(v10), and v9(v8) by falling
   through case 3 to case 1.
*/
#define RVV_Seg_LoadStore_GuestState(insn, nf, vm, v)    \
                                                         \
   ULong lmul = 0;                                       \
   __asm__ __volatile__(                                 \
      "csrr\t%0,vtype\n\t"                               \
      "andi\t%0,%0,0x03\n\t"                             \
   :"=r"(lmul)::);                                       \
                                                         \
   lmul = 1 << lmul;                                     \
   ULong reg_len = host_VLENB * lmul;                    \
   ULong v_end   = v##_offs + (nf - 1) * reg_len;        \
                                                         \
   switch (lmul) {                                       \
      case 1: {                                          \
         /* LMUL = 1 */                                  \
         switch (nf) {                                   \
            case 8:                                      \
               LDST_VEC_UPDATE_ADDR(insn, 15, vm)        \
            case 7:                                      \
               LDST_VEC_UPDATE_ADDR(insn, 14, vm)        \
            case 6:                                      \
               LDST_VEC_UPDATE_ADDR(insn, 13, vm)        \
            case 5:                                      \
               LDST_VEC_UPDATE_ADDR(insn, 12, vm)        \
            case 4:                                      \
               LDST_VEC_UPDATE_ADDR(insn, 11, vm)        \
            case 3:                                      \
               LDST_VEC_UPDATE_ADDR(insn, 10, vm)        \
            case 2:                                      \
               LDST_VEC_UPDATE_ADDR(insn,  9, vm)        \
            case 1:                                      \
               LDST_VEC_UPDATE_ADDR(insn,  8, vm)        \
            default:                                     \
               break;                                    \
        }                                                \
        break;                                           \
      }                                                  \
      case 2: {                                          \
         /* LMUL = 2 */                                  \
         switch (nf) {                                   \
            case 4:                                      \
               LDST_VEC_UPDATE_ADDR(insn, 14, vm)        \
            case 3:                                      \
               LDST_VEC_UPDATE_ADDR(insn, 12, vm)        \
            case 2:                                      \
               LDST_VEC_UPDATE_ADDR(insn, 10, vm)        \
            case 1:                                      \
               LDST_VEC_UPDATE_ADDR(insn,  8, vm)        \
            default:                                     \
               break;                                    \
        }                                                \
        break;                                           \
      }                                                  \
      case 4: {                                          \
         /* LMUL = 4 */                                  \
         switch (nf) {                                   \
            case 2:                                      \
               LDST_VEC_UPDATE_ADDR(insn, 12, vm)        \
            case 1:                                      \
               LDST_VEC_UPDATE_ADDR(insn,  8, vm)        \
            default:                                     \
               break;                                    \
         }                                               \
         break;                                          \
      }                                                  \
      case 8: {                                          \
         /* LMUL = 8 */                                  \
         __asm__ __volatile__(                           \
            #insn ".v\tv8,(%0)" vm "\n\t"                \
         ::"r"(v_end):);                                 \
         break;                                          \
      }                                                  \
      default:                                           \
         break;                                          \
   }

#define RVV_Seg_PreLoad_Init                              \
   /* Initialize host vec regs using guest vec regs. */   \
   /* Use maximal available bytes. */                     \
   ULong tailing_bytes = mask + (host_VLENB << 5) - v;    \
   __asm__ __volatile__(                                  \
      "csrr\tt0,vl\n"                                     \
      "csrr\tt1,vtype\n"                                  \
      "vsetvli\tx0,%0,e8,m8\n"                            \
      "vle.v\tv8,(%1)\n"                                  \
      "vsetvl\tx0,t0,t1\n"                                \
      ::"r"(tailing_bytes), "r"(vd_offs)                  \
      :"t0", "t1"                                         \
   );

/* 4.1.1 Segment unit-stride load */
#define RVV_Seg_Store_GuestState(nf, vm) \
   RVV_Seg_LoadStore_GuestState(vse, nf, vm, vd)

#define RVV_Seg_Unit_Stride_Load_Memory(insn, vm)     \
   RVV_Seg_PreLoad_Init                               \
   RVV_Unit_Stride_Load_Store_Memory(insn, vm)

#undef  RVV_Load_Memory
#undef  RVV_Store_GuestState
#define RVV_Load_Memory        RVV_Seg_Unit_Stride_Load_Memory
#define RVV_Store_GuestState   RVV_Seg_Store_GuestState

RVV_VSEG_NF_DEFS(RVV_VSEGLdst, vlseg, DIRTY_VLOAD_BODY, b)
RVV_VSEG_NF_DEFS(RVV_VSEGLdst, vlseg, DIRTY_VLOAD_BODY, h)
RVV_VSEG_NF_DEFS(RVV_VSEGLdst, vlseg, DIRTY_VLOAD_BODY, w)
RVV_VSEG_NF_DEFS(RVV_VSEGLdst, vlseg, DIRTY_VLOAD_BODY, bu)
RVV_VSEG_NF_DEFS(RVV_VSEGLdst, vlseg, DIRTY_VLOAD_BODY, hu)
RVV_VSEG_NF_DEFS(RVV_VSEGLdst, vlseg, DIRTY_VLOAD_BODY, wu)
RVV_VSEG_NF_DEFS(RVV_VSEGLdst, vlseg, DIRTY_VLOAD_BODY, e)

/* 4.1.2 Segment unit-stride store */
#define RVV_Seg_USX_Load_GuestState(nf, vm) \
   RVV_Seg_LoadStore_GuestState(vle, nf, vm, vs)

#undef  RVV_Load_GuestState
#undef  RVV_Store_Memory
#define RVV_Load_GuestState    RVV_Seg_USX_Load_GuestState
#define RVV_Store_Memory       RVV_Unit_Stride_Load_Store_Memory

RVV_VSEG_NF_DEFS(RVV_VSEGLdst, vsseg, DIRTY_VSTORE_BODY, b)
RVV_VSEG_NF_DEFS(RVV_VSEGLdst, vsseg, DIRTY_VSTORE_BODY, h)
RVV_VSEG_NF_DEFS(RVV_VSEGLdst, vsseg, DIRTY_VSTORE_BODY, w)
RVV_VSEG_NF_DEFS(RVV_VSEGLdst, vsseg, DIRTY_VSTORE_BODY, e)

/* 4.2.1 Segment strided load */
#define RVV_Seg_Strided_Load_Memory(insn, vm)  \
   RVV_Seg_PreLoad_Init                        \
   RVV_Strided_Load_Store_Memory(insn, vm)

#undef  RVV_Load_Memory
#define RVV_Load_Memory        RVV_Seg_Strided_Load_Memory

RVV_VSEG_NF_DEFS(RVV_VSEGSXLdst, vlsseg, DIRTY_VLOAD_BODY, b)
RVV_VSEG_NF_DEFS(RVV_VSEGSXLdst, vlsseg, DIRTY_VLOAD_BODY, h)
RVV_VSEG_NF_DEFS(RVV_VSEGSXLdst, vlsseg, DIRTY_VLOAD_BODY, w)
RVV_VSEG_NF_DEFS(RVV_VSEGSXLdst, vlsseg, DIRTY_VLOAD_BODY, bu)
RVV_VSEG_NF_DEFS(RVV_VSEGSXLdst, vlsseg, DIRTY_VLOAD_BODY, hu)
RVV_VSEG_NF_DEFS(RVV_VSEGSXLdst, vlsseg, DIRTY_VLOAD_BODY, wu)
RVV_VSEG_NF_DEFS(RVV_VSEGSXLdst, vlsseg, DIRTY_VLOAD_BODY, e)

/* 4.2.2 Segment strided store */
#undef  RVV_Store_Memory
#define RVV_Store_Memory       RVV_Strided_Load_Store_Memory

RVV_VSEG_NF_DEFS(RVV_VSEGSXLdst, vssseg, DIRTY_VSTORE_BODY, b)
RVV_VSEG_NF_DEFS(RVV_VSEGSXLdst, vssseg, DIRTY_VSTORE_BODY, h)
RVV_VSEG_NF_DEFS(RVV_VSEGSXLdst, vssseg, DIRTY_VSTORE_BODY, w)
RVV_VSEG_NF_DEFS(RVV_VSEGSXLdst, vssseg, DIRTY_VSTORE_BODY, e)

/* 4.3.1 Segment indexed load */
#define RVV_Seg_Indexed_Load_Memory(insn, vm)  \
   RVV_Seg_PreLoad_Init                        \
   RVV_Indexed_Load_Store_Memory(insn, vm)

#undef  RVV_Load_Memory
#define RVV_Load_Memory        RVV_Seg_Indexed_Load_Memory

RVV_VSEG_NF_DEFS(RVV_VSEGSXLdst, vlxseg, DIRTY_VLOAD_BODY, b)
RVV_VSEG_NF_DEFS(RVV_VSEGSXLdst, vlxseg, DIRTY_VLOAD_BODY, h)
RVV_VSEG_NF_DEFS(RVV_VSEGSXLdst, vlxseg, DIRTY_VLOAD_BODY, w)
RVV_VSEG_NF_DEFS(RVV_VSEGSXLdst, vlxseg, DIRTY_VLOAD_BODY, bu)
RVV_VSEG_NF_DEFS(RVV_VSEGSXLdst, vlxseg, DIRTY_VLOAD_BODY, hu)
RVV_VSEG_NF_DEFS(RVV_VSEGSXLdst, vlxseg, DIRTY_VLOAD_BODY, wu)
RVV_VSEG_NF_DEFS(RVV_VSEGSXLdst, vlxseg, DIRTY_VLOAD_BODY, e)

/* 4.3.2 Segment indexed store */
#undef  RVV_Store_Memory
#define RVV_Store_Memory       RVV_Indexed_Load_Store_Memory

RVV_VSEG_NF_DEFS(RVV_VSEGSXLdst, vsxseg, DIRTY_VSTORE_BODY, b)
RVV_VSEG_NF_DEFS(RVV_VSEGSXLdst, vsxseg, DIRTY_VSTORE_BODY, h)
RVV_VSEG_NF_DEFS(RVV_VSEGSXLdst, vsxseg, DIRTY_VSTORE_BODY, w)
RVV_VSEG_NF_DEFS(RVV_VSEGSXLdst, vsxseg, DIRTY_VSTORE_BODY, e)

/*--------------------------------------------------------------------*/
/*--- end                               guest_riscv64V0p7_helpers.c --*/
/*--------------------------------------------------------------------*/
