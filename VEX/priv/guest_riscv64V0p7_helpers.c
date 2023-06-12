
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

/*---------------------------------------------------------------*/
/*--- Get Name/Address of helper functions                    ---*/
/*---------------------------------------------------------------*/

/* Binop */
#define GETN_VBinopVV_M(insn)  "RVV0p7_Binop_"#insn"vv_m"
#define GETA_VBinopVV_M(insn)  RVV0p7_Binop_##insn##vv_m
#define GETN_VBinopVV(insn)    "RVV0p7_Binop_"#insn"vv"
#define GETA_VBinopVV(insn)    RVV0p7_Binop_##insn##vv

#define GETN_VBinopVX_M(insn)  "RVV0p7_Binop_"#insn"vx_m"
#define GETA_VBinopVX_M(insn)  RVV0p7_Binop_##insn##vx_m
#define GETN_VBinopVX(insn)    "RVV0p7_Binop_"#insn"vx"
#define GETA_VBinopVX(insn)    RVV0p7_Binop_##insn##vx

#define GETN_VBinopVI_M(insn)  "RVV0p7_Binop_"#insn"vi_m"
#define GETA_VBinopVI_M(insn)  RVV0p7_Binop_##insn##vi_m
#define GETN_VBinopVI(insn)    "RVV0p7_Binop_"#insn"vi"
#define GETA_VBinopVI(insn)    RVV0p7_Binop_##insn##vi

#define GETN_VBinopVF_M(insn)  "RVV0p7_Binop_"#insn"vf_m"
#define GETA_VBinopVF_M(insn)  RVV0p7_Binop_##insn##vf_m
#define GETN_VBinopVF(insn)    "RVV0p7_Binop_"#insn"vf"
#define GETA_VBinopVF(insn)    RVV0p7_Binop_##insn##vf

/* Unop */
#define GETN_VUnopV_M(insn)    "RVV0p7_Unop_"#insn"v_m"
#define GETA_VUnopV_M(insn)    RVV0p7_Unop_##insn##v_m
#define GETN_VUnopV(insn)      "RVV0p7_Unop_"#insn"v"
#define GETA_VUnopV(insn)      RVV0p7_Unop_##insn##v

#define GETN_VUnopX_M(insn)    "RVV0p7_Unop_"#insn"x_m"
#define GETA_VUnopX_M(insn)    RVV0p7_Unop_##insn##x_m
#define GETN_VUnopX(insn)      "RVV0p7_Unop_"#insn"x"
#define GETA_VUnopX(insn)      RVV0p7_Unop_##insn##x

#define GETN_VUnopF_M(insn)    "RVV0p7_Unop_"#insn"f_m"
#define GETA_VUnopF_M(insn)    RVV0p7_Unop_##insn##f_m
#define GETN_VUnopF(insn)      "RVV0p7_Unop_"#insn"f"
#define GETA_VUnopF(insn)      RVV0p7_Unop_##insn##f

/*---------------------------------------------------------------*/
/*--- Get variant type of helper functions                    ---*/
/*---------------------------------------------------------------*/

#define GETV_VopUnknow  0
#define GETV_VopAccD    (1 << 0) /* Indicate VD  is readed   by OP, e.g. VFMACC  */
#define GETV_VopWidenD  (1 << 1) /* Indicate VD  is widened  by OP, e.g. VFWADD  */
#define GETV_VopWidenS2 (1 << 2) /* Indicate VS2 is widened  by OP, e.g. VFWADDW/VFNCVT */
#define GETV_VopM1D     (1 << 3) /* Indicate VD  is 1 LMUL   by OP, e.g. VMFEQ   */
#define GETV_VopM1S     (1 << 4) /* Indicate VS* is 1 LMUL   by OP, e.g. VMAND   */

/*---------------------------------------------------------------*/
/*--- Get call of helper functions                            ---*/
/*---------------------------------------------------------------*/

/* Binop */
#define GETC_VBinopOP_T(insn, V, X, I, REGO, REGN, ARGS, VARIANT)              \
   do {                                                                        \
      if (isVOpVV(GET_FUNCT3())) {                                             \
         fName = mask ? GETN_VBinopV##V(insn) : GETN_VBinopV##V##_M(insn);     \
         fAddr = mask ? GETA_VBinopV##V(insn) : GETA_VBinopV##V##_M(insn);     \
         temp  = offsetVReg(rs1);                                              \
      } else if (isVOpVXorVF(GET_FUNCT3())) {                                  \
         fName = mask ? GETN_VBinopV##X(insn) : GETN_VBinopV##X##_M(insn);     \
         fAddr = mask ? GETA_VBinopV##X(insn) : GETA_VBinopV##X##_M(insn);     \
         temp  = REGO(rs1);                                                    \
      } else {                                                                 \
         fName = mask ? GETN_VBinopV##I(insn) : GETN_VBinopV##I##_M(insn);     \
         fAddr = mask ? GETA_VBinopV##I(insn) : GETA_VBinopV##I##_M(insn);     \
         temp  = rs1;                                                          \
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
             isVOpVV(GET_FUNCT3()) ? nameVReg(rs1) : REGN(rs1));               \
   } while (0)

// OPI
#define GETR_VBinopOPI()                                                       \
   args = mkIRExprVec_5(IRExpr_GSPTR(), mkU64(offsetVReg(rd)),                 \
                        mkU64(offsetVReg(rs2)), mkU64(temp),                   \
                        mkU64(offsetVReg(0)));

#define GETC_VBinopOPI(insn)                                                   \
   GETC_VBinopOP_T(insn, V, X, I, offsetIReg64, nameIReg, GETR_VBinopOPI,      \
                   GETV_VopUnknow)

#define GETC_VBinopOPI_V(insn)                                                 \
   GETC_VBinopOP_T(insn, V, V, V, offsetIReg64, nameIReg, GETR_VBinopOPI,      \
                   GETV_VopUnknow)

#define GETC_VBinopOPI_XI(insn)                                                \
   GETC_VBinopOP_T(insn, X, X, I, offsetIReg64, nameIReg, GETR_VBinopOPI,      \
                   GETV_VopUnknow)

#define GETC_VBinopOPI_X(insn)                                                 \
   GETC_VBinopOP_T(insn, X, X, X, offsetIReg64, nameIReg, GETR_VBinopOPI,      \
                   GETV_VopUnknow)

#define GETC_VBinopOPI_V_VAR(insn, vtype)                                      \
   GETC_VBinopOP_T(insn, V, V, V, offsetIReg64, nameIReg, GETR_VBinopOPI,      \
                   vtype)

// SAT
#define GETR_VBinopSAT()                                                       \
   assign(irsb, xrm,                                                           \
          binop(Iop_And32, binop(Iop_Shr32, getFCSR(), mkU8(9)), mkU32(3)));   \
   args = mkIRExprVec_6(IRExpr_GSPTR(), mkU64(offsetVReg(rd)),                 \
                        mkU64(offsetVReg(rs2)), mkU64(temp),                   \
                        mkU64(offsetVReg(0)), mkexpr(xrm));

#define GETC_VBinopSAT(insn)                                                   \
   GETC_VBinopOP_T(insn, V, X, I, offsetIReg64, nameIReg, GETR_VBinopSAT,      \
                   GETV_VopUnknow);                                            \
   accumulateXSAT(irsb, mkexpr(ret));

#define GETC_VBinopSAT_VAR(insn, vtype)                                        \
   GETC_VBinopOP_T(insn, V, X, I, offsetIReg64, nameIReg, GETR_VBinopSAT,      \
                   vtype);                                                     \
   accumulateXSAT(irsb, mkexpr(ret));

#define GETC_VBinopSAT_VX(insn)                                                \
   GETC_VBinopOP_T(insn, V, X, X, offsetIReg64, nameIReg, GETR_VBinopSAT,      \
                   GETV_VopUnknow);                                            \
   accumulateXSAT(irsb, mkexpr(ret));

// SAT2 - used for args format: rd, rs1/imm, rs2
#define GETR_VBinopSAT2()                                                      \
   assign(irsb, xrm,                                                           \
          binop(Iop_And32, binop(Iop_Shr32, getFCSR(), mkU8(9)), mkU32(3)));   \
   args = mkIRExprVec_6(IRExpr_GSPTR(), mkU64(offsetVReg(rd)),                 \
                        mkU64(offsetVReg(temp)), mkU64(rs2),                   \
                        mkU64(offsetVReg(0)), mkexpr(xrm));

#define GETC_VBinopSAT2_VX_VAR(insn, vtype)                                    \
   GETC_VBinopOP_T(insn, V, X, X, offsetIReg64, nameIReg, GETR_VBinopSAT2,     \
                   vtype);                                                     \
   accumulateXSAT(irsb, mkexpr(ret));

#define GETC_VBinopSAT2_X_VAR(insn, vtype)                                     \
   GETC_VBinopOP_T(insn, X, X, X, offsetIReg64, nameIReg, GETR_VBinopSAT2,     \
                   vtype);                                                     \
   accumulateXSAT(irsb, mkexpr(ret));

// OPF - used for args format: rd, rs2, rs1/imm
#define GETR_VBinopOPF()                                                       \
   assign(irsb, frm,                                                           \
          binop(Iop_And32, binop(Iop_Shr32, getFCSR(), mkU8(5)), mkU32(7)));   \
   args = mkIRExprVec_6(IRExpr_GSPTR(), mkU64(offsetVReg(rd)),                 \
                        mkU64(offsetVReg(rs2)), mkU64(temp),                   \
                        mkU64(offsetVReg(0)), mkexpr(frm));

#define GETC_VBinopOPF(insn)                                                   \
   GETC_VBinopOP_T(insn, V, F, F, offsetFReg, nameFReg, GETR_VBinopOPF,        \
                   GETV_VopUnknow);                                            \
   accumulateFFLAGS(irsb, mkexpr(ret));

#define GETC_VBinopOPF_VAR(insn, vtype)                                        \
   GETC_VBinopOP_T(insn, V, F, F, offsetFReg, nameFReg, GETR_VBinopOPF,        \
                   vtype);                                                     \
   accumulateFFLAGS(irsb, mkexpr(ret));

#define GETC_VBinopOPF_V_VAR(insn, vtype)                                      \
   GETC_VBinopOP_T(insn, V, V, V, offsetFReg, nameFReg, GETR_VBinopOPF,        \
                   vtype);                                                     \
   accumulateFFLAGS(irsb, mkexpr(ret));

#define GETC_VBinopOPF_F(insn)                                                 \
   GETC_VBinopOP_T(insn, F, F, F, offsetFReg, nameFReg, GETR_VBinopOPF,        \
                   GETV_VopUnknow);                                            \
   accumulateFFLAGS(irsb, mkexpr(ret));

#define GETC_VBinopOPF_F_VAR(insn, vtype)                                      \
   GETC_VBinopOP_T(insn, F, F, F, offsetFReg, nameFReg, GETR_VBinopOPF,        \
                   vtype);                                                     \
   accumulateFFLAGS(irsb, mkexpr(ret));

// OPF2 - used for args format: rd, rs1/imm, rs2
#define GETR_VBinopOPF2()                                                      \
   assign(irsb, frm,                                                           \
          binop(Iop_And32, binop(Iop_Shr32, getFCSR(), mkU8(5)), mkU32(7)));   \
   args = mkIRExprVec_6(IRExpr_GSPTR(), mkU64(offsetVReg(rd)),                 \
                        mkU64(temp), mkU64(offsetVReg(rs2)),                   \
                        mkU64(offsetVReg(0)), mkexpr(frm));

#define GETC_VBinopOPF2(insn, vtype)                                           \
   GETC_VBinopOP_T(insn, V, F, F, offsetFReg, nameFReg, GETR_VBinopOPF2,       \
                   vtype);                                                     \
   accumulateFFLAGS(irsb, mkexpr(ret));

/* Unop */
#define GETC_VUnopOP_T(insn, V, X, REGO, REGN, ARGS, VARIANT)                  \
   do {                                                                        \
      if (isVOpVV(GET_FUNCT3())) {                                             \
         fName = mask ? GETN_VUnop##V(insn) : GETN_VUnop##V##_M(insn);         \
         fAddr = mask ? GETA_VUnop##V(insn) : GETA_VUnop##V##_M(insn);         \
         temp  = offsetVReg(rs2);                                              \
      } else {                                                                 \
         fName = mask ? GETN_VUnop##X(insn) : GETN_VUnop##X##_M(insn);         \
         fAddr = mask ? GETA_VUnop##X(insn) : GETA_VUnop##X##_M(insn);         \
         temp  = REGO(rs1);                                                    \
      }                                                                        \
      vassert(fAddr != NULL);                                                  \
      ARGS()                                                                   \
      d = unsafeIRDirty_1_N(ret, 0, fName, fAddr, args);                       \
      d = GETD_VUnop(d, rd, isVOpVV(GET_FUNCT3()) ? rs2 : rs1, mask,           \
                     GET_FUNCT3(), VARIANT);                                   \
      stmt(irsb, IRStmt_Dirty(d));                                             \
                                                                               \
      DIP("%s(%s, %s)\n", fName, nameVReg(rd),                                 \
          isVOpVV(GET_FUNCT3()) ? nameVReg(rs2) : REGN(rs1));                  \
   } while (0)

// OPI
#define GETR_VUnopOPI()                                                        \
   args = mkIRExprVec_4(IRExpr_GSPTR(), mkU64(offsetVReg(rd)), mkU64(temp),    \
                        mkU64(offsetVReg(0)));

#define GETC_VUnopOPI_V_VAR(insn, vtype)                                       \
   GETC_VUnopOP_T(insn, V, V, offsetIReg64, nameIReg, GETR_VUnopOPI,           \
                  vtype);

#define GETC_VUnopOPI_X_VAR(insn, vtype)                                       \
   GETC_VUnopOP_T(insn, X, X, offsetIReg64, nameIReg, GETR_VUnopOPI,           \
                  vtype);

// OPF
#define GETR_VUnopOPF()                                                        \
   assign(irsb, frm,                                                           \
          binop(Iop_And32, binop(Iop_Shr32, getFCSR(), mkU8(5)), mkU32(7)));   \
   args = mkIRExprVec_5(IRExpr_GSPTR(), mkU64(offsetVReg(rd)), mkU64(temp),    \
                        mkU64(offsetVReg(0)), mkexpr(frm));

#define GETC_VUnopOPF_V(insn)                                                  \
   GETC_VUnopOP_T(insn, V, V, offsetFReg, nameFReg, GETR_VUnopOPF,             \
                  GETV_VopUnknow);                                             \
   accumulateFFLAGS(irsb, mkexpr(ret));

#define GETC_VUnopOPF_F(insn)                                                  \
   GETC_VUnopOP_T(insn, F, F, offsetFReg, nameFReg, GETR_VUnopOPF,             \
                  GETV_VopUnknow);                                             \
   accumulateFFLAGS(irsb, mkexpr(ret));

#define GETC_VUnopOPF_V_VAR(insn, vtype)                                       \
   GETC_VUnopOP_T(insn, V, V, offsetFReg, nameFReg, GETR_VUnopOPF, vtype);     \
   accumulateFFLAGS(irsb, mkexpr(ret));

#define GETC_VUnopOPF_F_VAR(insn, vtype)                                       \
   GETC_VUnopOP_T(insn, F, F, offsetFReg, nameFReg, GETR_VUnopOPF, vtype);     \
   accumulateFFLAGS(irsb, mkexpr(ret));

/*---------------------------------------------------------------*/
/*--- Get dirty info of helper functions                      ---*/
/*---------------------------------------------------------------*/

/* Binop */
static IRDirty*
GETD_VBinop(IRDirty* d, UInt vd, UInt vs2, UInt vs1, Bool mask, UInt sopc, UInt vtype)
{
   /* TODO */
   UInt lmul   = 0;
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
      d->fxState[i].nRepeats  = lmuls[i];
      d->fxState[i].repeatLen = host_VLENB;
   }

   if (isVOpVXorVF(sopc)) {
      d->fxState[d->nFxState].fx     = Ifx_Read;
      d->fxState[d->nFxState].offset = sopc == RV64_SOPC_OPFVF ? offsetFReg(vs1) : offsetIReg64(vs1);
      d->fxState[d->nFxState].size   = 8;
      d->nFxState += 1;
   }

   if (!mask) {
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
   /* TODO */
   UInt lmul   = 0;
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
      d->fxState[i].nRepeats  = lmuls[i];
      d->fxState[i].repeatLen = host_VLENB;
   }

   if (isVOpVXorVF(sopc)) {
      d->fxState[d->nFxState].fx     = Ifx_Read;
      d->fxState[d->nFxState].offset = sopc == RV64_SOPC_OPFVF ? offsetFReg(src) : offsetIReg64(src);
      d->fxState[d->nFxState].size   = 8;
      d->nFxState += 1;
   }

   if (!mask) {
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

// Push vl/vtype
// Set whole register du to current vtype
#define RVV0p7_Push()         \
   __asm__ __volatile__(      \
      "csrr\tt1,vl\n\t"       \
      "csrr\tt2,vtype\n\t"    \
      "vsetvl\tx0,x0,t2\n\t"  \
      ::: "t1", "t2");

// Pop vl/vtype
#define RVV0p7_Pop()          \
   __asm__ __volatile__(      \
      "vsetvl\tx0,t1,t2\n\t"  \
      :::);

// Push vl/vtype
// Set whole register with widened LMUL of current vtype
#define RVV0p7_PushW()        \
   __asm__ __volatile__(      \
      "csrr\tt1,vl\n\t"       \
      "csrr\tt2,vtype\n\t"    \
      "addi\tt0,t2,1\n\t"     \
      "vsetvl\tx0,x0,t0\n\t"  \
      ::: "t0", "t1", "t2");

// Push vl/vtype
// Set whole register with M1 LMUL
#define RVV0p7_PushM1()             \
   __asm__ __volatile__(            \
      "csrr\tt1,vl\n\t"             \
      "csrr\tt2,vtype\n\t"          \
      "vsetvli\tx0,x0,e8,m1\n\t"    \
      ::: "t1", "t2");

// Get mask
#define RVV0p7_Mask()               \
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
#define RVV0p7_VX() \
   __asm__ __volatile__("ld\tt0,0(%0)\n\t"::"r"(rs1):"t0");
#define RVV0p7_VI() \
   __asm__ __volatile__("mv\tt0,%0\n\t"::"r"(rs1):"t0");
#define RVV0p7_VF() \
   __asm__ __volatile__("fld\tft0,0(%0)\n\t"::"r"(rs1):"ft0");

// Push fcsr
// Set frm
#define RVV0p7_PushFCSR()           \
   __asm__ __volatile__(            \
      "csrr\tt1,fcsr\n\t"           \
      "csrw\tfrm,%0\n\t"            \
      "csrw\tfflags,x0\n\t"         \
      :                             \
      : "r"(frm)                    \
      : "t1");
// Get fflags
// Pop fcsr
#define RVV0p7_PopFCSR()            \
   __asm__ __volatile__(            \
      "csrr\t%0,fflags\n\t"         \
      "csrw\tfcsr,t1\n\t"           \
      : "=r"(ret)                   \
      :                             \
      :);

// Push fcsr
// Set xrm
#define RVV0p7_PushXSAT()           \
   __asm__ __volatile__(            \
      "csrr\tt1,fcsr\n\t"           \
      "csrw\tvxrm,%0\n\t"           \
      "csrw\tvxsat,x0\n\t"          \
      :                             \
      : "r"(xrm)                    \
      : "t1");
// Get xsat
// Pop fcsr
#define RVV0p7_PopXSAT()            \
   __asm__ __volatile__(            \
      "csrr\t%0,vxsat\n\t"          \
      "csrw\tfcsr,t1\n\t"           \
      : "=r"(ret)                   \
      :                             \
      :);

/*---------------------------------------------------------------*/
/*--- VV Instruction templates                                ---*/
/*---------------------------------------------------------------*/

// v8-v15, v16-v23, v24-v31
#define RVV0p7_BinopVV_M_PP_T(insn, vd, vs2, vs1, imask, mreg, ipush, ipop, ipre, ipost)\
   do {                                                     \
      UInt ret = 0;                                         \
                                                            \
      vd  += (ULong)st;                                     \
      vs2 += (ULong)st;                                     \
      vs1 += (ULong)st;                                     \
      mask += (ULong)st;                                    \
                                                            \
      ipush                                                 \
      __asm__ __volatile__(                                 \
         "vle.v\tv8,(%0)\n\t"                               \
         "vle.v\tv16,(%1)\n\t"                              \
         "vle.v\tv24,(%2)\n\t"                              \
         :                                                  \
         : "r"(vd), "r"(vs2), "r"(vs1)                      \
         :);                                                \
      ipop                                                  \
                                                            \
      imask                                                 \
      ipre                                                  \
      __asm__ __volatile__(insn "\tv8,v16,v24" mreg);       \
      ipost                                                 \
                                                            \
      ipush                                                 \
      __asm__ __volatile__(                                 \
         "vse.v\tv8,(%0)\n\t"                               \
         :                                                  \
         : "r"(vd)                                          \
         : "memory");                                       \
      ipop                                                  \
                                                            \
      return ret;                                           \
   } while (0)

// v8-v15, v16-v23, v24-v31 # used for Narrowing
#define RVV0p7_BinopVV_M_PP_TN(insn, vd, vs2, vs1, imask, mreg, ipush, ipop, ipre, ipost)\
   do {                                                     \
      UInt ret = 0;                                         \
                                                            \
      vd  += (ULong)st;                                     \
      vs2 += (ULong)st;                                     \
      vs1 += (ULong)st;                                     \
      mask += (ULong)st;                                    \
                                                            \
      ipush                                                 \
      __asm__ __volatile__(                                 \
         "vle.v\tv16,(%0)\n\t"                              \
         :                                                  \
         : "r"(vs2)                                         \
         :);                                                \
      ipop                                                  \
                                                            \
      __asm__ __volatile__(                                 \
         "vle.v\tv24,(%0)\n\t"                              \
         :                                                  \
         : "r"(vs1)                                         \
         :);                                                \
                                                            \
      imask                                                 \
      ipre                                                  \
      __asm__ __volatile__(insn "\tv8,v16,v24" mreg);       \
      ipost                                                 \
                                                            \
      __asm__ __volatile__(                                 \
         "vse.v\tv8,(%0)\n\t"                               \
         :                                                  \
         : "r"(vd)                                          \
         : "memory");                                       \
                                                            \
      return ret;                                           \
   } while (0)

// v8, v16-v23, v24-v31 # used for VD is 1 LMUL, e.g. compared
#define RVV0p7_BinopVV_M_M1D_T(insn, vd, vs2, vs1, imask, mreg, ipush, ipop, ipre, ipost)\
   do {                                                     \
      UInt ret = 0;                                         \
                                                            \
      vd  += (ULong)st;                                     \
      vs2 += (ULong)st;                                     \
      vs1 += (ULong)st;                                     \
      mask += (ULong)st;                                    \
                                                            \
      __asm__ __volatile__(                                 \
         "csrr\tt1,vl\n\t"                                  \
         "csrr\tt2,vtype\n\t"                               \
         "vsetvli\tx0,x0,e8,m1\n\t"                         \
         "vle.v\tv8,(%0)\n\t"                               \
         "vsetvl\tx0,t1,t2\n\t"                             \
         :                                                  \
         : "r"(vd)                                          \
         : "t1", "t2");                                     \
                                                            \
      ipush                                                 \
      __asm__ __volatile__(                                 \
         "vle.v\tv16,(%0)\n\t"                              \
         "vle.v\tv24,(%1)\n\t"                              \
         :                                                  \
         : "r"(vs2), "r"(vs1)                               \
         :);                                                \
      ipop                                                  \
                                                            \
      imask                                                 \
      ipre                                                  \
      __asm__ __volatile__(insn "\tv8,v16,v24" mreg);       \
      ipost                                                 \
                                                            \
      __asm__ __volatile__(                                 \
         "csrr\tt1,vl\n\t"                                  \
         "csrr\tt2,vtype\n\t"                               \
         "vsetvli\tx0,x0,e8,m1\n\t"                         \
         "vse.v\tv8,(%0)\n\t"                               \
         "vsetvl\tx0,t1,t2\n\t"                             \
         :                                                  \
         : "r"(vd)                                          \
         : "t1", "t2", "memory");                           \
                                                            \
      return ret;                                           \
   } while (0)

// v8-v15, v16-v23
#define RVV0p7_UnopV_M_PP_T(insn, vd, vs2, imask, mreg, ipush, ipop, ipre, ipost)\
   do {                                                     \
      UInt ret = 0;                                         \
                                                            \
      vd  += (ULong)st;                                     \
      vs2 += (ULong)st;                                     \
      mask += (ULong)st;                                    \
                                                            \
      __asm__ __volatile__(                                 \
         "vle.v\tv16,(%0)\n\t"                              \
         :                                                  \
         : "r"(vs2)                                         \
         :);                                                \
                                                            \
      imask                                                 \
      ipre                                                  \
      __asm__ __volatile__(insn "\tv8,v16" mreg);           \
      ipost                                                 \
                                                            \
      ipush                                                 \
      __asm__ __volatile__(                                 \
         "vse.v\tv8,(%0)\n\t"                               \
         :                                                  \
         : "r"(vd)                                          \
         : "memory");                                       \
      ipop                                                  \
                                                            \
      return ret;                                           \
   } while (0)

// v8-v15, v16-v23 # used for Narrowing
#define RVV0p7_UnopV_M_PPS_T(insn, vd, vs2, imask, mreg, ipush, ipop, ipre, ipost)\
   do {                                                     \
      UInt ret = 0;                                         \
                                                            \
      vd  += (ULong)st;                                     \
      vs2 += (ULong)st;                                     \
      mask += (ULong)st;                                    \
                                                            \
      ipush                                                 \
      __asm__ __volatile__(                                 \
         "vle.v\tv16,(%0)\n\t"                              \
         :                                                  \
         : "r"(vs2)                                         \
         :);                                                \
      ipop                                                  \
                                                            \
      imask                                                 \
      ipre                                                  \
      __asm__ __volatile__(insn "\tv8,v16" mreg);           \
      ipost                                                 \
                                                            \
      __asm__ __volatile__(                                 \
         "vse.v\tv8,(%0)\n\t"                               \
         :                                                  \
         : "r"(vd)                                          \
         : "memory");                                       \
                                                            \
      return ret;                                           \
   } while (0)

/*---------------------------------------------------------------*/
/*--- OPIVV/OPFVV Instruction templates helper                ---*/
/*---------------------------------------------------------------*/

/* Binop */
// OPI
#define RVV0p7_BinopOPIVV_M_T(insn, vd, vs2, vs1)\
   RVV0p7_BinopVV_M_PP_T(insn, vd, vs2, vs1, RVV0p7_Mask(), ",v0.t", , , , )
#define RVV0p7_BinopOPIVV_T(insn, vd, vs2, vs1)\
   RVV0p7_BinopVV_M_PP_T(insn, vd, vs2, vs1, , , , , , )

#define RVV0p7_BinopOPIVV_M_P_T(insn, vd, vs2, vs1)\
   RVV0p7_BinopVV_M_PP_T(insn, vd, vs2, vs1, RVV0p7_Mask(), ",v0.t", RVV0p7_Push(), RVV0p7_Pop(), , )
#define RVV0p7_BinopOPIVV_P_T(insn, vd, vs2, vs1)\
   RVV0p7_BinopVV_M_PP_T(insn, vd, vs2, vs1, , , RVV0p7_Push(), RVV0p7_Pop(), , )

#define RVV0p7_BinopOPIVV_M_M1D_P_T(insn, vd, vs2, vs1)\
   RVV0p7_BinopVV_M_M1D_T(insn, vd, vs2, vs1, RVV0p7_Mask(), ",v0.t", RVV0p7_Push(), RVV0p7_Pop(), , )
#define RVV0p7_BinopOPIVV_M1D_P_T(insn, vd, vs2, vs1)\
   RVV0p7_BinopVV_M_M1D_T(insn, vd, vs2, vs1, , , RVV0p7_Push(), RVV0p7_Pop(), , )

// SAT
#define RVV0p7_BinopSATVV_M_T(insn, vd, vs2, vs1)\
   RVV0p7_BinopVV_M_PP_T(insn, vd, vs2, vs1, RVV0p7_Mask(), ",v0.t", , , RVV0p7_PushXSAT(), RVV0p7_PopXSAT())
#define RVV0p7_BinopSATVV_T(insn, vd, vs2, vs1)\
   RVV0p7_BinopVV_M_PP_T(insn, vd, vs2, vs1, , , , , RVV0p7_PushXSAT(), RVV0p7_PopXSAT())

#define RVV0p7_BinopSATWVV_M_T(insn, vd, vs2, vs1)                             \
   RVV0p7_BinopVV_M_PP_T(insn, vd, vs2, vs1, RVV0p7_Mask(), ",v0.t",           \
                         RVV0p7_PushW(), RVV0p7_Pop(), RVV0p7_PushXSAT(),      \
                         RVV0p7_PopXSAT())
#define RVV0p7_BinopSATWVV_T(insn, vd, vs2, vs1)                               \
   RVV0p7_BinopVV_M_PP_T(insn, vd, vs2, vs1, , , RVV0p7_PushW(), RVV0p7_Pop(), \
                         RVV0p7_PushXSAT(), RVV0p7_PopXSAT())

#define RVV0p7_BinopSATNVV_M_T(insn, vd, vs2, vs1)                             \
   RVV0p7_BinopVV_M_PP_TN(insn, vd, vs2, vs1, RVV0p7_Mask(), ",v0.t",           \
                          RVV0p7_PushW(), RVV0p7_Pop(), RVV0p7_PushXSAT(),      \
                          RVV0p7_PopXSAT())
#define RVV0p7_BinopSATNVV_T(insn, vd, vs2, vs1)                               \
   RVV0p7_BinopVV_M_PP_TN(insn, vd, vs2, vs1, , , RVV0p7_PushW(), RVV0p7_Pop(), \
                          RVV0p7_PushXSAT(), RVV0p7_PopXSAT())

// MASK
#define RVV0p7_BinopOPIMM_T(insn, vd, vs2, vs1)\
   RVV0p7_BinopVV_M_PP_T(insn, vd, vs2, vs1, , , RVV0p7_PushM1(), RVV0p7_Pop(), , )

#define RVV0p7_UnopOPIM_M_T(insn, vd, vs2)\
   RVV0p7_UnopV_M_PP_T(insn, vd, vs2, RVV0p7_Mask(), ",v0.t", RVV0p7_PushM1(), RVV0p7_Pop(), , )

#define RVV0p7_UnopOPIM_M_M1S_T(insn, vd, vs2)\
   RVV0p7_UnopV_M_PPS_T(insn, vd, vs2, RVV0p7_Mask(), ",v0.t", RVV0p7_PushM1(), RVV0p7_Pop(), , )

// OPF
#define RVV0p7_BinopOPFVV_M_T(insn, vd, vs2, vs1)\
   RVV0p7_BinopVV_M_PP_T(insn, vd, vs2, vs1, RVV0p7_Mask(), ",v0.t", , , RVV0p7_PushFCSR(), RVV0p7_PopFCSR())
#define RVV0p7_BinopOPFVV_T(insn, vd, vs2, vs1)\
   RVV0p7_BinopVV_M_PP_T(insn, vd, vs2, vs1, , , , , RVV0p7_PushFCSR(), RVV0p7_PopFCSR())

#define RVV0p7_BinopOPFWVV_M_T(insn, vd, vs2, vs1)                             \
   RVV0p7_BinopVV_M_PP_T(insn, vd, vs2, vs1, RVV0p7_Mask(), ",v0.t",           \
                         RVV0p7_PushW(), RVV0p7_Pop(), RVV0p7_PushFCSR(),      \
                         RVV0p7_PopFCSR())
#define RVV0p7_BinopOPFWVV_T(insn, vd, vs2, vs1)                               \
   RVV0p7_BinopVV_M_PP_T(insn, vd, vs2, vs1, , , RVV0p7_PushW(), RVV0p7_Pop(), \
                         RVV0p7_PushFCSR(), RVV0p7_PopFCSR())

#define RVV0p7_BinopOPFVV_M_CMP(insn, vd, vs2, vs1)\
   RVV0p7_BinopVV_M_M1D_T(insn, vd, vs2, vs1, RVV0p7_Mask(), ",v0.t", , , RVV0p7_PushFCSR(), RVV0p7_PopFCSR())
#define RVV0p7_BinopOPFVV_CMP(insn, vd, vs2, vs1)\
   RVV0p7_BinopVV_M_M1D_T(insn, vd, vs2, vs1, , , , , RVV0p7_PushFCSR(), RVV0p7_PopFCSR())

#define RVV0p7_BinopOPFVV_M_M1D_P_T(insn, vd, vs2, vs1)\
   RVV0p7_BinopVV_M_M1D_T(insn, vd, vs2, vs1, RVV0p7_Mask(), ",v0.t", RVV0p7_Push(), RVV0p7_Pop(), RVV0p7_PushFCSR(), RVV0p7_PopFCSR())
#define RVV0p7_BinopOPFVV_M1D_P_T(insn, vd, vs2, vs1)\
   RVV0p7_BinopVV_M_M1D_T(insn, vd, vs2, vs1, , , RVV0p7_Push(), RVV0p7_Pop(), RVV0p7_PushFCSR(), RVV0p7_PopFCSR())

/* Unop */
// OPF
#define RVV0p7_UnopOPFV_M_T(insn, vd, vs2)\
   RVV0p7_UnopV_M_PP_T(insn, vd, vs2, RVV0p7_Mask(), ",v0.t", , , RVV0p7_PushFCSR(), RVV0p7_PopFCSR())
#define RVV0p7_UnopOPFV_T(insn, vd, vs2)\
   RVV0p7_UnopV_M_PP_T(insn, vd, vs2, , , , , RVV0p7_PushFCSR(), RVV0p7_PopFCSR())

#define RVV0p7_UnopOPFWV_M_T(insn, vd, vs2)                                    \
   RVV0p7_UnopV_M_PP_T(insn, vd, vs2, RVV0p7_Mask(), ",v0.t", RVV0p7_PushW(),  \
                       RVV0p7_Pop(), RVV0p7_PushFCSR(), RVV0p7_PopFCSR())
#define RVV0p7_UnopOPFWV_T(insn, vd, vs2)                                      \
   RVV0p7_UnopV_M_PP_T(insn, vd, vs2, , , RVV0p7_PushW(), RVV0p7_Pop(),        \
                       RVV0p7_PushFCSR(), RVV0p7_PopFCSR())

#define RVV0p7_UnopOPFNV_M_T(insn, vd, vs2)                                     \
   RVV0p7_UnopV_M_PPS_T(insn, vd, vs2, RVV0p7_Mask(), ",v0.t", RVV0p7_PushW(),  \
                       RVV0p7_Pop(), RVV0p7_PushFCSR(), RVV0p7_PopFCSR())
#define RVV0p7_UnopOPFNV_T(insn, vd, vs2)                                       \
   RVV0p7_UnopV_M_PPS_T(insn, vd, vs2, , , RVV0p7_PushW(), RVV0p7_Pop(),        \
                       RVV0p7_PushFCSR(), RVV0p7_PopFCSR())

/*---------------------------------------------------------------*/
/*--- VX/VI/VF Instruction templates                          ---*/
/*---------------------------------------------------------------*/

// v8-v15, v16-v23, t0/ft0
#define RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, imask, mreg, ipush, ipop, isopc, treg, ipre, ipost)\
   do {                                                     \
      UInt ret = 0;                                         \
                                                            \
      vd  += (ULong)st;                                     \
      vs2 += (ULong)st;                                     \
      mask += (ULong)st;                                    \
                                                            \
      ipush                                                 \
      __asm__ __volatile__(                                 \
         "vle.v\tv8,(%0)\n\t"                               \
         "vle.v\tv16,(%1)\n\t"                              \
         :                                                  \
         : "r"(vd), "r"(vs2)                                \
         :);                                                \
      ipop                                                  \
                                                            \
      imask                                                 \
      ipre                                                  \
      isopc                                                 \
      __asm__ __volatile__(insn "\tv8,v16," treg mreg);     \
      ipost                                                 \
                                                            \
      ipush                                                 \
      __asm__ __volatile__(                                 \
         "vse.v\tv8,(%0)\n\t"                               \
         :                                                  \
         : "r"(vd)                                          \
         : "memory");                                       \
      ipop                                                  \
                                                            \
      return ret;                                           \
   } while (0)

// v8-v15, t0/ft0, v16-v23
#define RVV0p7_BinopVX_VI_VF_M_PP_T2(insn, vd, vs2, rs1, imask, mreg, ipush, ipop, isopc, treg, ipre, ipost)\
   do {                                                     \
      UInt ret = 0;                                         \
                                                            \
      vd  += (ULong)st;                                     \
      vs2 += (ULong)st;                                     \
      mask += (ULong)st;                                    \
                                                            \
      ipush                                                 \
      __asm__ __volatile__(                                 \
         "vle.v\tv8,(%0)\n\t"                               \
         "vle.v\tv16,(%1)\n\t"                              \
         :                                                  \
         : "r"(vd), "r"(vs2)                                \
         :);                                                \
      ipop                                                  \
                                                            \
      imask                                                 \
      ipre                                                  \
      isopc                                                 \
      __asm__ __volatile__(insn "\tv8," treg ",v16" mreg);  \
      ipost                                                 \
                                                            \
      ipush                                                 \
      __asm__ __volatile__(                                 \
         "vse.v\tv8,(%0)\n\t"                               \
         :                                                  \
         : "r"(vd)                                          \
         : "memory");                                       \
      ipop                                                  \
                                                            \
      return ret;                                           \
   } while (0)

// v8-v15, v16-v23, t0/ft0 # used for Narrowing
#define RVV0p7_BinopVX_VI_VF_M_PP_TN(insn, vd, vs2, rs1, imask, mreg, ipush, ipop, isopc, treg, ipre, ipost)\
   do {                                                     \
      UInt ret = 0;                                         \
                                                            \
      vd  += (ULong)st;                                     \
      vs2 += (ULong)st;                                     \
      mask += (ULong)st;                                    \
                                                            \
      ipush                                                 \
      __asm__ __volatile__(                                 \
         "vle.v\tv16,(%0)\n\t"                              \
         :                                                  \
         : "r"(vs2)                                         \
         :);                                                \
      ipop                                                  \
                                                            \
      imask                                                 \
      ipre                                                  \
      isopc                                                 \
      __asm__ __volatile__(insn "\tv8,v16," treg mreg);     \
      ipost                                                 \
                                                            \
      __asm__ __volatile__(                                 \
         "vse.v\tv8,(%0)\n\t"                               \
         :                                                  \
         : "r"(vd)                                          \
         : "memory");                                       \
                                                            \
      return ret;                                           \
   } while (0)

// v8, v16-v23, t0/ft0 # used for compared
#define RVV0p7_BinopVX_VI_VF_M_CMP_T(insn, vd, vs2, rs1, imask, mreg, isopc, treg, ipre, ipost)\
   do {                                                     \
      UInt ret = 0;                                         \
                                                            \
      vd  += (ULong)st;                                     \
      vs2 += (ULong)st;                                     \
      mask += (ULong)st;                                    \
                                                            \
      __asm__ __volatile__(                                 \
         "csrr\tt1,vl\n\t"                                  \
         "csrr\tt2,vtype\n\t"                               \
         "vsetvli\tx0,x0,e8,m1\n\t"                         \
         "vle.v\tv8,(%0)\n\t"                               \
         "vsetvl\tx0,t1,t2\n\t"                             \
         :                                                  \
         : "r"(vd)                                          \
         : "t1", "t2");                                     \
                                                            \
      __asm__ __volatile__(                                 \
         "vle.v\tv16,(%0)\n\t"                              \
         :                                                  \
         : "r"(vs2)                                         \
         :);                                                \
                                                            \
      imask                                                 \
      ipre                                                  \
      isopc                                                 \
      __asm__ __volatile__(insn "\tv8,v16," treg mreg);     \
      ipost                                                 \
                                                            \
      __asm__ __volatile__(                                 \
         "csrr\tt1,vl\n\t"                                  \
         "csrr\tt2,vtype\n\t"                               \
         "vsetvli\tx0,x0,e8,m1\n\t"                         \
         "vse.v\tv8,(%0)\n\t"                               \
         "vsetvl\tx0,t1,t2\n\t"                             \
         :                                                  \
         : "r"(vd)                                          \
         : "t1", "t2", "memory");                           \
                                                            \
      return ret;                                           \
   } while (0)

// ret, v16, t0/ft0
#define RVV0p7_BinopRVX_VF_T(insn, vs2, rs1, isopc, treg, oreg)\
   do {                                                     \
      vs2 += (ULong)st;                                     \
      rs1 += (ULong)st;                                     \
                                                            \
      __asm__ __volatile__(                                 \
         "csrr\tt1,vl\n\t"                                  \
         "csrr\tt2,vtype\n\t"                               \
         "vsetvli\tx0,x0,e8,m1\n\t"                         \
         "vle.v\tv16,(%0)\n\t"                              \
         "vsetvl\tx0,t1,t2\n\t"                             \
         :                                                  \
         : "r"(vs2)                                         \
         : "t1", "t2");                                     \
                                                            \
      isopc                                                 \
      __asm__ __volatile__(insn "\t%0,v16" treg             \
                           :"=" oreg (ret)::);              \
                                                            \
      return ret;                                           \
   } while (0)

// v8-v15, t0/ft0
#define RVV0p7_UnopX_F_M_PP_T(insn, vd, rs1, imask, mreg, ipush, ipop, isopc, treg, ipre, ipost)\
   do {                                                     \
      UInt ret = 0;                                         \
                                                            \
      vd  += (ULong)st;                                     \
      rs1 += (ULong)st;                                     \
      mask += (ULong)st;                                    \
                                                            \
      ipush                                                 \
      __asm__ __volatile__(                                 \
         "vle.v\tv8,(%0)\n\t"                               \
         :                                                  \
         : "r"(vd)                                          \
         :);                                                \
      ipop                                                  \
                                                            \
      imask                                                 \
      ipre                                                  \
      isopc                                                 \
      __asm__ __volatile__(insn "\tv8," treg mreg);         \
      ipost                                                 \
                                                            \
      ipush                                                 \
      __asm__ __volatile__(                                 \
         "vse.v\tv8,(%0)\n\t"                               \
         :                                                  \
         : "r"(vd)                                          \
         : "memory");                                       \
      ipop                                                  \
                                                            \
      return ret;                                           \
   } while (0)

/*---------------------------------------------------------------*/
/*--- VX/VI/VF Instruction templates helper                   ---*/
/*---------------------------------------------------------------*/

/* Binop */
// OPI
#define RVV0p7_BinopVX_M_T(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, RVV0p7_Mask(), ",v0.t", , , RVV0p7_VX(), "t0", , )
#define RVV0p7_BinopVX_T(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, , , , , RVV0p7_VX(), "t0", , )

#define RVV0p7_BinopVI_M_T(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, RVV0p7_Mask(), ",v0.t", , , RVV0p7_VI(), "t0", , )
#define RVV0p7_BinopVI_T(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, , , , , RVV0p7_VI(), "t0", , )

#define RVV0p7_BinopVX_M_P_T(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, RVV0p7_Mask(), ",v0.t", RVV0p7_Push(), RVV0p7_Pop(), RVV0p7_VX(), "t0", , )
#define RVV0p7_BinopVX_P_T(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, , , RVV0p7_Push(), RVV0p7_Pop(), RVV0p7_VX(), "t0", , )

#define RVV0p7_BinopVI_M_P_T(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, RVV0p7_Mask(), ",v0.t", RVV0p7_Push(), RVV0p7_Pop(), RVV0p7_VI(), "t0", , )
#define RVV0p7_BinopVI_P_T(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, , , RVV0p7_Push(), RVV0p7_Pop(), RVV0p7_VI(), "t0", , )

// SAT
#define RVV0p7_BinopSATVX_M_T(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, RVV0p7_Mask(), ",v0.t", , , RVV0p7_VX(), "t0", RVV0p7_PushXSAT(), RVV0p7_PopXSAT())
#define RVV0p7_BinopSATVX_T(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, , , , , RVV0p7_VX(), "t0", RVV0p7_PushXSAT(), RVV0p7_PopXSAT())

#define RVV0p7_BinopSATVI_M_T(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, RVV0p7_Mask(), ",v0.t", , , RVV0p7_VI(), "t0", RVV0p7_PushXSAT(), RVV0p7_PopXSAT())
#define RVV0p7_BinopSATVI_T(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, , , , , RVV0p7_VI(), "t0", RVV0p7_PushXSAT(), RVV0p7_PopXSAT())

#define RVV0p7_BinopSATWVX_M_T2(insn, vd, vs2, rs1)                            \
   RVV0p7_BinopVX_VI_VF_M_PP_T2(insn, vd, vs2, rs1, RVV0p7_Mask(), ",v0.t",    \
                                RVV0p7_PushW(), RVV0p7_Pop(), RVV0p7_VX(),     \
                                "t0", RVV0p7_PushXSAT(), RVV0p7_PopXSAT())
#define RVV0p7_BinopSATWVX_T2(insn, vd, vs2, rs1)                              \
   RVV0p7_BinopVX_VI_VF_M_PP_T2(insn, vd, vs2, rs1, , , RVV0p7_PushW(),        \
                                RVV0p7_Pop(), RVV0p7_VX(), "t0",               \
                                RVV0p7_PushXSAT(), RVV0p7_PopXSAT())

#define RVV0p7_BinopSATNVX_M_T(insn, vd, vs2, rs1)                             \
   RVV0p7_BinopVX_VI_VF_M_PP_TN(insn, vd, vs2, rs1, RVV0p7_Mask(), ",v0.t",    \
                                RVV0p7_PushW(), RVV0p7_Pop(), RVV0p7_VX(),     \
                                "t0", RVV0p7_PushXSAT(), RVV0p7_PopXSAT())
#define RVV0p7_BinopSATNVX_T(insn, vd, vs2, rs1)                               \
   RVV0p7_BinopVX_VI_VF_M_PP_TN(insn, vd, vs2, rs1, , , RVV0p7_PushW(),        \
                                RVV0p7_Pop(), RVV0p7_VX(), "t0",               \
                                RVV0p7_PushXSAT(), RVV0p7_PopXSAT())

#define RVV0p7_BinopSATNVI_M_T(insn, vd, vs2, rs1)                             \
   RVV0p7_BinopVX_VI_VF_M_PP_TN(insn, vd, vs2, rs1, RVV0p7_Mask(), ",v0.t",    \
                                RVV0p7_PushW(), RVV0p7_Pop(), RVV0p7_VI(),     \
                                "t0", RVV0p7_PushXSAT(), RVV0p7_PopXSAT())
#define RVV0p7_BinopSATNVI_T(insn, vd, vs2, rs1)                               \
   RVV0p7_BinopVX_VI_VF_M_PP_TN(insn, vd, vs2, rs1, , , RVV0p7_PushW(),        \
                                RVV0p7_Pop(), RVV0p7_VI(), "t0",               \
                                RVV0p7_PushXSAT(), RVV0p7_PopXSAT())

// OPF
#define RVV0p7_BinopVF_M_T(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, RVV0p7_Mask(), ",v0.t", , , RVV0p7_VF(), "ft0", RVV0p7_PushFCSR(), RVV0p7_PopFCSR())
#define RVV0p7_BinopVF_T(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, , , , , RVV0p7_VF(), "ft0", RVV0p7_PushFCSR(), RVV0p7_PopFCSR())
#define RVV0p7_BinopVF_M2_T(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, RVV0p7_Mask(), ",v0", , , RVV0p7_VF(), "ft0", RVV0p7_PushFCSR(), RVV0p7_PopFCSR())

#define RVV0p7_BinopVF_M_T2(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T2(insn, vd, vs2, rs1, RVV0p7_Mask(), ",v0.t", , , RVV0p7_VF(), "ft0", RVV0p7_PushFCSR(), RVV0p7_PopFCSR())
#define RVV0p7_BinopVF_T2(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T2(insn, vd, vs2, rs1, , , , , RVV0p7_VF(), "ft0", RVV0p7_PushFCSR(), RVV0p7_PopFCSR())

#define RVV0p7_BinopWVF_M_T(insn, vd, vs2, rs1)                               \
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, RVV0p7_Mask(), ",v0.t",    \
                               RVV0p7_PushW(), RVV0p7_Pop(), RVV0p7_VF(),     \
                               "ft0", RVV0p7_PushFCSR(), RVV0p7_PopFCSR())
#define RVV0p7_BinopWVF_T(insn, vd, vs2, rs1)                                 \
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, , , RVV0p7_PushW(),        \
                               RVV0p7_Pop(), RVV0p7_VF(), "ft0",              \
                               RVV0p7_PushFCSR(), RVV0p7_PopFCSR())

#define RVV0p7_BinopWVF_M_T2(insn, vd, vs2, rs1)                               \
   RVV0p7_BinopVX_VI_VF_M_PP_T2(insn, vd, vs2, rs1, RVV0p7_Mask(), ",v0.t",    \
                                RVV0p7_PushW(), RVV0p7_Pop(), RVV0p7_VF(),     \
                                "ft0", RVV0p7_PushFCSR(), RVV0p7_PopFCSR())
#define RVV0p7_BinopWVF_T2(insn, vd, vs2, rs1)                                 \
   RVV0p7_BinopVX_VI_VF_M_PP_T2(insn, vd, vs2, rs1, , , RVV0p7_PushW(),        \
                                RVV0p7_Pop(), RVV0p7_VF(), "ft0",              \
                                RVV0p7_PushFCSR(), RVV0p7_PopFCSR())

#define RVV0p7_BinopVF_M_CMP(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_CMP_T(insn, vd, vs2, rs1, RVV0p7_Mask(), ",v0.t", RVV0p7_VF(), "ft0", RVV0p7_PushFCSR(), RVV0p7_PopFCSR())
#define RVV0p7_BinopVF_CMP(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_CMP_T(insn, vd, vs2, rs1, , , RVV0p7_VF(), "ft0", RVV0p7_PushFCSR(), RVV0p7_PopFCSR())

/* Unop */
// OPF
#define RVV0p7_UnopF_M_T(insn, vd, rs1)\
   RVV0p7_UnopX_F_M_PP_T(insn, vd, rs1, RVV0p7_Mask(), ",v0.t", , , RVV0p7_VF(), "ft0", RVV0p7_PushFCSR(), RVV0p7_PopFCSR())
#define RVV0p7_UnopF_T(insn, vd, rs1)\
   RVV0p7_UnopX_F_M_PP_T(insn, vd, rs1, , , , , RVV0p7_VF(), "ft0", RVV0p7_PushFCSR(), RVV0p7_PopFCSR())

/*---------------------------------------------------------------*/
/*--- OPI function templates                                  ---*/
/*---------------------------------------------------------------*/

#define RVV0p7_BinopOPIVV_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vv_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong vs1, ULong mask) { \
      RVV0p7_BinopOPIVV_M_T(#insn".vv", vd, vs2, vs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vv(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong vs1, ULong mask) { \
      RVV0p7_BinopOPIVV_T(#insn".vv", vd, vs2, vs1); \
   } \

#define RVV0p7_BinopVX_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vx_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong rs1, ULong mask) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopVX_M_T(#insn".vx", vd, vs2, rs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vx(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong rs1, ULong mask) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopVX_T(#insn".vx", vd, vs2, rs1); \
   } \

#define RVV0p7_BinopVI_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vi_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong rs1, ULong mask) { \
      RVV0p7_BinopVI_M_T(#insn".vx", vd, vs2, rs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vi(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong rs1, ULong mask) { \
      RVV0p7_BinopVI_T(#insn".vx", vd, vs2, rs1); \
   } \

#define RVV0p7_BinopOPIVV_P_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vv_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong vs1, ULong mask) { \
      RVV0p7_BinopOPIVV_M_P_T(#insn".vv", vd, vs2, vs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vv(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong vs1, ULong mask) { \
      RVV0p7_BinopOPIVV_P_T(#insn".vv", vd, vs2, vs1); \
   } \

#define RVV0p7_BinopVX_P_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vx_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong rs1, ULong mask) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopVX_M_P_T(#insn".vx", vd, vs2, rs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vx(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong rs1, ULong mask) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopVX_P_T(#insn".vx", vd, vs2, rs1); \
   } \

#define RVV0p7_BinopVI_P_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vi_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong rs1, ULong mask) { \
      RVV0p7_BinopVI_M_P_T(#insn".vx", vd, vs2, rs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vi(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong rs1, ULong mask) { \
      RVV0p7_BinopVI_P_T(#insn".vx", vd, vs2, rs1); \
   } \

#define RVV0p7_BinopOPIVS_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vv_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong vs1, ULong mask) { \
      RVV0p7_BinopOPIVV_M_T(#insn".vs", vd, vs2, vs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vv(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong vs1, ULong mask) { \
      RVV0p7_BinopOPIVV_T(#insn".vs", vd, vs2, vs1); \
   } \

#define RVV0p7_BinopOPIVS_M1D_P_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vv_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong vs1, ULong mask) { \
      RVV0p7_BinopOPIVV_M_M1D_P_T(#insn".vs", vd, vs2, vs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vv(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong vs1, ULong mask) { \
      RVV0p7_BinopOPIVV_M1D_P_T(#insn".vs", vd, vs2, vs1); \
   } \

// SAT
#define RVV0p7_BinopSATVV_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vv_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong vs1, ULong mask, \
                                         UInt xrm) { \
      RVV0p7_BinopSATVV_M_T(#insn".vv", vd, vs2, vs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vv(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong vs1, ULong mask, \
                                       UInt xrm) { \
      RVV0p7_BinopSATVV_T(#insn".vv", vd, vs2, vs1); \
   } \

#define RVV0p7_BinopSATVX_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vx_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong rs1, ULong mask, \
                                         UInt xrm) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopSATVX_M_T(#insn".vx", vd, vs2, rs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vx(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong rs1, ULong mask, \
                                       UInt xrm) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopSATVX_T(#insn".vx", vd, vs2, rs1); \
   } \

#define RVV0p7_BinopSATVI_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vi_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong rs1, ULong mask, \
                                         UInt xrm) { \
      RVV0p7_BinopSATVI_M_T(#insn".vx", vd, vs2, rs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vi(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong rs1, ULong mask, \
                                       UInt xrm) { \
      RVV0p7_BinopSATVI_T(#insn".vx", vd, vs2, rs1); \
   } \

#define RVV0p7_BinopSATWVV_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vv_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong vs1, ULong mask, \
                                         UInt xrm) { \
      RVV0p7_BinopSATWVV_M_T(#insn".vv", vd, vs2, vs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vv(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong vs1, ULong mask, \
                                       UInt xrm) { \
      RVV0p7_BinopSATWVV_T(#insn".vv", vd, vs2, vs1); \
   } \

#define RVV0p7_BinopSATWVX_FT2(insn) \
   static UInt RVV0p7_Binop_##insn##vx_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong rs1, ULong vs2, ULong mask, \
                                         UInt xrm) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopSATWVX_M_T2(#insn".vx", vd, vs2, rs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vx(VexGuestRISCV64State *st, \
                                       ULong vd, ULong rs1, ULong vs2, ULong mask, \
                                       UInt xrm) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopSATWVX_T2(#insn".vx", vd, vs2, rs1); \
   } \

#define RVV0p7_BinopSATNVV_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vv_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong vs1, ULong mask, \
                                         UInt xrm) { \
      RVV0p7_BinopSATNVV_M_T(#insn".vv", vd, vs2, vs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vv(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong vs1, ULong mask, \
                                       UInt xrm) { \
      RVV0p7_BinopSATNVV_T(#insn".vv", vd, vs2, vs1); \
   } \

#define RVV0p7_BinopSATNVX_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vx_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong rs1, ULong mask, \
                                         UInt xrm) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopSATNVX_M_T(#insn".vx", vd, vs2, rs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vx(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong rs1, ULong mask, \
                                       UInt xrm) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopSATNVX_T(#insn".vx", vd, vs2, rs1); \
   } \

#define RVV0p7_BinopSATNVI_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vi_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong rs1, ULong mask, \
                                         UInt xrm) { \
      RVV0p7_BinopSATNVI_M_T(#insn".vx", vd, vs2, rs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vi(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong rs1, ULong mask, \
                                       UInt xrm) { \
      RVV0p7_BinopSATNVI_T(#insn".vx", vd, vs2, rs1); \
   } \

// MASK
#define RVV0p7_BinopOPIMM_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vv(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong vs1, ULong mask) { \
      RVV0p7_BinopOPIMM_T(#insn".mm", vd, vs2, vs1); \
   } \

#define RVV0p7_UnopOPIM_FT(insn) \
   static UInt RVV0p7_Unop_##insn##v_m(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong mask) { \
      RVV0p7_UnopOPIM_M_T(#insn".m", vd, vs2); \
   } \

#define RVV0p7_UnopOPIM_M1S_FT(insn) \
   static UInt RVV0p7_Unop_##insn##v_m(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong mask) { \
      RVV0p7_UnopOPIM_M_M1S_T(#insn".m", vd, vs2); \
   } \

/*---------------------------------------------------------------*/
/*--- OPI function templates helper                           ---*/
/*---------------------------------------------------------------*/

#define RVV0p7_BinopOPIVV_VX_FT(insn) \
   RVV0p7_BinopOPIVV_FT(insn) \
   RVV0p7_BinopVX_FT(insn) \

#define RVV0p7_BinopOPIVV_VX_VI_FT(insn) \
   RVV0p7_BinopOPIVV_VX_FT(insn) \
   RVV0p7_BinopVI_FT(insn) \

#define RVV0p7_BinopVX_VI_P_FT(insn) \
   RVV0p7_BinopVX_P_FT(insn) \
   RVV0p7_BinopVI_P_FT(insn) \

#define RVV0p7_BinopOPIVV_VX_VI_P_FT(insn) \
   RVV0p7_BinopOPIVV_P_FT(insn) \
   RVV0p7_BinopVX_VI_P_FT(insn) \

/* SAT */
#define RVV0p7_BinopSATVV_VX_FT(insn) \
   RVV0p7_BinopSATVV_FT(insn) \
   RVV0p7_BinopSATVX_FT(insn) \

#define RVV0p7_BinopSATVV_VX_VI_FT(insn) \
   RVV0p7_BinopSATVV_VX_FT(insn) \
   RVV0p7_BinopSATVI_FT(insn) \

#define RVV0p7_BinopSATWVV_WVX_FT2(insn) \
   RVV0p7_BinopSATWVV_FT(insn) \
   RVV0p7_BinopSATWVX_FT2(insn) \

#define RVV0p7_BinopSATNVV_VX_VI_FT(insn) \
   RVV0p7_BinopSATNVV_FT(insn) \
   RVV0p7_BinopSATNVX_FT(insn) \
   RVV0p7_BinopSATNVI_FT(insn) \

/*---------------------------------------------------------------*/
/*--- OPI function definitions                                ---*/
/*---------------------------------------------------------------*/

RVV0p7_BinopOPIVV_VX_VI_FT(vadd)
RVV0p7_BinopVX_VI_P_FT(vslideup)
RVV0p7_BinopVX_VI_P_FT(vslidedown)
RVV0p7_BinopVX_P_FT(vslide1up)
RVV0p7_BinopVX_P_FT(vslide1down)
RVV0p7_BinopOPIVV_VX_VI_P_FT(vrgather)

RVV0p7_BinopOPIVS_FT(vredsum)
RVV0p7_BinopOPIVS_FT(vredmaxu)
RVV0p7_BinopOPIVS_FT(vredmax)
RVV0p7_BinopOPIVS_FT(vredminu)
RVV0p7_BinopOPIVS_FT(vredmin)
RVV0p7_BinopOPIVS_FT(vredand)
RVV0p7_BinopOPIVS_FT(vredor)
RVV0p7_BinopOPIVS_FT(vredxor)
RVV0p7_BinopOPIVS_M1D_P_FT(vwredsumu)
RVV0p7_BinopOPIVS_M1D_P_FT(vwredsum)

/* SAT */
RVV0p7_BinopSATVV_VX_VI_FT(vsaddu)
RVV0p7_BinopSATVV_VX_VI_FT(vsadd)
RVV0p7_BinopSATVV_VX_FT(vssubu)
RVV0p7_BinopSATVV_VX_FT(vssub)
RVV0p7_BinopSATVV_VX_VI_FT(vaadd)
RVV0p7_BinopSATVV_VX_FT(vasub)
RVV0p7_BinopSATVV_VX_FT(vsmul)
RVV0p7_BinopSATWVV_WVX_FT2(vwsmaccu)
RVV0p7_BinopSATWVV_WVX_FT2(vwsmacc)
RVV0p7_BinopSATWVV_WVX_FT2(vwsmaccsu)
RVV0p7_BinopSATWVX_FT2(vwsmaccus)
RVV0p7_BinopSATVV_VX_VI_FT(vssrl)
RVV0p7_BinopSATVV_VX_VI_FT(vssra)
RVV0p7_BinopSATNVV_VX_VI_FT(vnclipu)
RVV0p7_BinopSATNVV_VX_VI_FT(vnclip)

/* MASK */
RVV0p7_BinopOPIMM_FT(vmand)
RVV0p7_BinopOPIMM_FT(vmnand)
RVV0p7_BinopOPIMM_FT(vmandnot)
RVV0p7_BinopOPIMM_FT(vmxor)
RVV0p7_BinopOPIMM_FT(vmor)
RVV0p7_BinopOPIMM_FT(vmnor)
RVV0p7_BinopOPIMM_FT(vmornot)
RVV0p7_BinopOPIMM_FT(vmxnor)
RVV0p7_UnopOPIM_FT(vmsbf)
RVV0p7_UnopOPIM_FT(vmsif)
RVV0p7_UnopOPIM_FT(vmsof)
RVV0p7_UnopOPIM_M1S_FT(viota)

/*---------------------------------------------------------------*/
/*--- OPI special function definitions                        ---*/
/*---------------------------------------------------------------*/

static ULong GETA_VBinopVX(vext)(VexGuestRISCV64State *st,
                                 ULong vs2, ULong rs1) {
   ULong ret = 0;
   RVV0p7_BinopRVX_VF_T("vext.x.v", vs2, rs1, RVV0p7_VX(), ",t0", "r");
}

static UInt GETA_VUnopX(vmvs)(VexGuestRISCV64State *st,
                              ULong vd, ULong rs1, ULong mask) {
   RVV0p7_UnopX_F_M_PP_T("vmv.s.x", vd, rs1, , , RVV0p7_PushM1(), RVV0p7_Pop(), RVV0p7_VX(), "t0", , );
}

static ULong GETA_VBinopVV(vcompress)(VexGuestRISCV64State *st,
                                      ULong vd, ULong vs2, ULong vs1, ULong mask) {
   RVV0p7_BinopOPIVV_T("vcompress.vm", vd, vs2, vs1);
}

/*---------------------------------------------------------------*/
/*--- OPF function templates                                  ---*/
/*---------------------------------------------------------------*/

#define RVV0p7_BinopOPFVV_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vv_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong vs1, ULong mask, \
                                         UInt frm) { \
      RVV0p7_BinopOPFVV_M_T(#insn".vv", vd, vs2, vs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vv(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong vs1, ULong mask, \
                                       UInt frm) { \
      RVV0p7_BinopOPFVV_T(#insn".vv", vd, vs2, vs1); \
   } \

#define RVV0p7_BinopVF_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vf_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong rs1, ULong mask, \
                                         UInt frm) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopVF_M_T(#insn".vf", vd, vs2, rs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vf(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong rs1, ULong mask, \
                                       UInt frm) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopVF_T(#insn".vf", vd, vs2, rs1); \
   } \

#define RVV0p7_BinopVF_FT2(insn) \
   static UInt RVV0p7_Binop_##insn##vf_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong rs1, ULong vs2, ULong mask, \
                                         UInt frm) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopVF_M_T2(#insn".vf", vd, vs2, rs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vf(VexGuestRISCV64State *st, \
                                       ULong vd, ULong rs1, ULong vs2, ULong mask, \
                                       UInt frm) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopVF_T2(#insn".vf", vd, vs2, rs1); \
   } \

#define RVV0p7_BinopOPFWVV_FT(name, insn) \
   static UInt RVV0p7_Binop_##name##vv_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong vs1, ULong mask, \
                                         UInt frm) { \
      RVV0p7_BinopOPFWVV_M_T(#insn, vd, vs2, vs1); \
   } \
   static UInt RVV0p7_Binop_##name##vv(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong vs1, ULong mask, \
                                       UInt frm) { \
      RVV0p7_BinopOPFWVV_T(#insn, vd, vs2, vs1); \
   } \

#define RVV0p7_BinopWVF_FT(name, insn) \
   static UInt RVV0p7_Binop_##name##vf_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong rs1, ULong mask, \
                                         UInt frm) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopWVF_M_T(#insn, vd, vs2, rs1); \
   } \
   static UInt RVV0p7_Binop_##name##vf(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong rs1, ULong mask, \
                                       UInt frm) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopWVF_T(#insn, vd, vs2, rs1); \
   } \

#define RVV0p7_BinopWVF_FT2(insn) \
   static UInt RVV0p7_Binop_##insn##vf_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong rs1, ULong vs2, ULong mask, \
                                         UInt frm) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopWVF_M_T2(#insn".vf", vd, vs2, rs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vf(VexGuestRISCV64State *st, \
                                       ULong vd, ULong rs1, ULong vs2, ULong mask, \
                                       UInt frm) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopWVF_T2(#insn".vf", vd, vs2, rs1); \
   } \

#define RVV0p7_BinopOPFVV_FCMP(insn) \
   static UInt RVV0p7_Binop_##insn##vv_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong vs1, ULong mask, \
                                         UInt frm) { \
      RVV0p7_BinopOPFVV_M_CMP(#insn".vv", vd, vs2, vs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vv(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong vs1, ULong mask, \
                                       UInt frm) { \
      RVV0p7_BinopOPFVV_CMP(#insn".vv", vd, vs2, vs1); \
   } \

#define RVV0p7_BinopVF_FCMP(insn) \
   static UInt RVV0p7_Binop_##insn##vf_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong rs1, ULong mask, \
                                         UInt frm) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopVF_M_CMP(#insn".vf", vd, vs2, rs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vf(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong rs1, ULong mask, \
                                       UInt frm) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopVF_CMP(#insn".vf", vd, vs2, rs1); \
   } \

#define RVV0p7_UnopOPFV_FT(name, insn) \
   static UInt RVV0p7_Unop_##name##v_m(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong mask, \
                                       UInt frm) { \
      RVV0p7_UnopOPFV_M_T(#insn".v", vd, vs2); \
   } \
   static UInt RVV0p7_Unop_##name##v(VexGuestRISCV64State *st, \
                                     ULong vd, ULong vs2, ULong mask, \
                                     UInt frm) { \
      RVV0p7_UnopOPFV_T(#insn".v", vd, vs2); \
   } \

#define RVV0p7_UnopOPFWV_FT(name, insn) \
   static UInt RVV0p7_Unop_##name##v_m(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong mask, \
                                       UInt frm) { \
      RVV0p7_UnopOPFWV_M_T(#insn".v", vd, vs2); \
   } \
   static UInt RVV0p7_Unop_##name##v(VexGuestRISCV64State *st, \
                                     ULong vd, ULong vs2, ULong mask, \
                                     UInt frm) { \
      RVV0p7_UnopOPFWV_T(#insn".v", vd, vs2); \
   } \

#define RVV0p7_UnopOPFNV_FT(name, insn) \
   static UInt RVV0p7_Unop_##name##v_m(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong mask, \
                                       UInt frm) { \
      RVV0p7_UnopOPFNV_M_T(#insn".v", vd, vs2); \
   } \
   static UInt RVV0p7_Unop_##name##v(VexGuestRISCV64State *st, \
                                     ULong vd, ULong vs2, ULong mask, \
                                     UInt frm) { \
      RVV0p7_UnopOPFNV_T(#insn".v", vd, vs2); \
   } \

#define RVV0p7_BinopOPFVS_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vv_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong vs1, ULong mask, \
                                         UInt frm) { \
      RVV0p7_BinopOPFVV_M_T(#insn".vs", vd, vs2, vs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vv(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong vs1, ULong mask, \
                                       UInt frm) { \
      RVV0p7_BinopOPFVV_T(#insn".vs", vd, vs2, vs1); \
   } \

#define RVV0p7_BinopOPFVS_M1D_P_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vv_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong vs1, ULong mask, \
                                         UInt frm) { \
      RVV0p7_BinopOPFVV_M_M1D_P_T(#insn".vs", vd, vs2, vs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vv(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong vs1, ULong mask, \
                                       UInt frm) { \
      RVV0p7_BinopOPFVV_M1D_P_T(#insn".vs", vd, vs2, vs1); \
   } \

/*---------------------------------------------------------------*/
/*--- OPF function templates helper                           ---*/
/*---------------------------------------------------------------*/

#define RVV0p7_BinopOPFVV_VF_FT(insn) \
   RVV0p7_BinopOPFVV_FT(insn) \
   RVV0p7_BinopVF_FT(insn) \

#define RVV0p7_BinopOPFVV_VF_FT2(insn) \
   RVV0p7_BinopOPFVV_FT(insn) \
   RVV0p7_BinopVF_FT2(insn) \

#define RVV0p7_BinopOPFWVV_WVF_FT2(name, insn) \
   RVV0p7_BinopOPFWVV_FT(name, insn) \
   RVV0p7_BinopWVF_FT2(name) \

#define RVV0p7_BinopOPFVV_VF_FCMP(insn) \
   RVV0p7_BinopOPFVV_FCMP(insn) \
   RVV0p7_BinopVF_FCMP(insn) \

/*---------------------------------------------------------------*/
/*--- OPF function definitions                                ---*/
/*---------------------------------------------------------------*/

RVV0p7_BinopOPFVV_VF_FT(vfadd)
RVV0p7_BinopOPFVV_VF_FT(vfsub)
RVV0p7_BinopVF_FT(vfrsub)
RVV0p7_BinopOPFVV_VF_FT(vfmul)
RVV0p7_BinopOPFVV_VF_FT(vfdiv)
RVV0p7_BinopVF_FT(vfrdiv)
RVV0p7_BinopOPFVV_VF_FT(vfmin)
RVV0p7_BinopOPFVV_VF_FT(vfmax)
RVV0p7_BinopOPFVV_VF_FT(vfsgnj)
RVV0p7_BinopOPFVV_VF_FT(vfsgnjn)
RVV0p7_BinopOPFVV_VF_FT(vfsgnjx)

RVV0p7_BinopOPFWVV_FT(vfwadd, vfwadd.vv)
RVV0p7_BinopWVF_FT(vfwadd, vfwadd.vf)
RVV0p7_BinopOPFWVV_FT(vfwsub, vfwsub.vv)
RVV0p7_BinopWVF_FT(vfwsub, vfwsub.vf)
RVV0p7_BinopOPFWVV_FT(vfwmul, vfwmul.vv)
RVV0p7_BinopWVF_FT(vfwmul, vfwmul.vf)
RVV0p7_BinopOPFWVV_FT(vfwaddw, vfwadd.wv)
RVV0p7_BinopWVF_FT(vfwaddw, vfwadd.wf)
RVV0p7_BinopOPFWVV_FT(vfwsubw, vfwsub.wv)
RVV0p7_BinopWVF_FT(vfwsubw, vfwsub.wf)

RVV0p7_BinopOPFVV_VF_FT2(vfmacc)
RVV0p7_BinopOPFVV_VF_FT2(vfnmacc)
RVV0p7_BinopOPFVV_VF_FT2(vfmsac)
RVV0p7_BinopOPFVV_VF_FT2(vfnmsac)
RVV0p7_BinopOPFVV_VF_FT2(vfmadd)
RVV0p7_BinopOPFVV_VF_FT2(vfnmadd)
RVV0p7_BinopOPFVV_VF_FT2(vfmsub)
RVV0p7_BinopOPFVV_VF_FT2(vfnmsub)
RVV0p7_BinopOPFWVV_WVF_FT2(vfwmacc,  vfwmacc.vv)
RVV0p7_BinopOPFWVV_WVF_FT2(vfwnmacc, vfwnmacc.vv)
RVV0p7_BinopOPFWVV_WVF_FT2(vfwmsac,  vfwmsac.vv)
RVV0p7_BinopOPFWVV_WVF_FT2(vfwnmsac, vfwnmsac.vv)

RVV0p7_UnopOPFV_FT(vfsqrt, vfsqrt)
RVV0p7_UnopOPFV_FT(vfclass, vfclass)
RVV0p7_UnopOPFV_FT(vfcvt_xu_f,  vfcvt.xu.f)
RVV0p7_UnopOPFV_FT(vfcvt_x_f,   vfcvt.x.f)
RVV0p7_UnopOPFV_FT(vfcvt_f_xu,  vfcvt.f.xu)
RVV0p7_UnopOPFV_FT(vfcvt_f_x,   vfcvt.f.x)
RVV0p7_UnopOPFWV_FT(vfwcvt_xu_f, vfwcvt.xu.f)
RVV0p7_UnopOPFWV_FT(vfwcvt_x_f,  vfwcvt.x.f)
RVV0p7_UnopOPFWV_FT(vfwcvt_f_xu, vfwcvt.f.xu)
RVV0p7_UnopOPFWV_FT(vfwcvt_f_x,  vfwcvt.f.x)
RVV0p7_UnopOPFWV_FT(vfwcvt_f_f,  vfwcvt.f.f)
RVV0p7_UnopOPFNV_FT(vfncvt_xu_f, vfncvt.xu.f)
RVV0p7_UnopOPFNV_FT(vfncvt_x_f,  vfncvt.x.f)
RVV0p7_UnopOPFNV_FT(vfncvt_f_xu, vfncvt.f.xu)
RVV0p7_UnopOPFNV_FT(vfncvt_f_x,  vfncvt.f.x)
RVV0p7_UnopOPFNV_FT(vfncvt_f_f,  vfncvt.f.f)

RVV0p7_BinopOPFVV_VF_FCMP(vmfeq)
RVV0p7_BinopOPFVV_VF_FCMP(vmfne)
RVV0p7_BinopOPFVV_VF_FCMP(vmflt)
RVV0p7_BinopOPFVV_VF_FCMP(vmfle)
RVV0p7_BinopOPFVV_VF_FCMP(vmford)
RVV0p7_BinopVF_FCMP(vmfgt)
RVV0p7_BinopVF_FCMP(vmfge)

RVV0p7_BinopOPFVS_FT(vfredosum)
RVV0p7_BinopOPFVS_FT(vfredsum)
RVV0p7_BinopOPFVS_FT(vfredmax)
RVV0p7_BinopOPFVS_FT(vfredmin)
RVV0p7_BinopOPFVS_M1D_P_FT(vfwredosum)
RVV0p7_BinopOPFVS_M1D_P_FT(vfwredsum)

/*---------------------------------------------------------------*/
/*--- OPF special function definitions                        ---*/
/*---------------------------------------------------------------*/

static UInt GETA_VBinopVF_M(vfmerge)(VexGuestRISCV64State *st,
                                     ULong vd, ULong vs2, ULong rs1, ULong mask,
                                     UInt frm) {
   rs1 += (ULong)st;
   RVV0p7_BinopVF_M2_T("vfmerge.vfm", vd, vs2, rs1);
}
static UInt GETA_VUnopF(vfmerge)(VexGuestRISCV64State *st,
                                 ULong vd, ULong rs1, ULong mask,
                                 UInt frm) {
   RVV0p7_UnopF_T("vfmv.v.f", vd, rs1);
}

static Double GETA_VUnopV(vfmv)(VexGuestRISCV64State *st,
                                ULong vs2, ULong rs1) {
   Double ret = 0;
   RVV0p7_BinopRVX_VF_T("vfmv.f.s", vs2, rs1, , , "f");
}

static UInt GETA_VUnopF(vfmvs)(VexGuestRISCV64State *st,
                               ULong vd, ULong rs1, ULong mask,
                               UInt frm) {
   RVV0p7_UnopX_F_M_PP_T("vfmv.s.f", vd, rs1, , , RVV0p7_PushM1(), RVV0p7_Pop(), RVV0p7_VF(), "ft0", , );
}

/*---------------------------------------------------------------*/
/*--- Vector unsupported function definitions                 ---*/
/*---------------------------------------------------------------*/

typedef enum {
   GETA_VUnopX_M(vmvs)        = (Addr)NULL,
   GETA_VUnopF_M(vfmvs)       = (Addr)NULL,
   GETA_VBinopVV_M(vcompress) = (Addr)NULL,
   GETA_VBinopVF(vfmerge)     = (Addr)NULL,
   GETA_VUnopF_M(vfmerge)     = (Addr)NULL,

   GETA_VBinopVV_M(vmand)     = (Addr)NULL,
   GETA_VBinopVV_M(vmnand)    = (Addr)NULL,
   GETA_VBinopVV_M(vmandnot)  = (Addr)NULL,
   GETA_VBinopVV_M(vmxor)     = (Addr)NULL,
   GETA_VBinopVV_M(vmor)      = (Addr)NULL,
   GETA_VBinopVV_M(vmnor)     = (Addr)NULL,
   GETA_VBinopVV_M(vmornot)   = (Addr)NULL,
   GETA_VBinopVV_M(vmxnor)    = (Addr)NULL,
   GETA_VUnopV(vmsbf)         = (Addr)NULL,
   GETA_VUnopV(vmsif)         = (Addr)NULL,
   GETA_VUnopV(vmsof)         = (Addr)NULL,
   GETA_VUnopV(viota)         = (Addr)NULL,
} GETA_NULL;

/*--------------------------------------------------------------------*/
/*--- end                               guest_riscv64V0p7_helpers.c --*/
/*--------------------------------------------------------------------*/
