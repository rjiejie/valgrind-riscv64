
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

#define GETN_VUnopI_M(insn)    "RVV0p7_Unop_"#insn"i_m"
#define GETA_VUnopI_M(insn)    RVV0p7_Unop_##insn##i_m
#define GETN_VUnopI(insn)      "RVV0p7_Unop_"#insn"i"
#define GETA_VUnopI(insn)      RVV0p7_Unop_##insn##i

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
#define GETV_VopMask    (1 << 5) /* Indicate V0  is readed   by OP, e.g. VADC    */

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

#define GETC_VBinopOPI_VX(insn)                                                \
   GETC_VBinopOP_T(insn, V, X, X, offsetIReg64, nameIReg, GETR_VBinopOPI,      \
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

#define GETC_VBinopOPI_X_VAR(insn, vtype)                                      \
   GETC_VBinopOP_T(insn, X, X, X, offsetIReg64, nameIReg, GETR_VBinopOPI,      \
                   vtype)

#define GETC_VBinopOPI_V_VAR(insn, vtype)                                      \
   GETC_VBinopOP_T(insn, V, V, V, offsetIReg64, nameIReg, GETR_VBinopOPI,      \
                   vtype)

#define GETC_VBinopOPI_XI_VAR(insn, vtype)                                     \
   GETC_VBinopOP_T(insn, X, X, I, offsetIReg64, nameIReg, GETR_VBinopOPI,      \
                   vtype)

#define GETC_VBinopOPI_VX_VAR(insn, vtype)                                     \
   GETC_VBinopOP_T(insn, V, X, X, offsetIReg64, nameIReg, GETR_VBinopOPI,      \
                   vtype)

#define GETC_VBinopOPI_VAR(insn, vtype)                                        \
   GETC_VBinopOP_T(insn, V, X, I, offsetIReg64, nameIReg, GETR_VBinopOPI,      \
                   vtype);

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
#define GETC_VUnopOP_T(insn, V, X, I, REGO, REGN, ARGS, VARIANT)               \
   do {                                                                        \
      if (isVOpVV(GET_FUNCT3())) {                                             \
         fName = mask ? GETN_VUnop##V(insn) : GETN_VUnop##V##_M(insn);         \
         fAddr = mask ? GETA_VUnop##V(insn) : GETA_VUnop##V##_M(insn);         \
         temp  = offsetVReg(rs2);                                              \
      } else if (isVOpVXorVF(GET_FUNCT3())) {                                  \
         fName = mask ? GETN_VUnop##X(insn) : GETN_VUnop##X##_M(insn);         \
         fAddr = mask ? GETA_VUnop##X(insn) : GETA_VUnop##X##_M(insn);         \
         temp  = REGO(rs1);                                                    \
      } else {                                                                 \
         fName = mask ? GETN_VUnop##I(insn) : GETN_VUnop##I##_M(insn);         \
         fAddr = mask ? GETA_VUnop##I(insn) : GETA_VUnop##I##_M(insn);         \
         temp  = rs1;                                                          \
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
             isVOpVV(GET_FUNCT3()) ? nameVReg(rs2) : REGN(rs1));               \
   } while (0)

// OPI
#define GETR_VUnopOPI()                                                        \
   args = mkIRExprVec_4(IRExpr_GSPTR(), mkU64(offsetVReg(rd)), mkU64(temp),    \
                        mkU64(offsetVReg(0)));

#define GETC_VUnopOPI_V(insn)                                                  \
   GETC_VUnopOP_T(insn, V, V, V, offsetIReg64, nameIReg, GETR_VUnopOPI,        \
                  GETV_VopUnknow);

#define GETC_VUnopOPI_V_VAR(insn, vtype)                                       \
   GETC_VUnopOP_T(insn, V, V, V, offsetIReg64, nameIReg, GETR_VUnopOPI,        \
                  vtype);

#define GETC_VUnopOPI_X_VAR(insn, vtype)                                       \
   GETC_VUnopOP_T(insn, X, X, X, offsetIReg64, nameIReg, GETR_VUnopOPI,        \
                  vtype);

#define GETC_VUnopOPI(insn)                                                    \
   GETC_VUnopOP_T(insn, V, X, I, offsetIReg64, nameIReg, GETR_VUnopOPI,        \
                  GETV_VopUnknow);

// OPF
#define GETR_VUnopOPF()                                                        \
   assign(irsb, frm,                                                           \
          binop(Iop_And32, binop(Iop_Shr32, getFCSR(), mkU8(5)), mkU32(7)));   \
   args = mkIRExprVec_5(IRExpr_GSPTR(), mkU64(offsetVReg(rd)), mkU64(temp),    \
                        mkU64(offsetVReg(0)), mkexpr(frm));

#define GETC_VUnopOPF_V(insn)                                                  \
   GETC_VUnopOP_T(insn, V, V, V, offsetFReg, nameFReg, GETR_VUnopOPF,          \
                  GETV_VopUnknow);                                             \
   accumulateFFLAGS(irsb, mkexpr(ret));

#define GETC_VUnopOPF_F(insn)                                                  \
   GETC_VUnopOP_T(insn, F, F, F, offsetFReg, nameFReg, GETR_VUnopOPF,          \
                  GETV_VopUnknow);                                             \
   accumulateFFLAGS(irsb, mkexpr(ret));

#define GETC_VUnopOPF_V_VAR(insn, vtype)                                       \
   GETC_VUnopOP_T(insn, V, V, V, offsetFReg, nameFReg, GETR_VUnopOPF, vtype);  \
   accumulateFFLAGS(irsb, mkexpr(ret));

#define GETC_VUnopOPF_F_VAR(insn, vtype)                                       \
   GETC_VUnopOP_T(insn, F, F, F, offsetFReg, nameFReg, GETR_VUnopOPF, vtype);  \
   accumulateFFLAGS(irsb, mkexpr(ret));

/*---------------------------------------------------------------*/
/*--- Get dirty info of helper functions                      ---*/
/*---------------------------------------------------------------*/

/* Binop */
static IRDirty*
GETD_VBinop(IRDirty* d, UInt vd, UInt vs2, UInt vs1, Bool mask, UInt sopc, UInt vtype)
{
   UInt lmul   = extract_lmul(guest_VFLAG);
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
   UInt lmul   = extract_lmul(guest_VFLAG);
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

// v8-v15, v16-v23
#define RVV0p7_UnopV_M_PP_T(insn, vd, vs, imask, mreg, ipush, ipop, ipre, ipost)\
   do {                                                     \
      UInt ret = 0;                                         \
                                                            \
      vd  += (ULong)st;                                     \
      vs  += (ULong)st;                                     \
      mask += (ULong)st;                                    \
                                                            \
      ipush                                                 \
      __asm__ __volatile__(                                 \
         "vle.v\tv8,(%0)\n\t"                               \
         "vle.v\tv16,(%1)\n\t"                              \
         :                                                  \
         : "r"(vd), "r"(vs)                                 \
         :);                                                \
      ipop                                                  \
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

#define RVV0p7_BinopOPIVV_M_T2(insn, vd, vs2, vs1)\
   RVV0p7_BinopVV_M_PP_T(insn, vd, vs1, vs2, RVV0p7_Mask(), ",v0.t", , , , )
#define RVV0p7_BinopOPIVV_T2(insn, vd, vs2, vs1)\
   RVV0p7_BinopVV_M_PP_T(insn, vd, vs1, vs2, , , , , , )

#define RVV0p7_BinopOPIWVV_M_T(insn, vd, vs2, vs1)                              \
   RVV0p7_BinopVV_M_PP_T(insn, vd, vs2, vs1, RVV0p7_Mask(), ",v0.t",            \
                         RVV0p7_PushW(), RVV0p7_Pop(), ,)
#define RVV0p7_BinopOPIWVV_T(insn, vd, vs2, vs1) \
   RVV0p7_BinopVV_M_PP_T(insn, vd, vs2, vs1, , , RVV0p7_PushW(), RVV0p7_Pop(), ,)

#define RVV0p7_BinopOPIWVV_M_T2(insn, vd, vs2, vs1)                             \
   RVV0p7_BinopVV_M_PP_T(insn, vd, vs1, vs2, RVV0p7_Mask(), ",v0.t",            \
                         RVV0p7_PushW(), RVV0p7_Pop(), , )
#define RVV0p7_BinopOPIWVV_T2(insn, vd, vs2, vs1) \
   RVV0p7_BinopVV_M_PP_T(insn, vd, vs1, vs2, , , RVV0p7_PushW(), RVV0p7_Pop(), ,)

// VVM
#define RVV0p7_BinopOPIVVM_T(insn, vd, vs2, vs1)\
   RVV0p7_BinopVV_M_PP_T(insn, vd, vs2, vs1, RVV0p7_Mask(), ",v0", , , , )

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

#define RVV0p7_BinopOPFVV_M_P_T(insn, vd, vs2, vs1)\
   RVV0p7_BinopVV_M_PP_T(insn, vd, vs2, vs1, RVV0p7_Mask(), ",v0.t", RVV0p7_Push(), RVV0p7_Pop(), RVV0p7_PushFCSR(), RVV0p7_PopFCSR())
#define RVV0p7_BinopOPFVV_P_T(insn, vd, vs2, vs1)\
   RVV0p7_BinopVV_M_PP_T(insn, vd, vs2, vs1, , , RVV0p7_Push(), RVV0p7_Pop(), RVV0p7_PushFCSR(), RVV0p7_PopFCSR())

/* Unop */
// OPI
#define RVV0p7_UnopOPIV_T(insn, vd, vs2)\
   RVV0p7_UnopV_M_PP_T(insn, vd, vs2, , , , , , )

#define RVV0p7_UnopOPIV_M_P_T(insn, vd, vs2)\
   RVV0p7_UnopV_M_PP_T(insn, vd, vs2, RVV0p7_Mask(), ",v0.t", RVV0p7_Push(), RVV0p7_Pop(), , )
#define RVV0p7_UnopOPIV_P_T(insn, vd, vs2)\
   RVV0p7_UnopV_M_PP_T(insn, vd, vs2, , , RVV0p7_Push(), RVV0p7_Pop(), , )

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

// ret, v16, t0/ft0
#define RVV0p7_BinopRVX_VF_T(insn, vs2, rs1, imask, mreg, isopc, treg, oreg)\
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
      imask                                                 \
      isopc                                                 \
      __asm__ __volatile__(insn "\t%0,v16" treg mreg        \
                           :"=" oreg (ret)::);              \
                                                            \
      return ret;                                           \
   } while (0)

// v8-v15, t0/ft0
#define RVV0p7_UnopX_I_F_M_PP_T(insn, vd, rs1, imask, mreg, ipush, ipop, isopc, treg, ipre, ipost)\
   do {                                                     \
      UInt ret = 0;                                         \
                                                            \
      vd  += (ULong)st;                                     \
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

#define RVV0p7_BinopVX_M_T2(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T2(insn, vd, vs2, rs1, RVV0p7_Mask(), ",v0.t", , , RVV0p7_VX(), "t0", , )
#define RVV0p7_BinopVX_T2(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T2(insn, vd, vs2, rs1, , , , , RVV0p7_VX(), "t0", , )

#define RVV0p7_BinopWVX_M_T(insn, vd, vs2, rs1)                                \
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, RVV0p7_Mask(), ",v0.t",     \
                               RVV0p7_PushW(), RVV0p7_Pop(), RVV0p7_VX(),      \
                               "t0", , )
#define RVV0p7_BinopWVX_T(insn, vd, vs2, rs1)                                  \
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, , , RVV0p7_PushW(),         \
                               RVV0p7_Pop(), RVV0p7_VX(), "t0", , )

#define RVV0p7_BinopWVI_M_T(insn, vd, vs2, rs1)                                \
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, RVV0p7_Mask(), ",v0.t",     \
                               RVV0p7_PushW(), RVV0p7_Pop(), RVV0p7_VI(),      \
                               "t0", , )
#define RVV0p7_BinopWVI_T(insn, vd, vs2, rs1)                                  \
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, , , RVV0p7_PushW(),         \
                               RVV0p7_Pop(), RVV0p7_VI(), "t0", , )

#define RVV0p7_BinopWVX_M_T2(insn, vd, vs2, rs1)                               \
   RVV0p7_BinopVX_VI_VF_M_PP_T2(insn, vd, vs2, rs1, RVV0p7_Mask(), ",v0.t",    \
                                RVV0p7_PushW(), RVV0p7_Pop(), RVV0p7_VX(),     \
                                "t0", , )
#define RVV0p7_BinopWVX_T2(insn, vd, vs2, rs1)                                 \
   RVV0p7_BinopVX_VI_VF_M_PP_T2(insn, vd, vs2, rs1, , , RVV0p7_PushW(),        \
                                RVV0p7_Pop(), RVV0p7_VX(), "t0", , )

// VXM/VIM
#define RVV0p7_BinopVXM_T(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, RVV0p7_Mask(), ",v0", , , RVV0p7_VX(), "t0", , )
#define RVV0p7_BinopVIM_T(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, RVV0p7_Mask(), ",v0", , , RVV0p7_VI(), "t0", , )

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

#define RVV0p7_BinopSATWVX_M_T(insn, vd, vs2, rs1)                             \
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, RVV0p7_Mask(), ",v0.t",     \
                               RVV0p7_PushW(), RVV0p7_Pop(), RVV0p7_VX(),     \
                               "t0", RVV0p7_PushXSAT(), RVV0p7_PopXSAT())
#define RVV0p7_BinopSATWVX_T(insn, vd, vs2, rs1)                               \
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, , , RVV0p7_PushW(),         \
                               RVV0p7_Pop(), RVV0p7_VX(), "t0",               \
                               RVV0p7_PushXSAT(), RVV0p7_PopXSAT())

#define RVV0p7_BinopSATWVI_M_T(insn, vd, vs2, rs1)                             \
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, RVV0p7_Mask(), ",v0.t",     \
                               RVV0p7_PushW(), RVV0p7_Pop(), RVV0p7_VI(),     \
                               "t0", RVV0p7_PushXSAT(), RVV0p7_PopXSAT())
#define RVV0p7_BinopSATWVI_T(insn, vd, vs2, rs1)                               \
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, , , RVV0p7_PushW(),         \
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

#define RVV0p7_BinopVF_M_P_T(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, RVV0p7_Mask(), ",v0.t", RVV0p7_Push(), RVV0p7_Pop(), RVV0p7_VF(), "ft0", RVV0p7_PushFCSR(), RVV0p7_PopFCSR())
#define RVV0p7_BinopVF_P_T(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, , , RVV0p7_Push(), RVV0p7_Pop(), RVV0p7_VF(), "ft0", RVV0p7_PushFCSR(), RVV0p7_PopFCSR())

/* Unop */
// OPI
#define RVV0p7_UnopX_T(insn, vd, rs1)\
   RVV0p7_UnopX_I_F_M_PP_T(insn, vd, rs1, , , , , RVV0p7_VX(), "t0", , )

#define RVV0p7_UnopX_P_T(insn, vd, rs1)\
   RVV0p7_UnopX_I_F_M_PP_T(insn, vd, rs1, , , RVV0p7_Push(), RVV0p7_Pop(), RVV0p7_VX(), "t0", , )

#define RVV0p7_UnopI_T(insn, vd, rs1)\
   RVV0p7_UnopX_I_F_M_PP_T(insn, vd, rs1, , , , , RVV0p7_VI(), "t0", , )

// OPF
#define RVV0p7_UnopF_M_T(insn, vd, rs1)\
   RVV0p7_UnopX_I_F_M_PP_T(insn, vd, rs1, RVV0p7_Mask(), ",v0.t", , , RVV0p7_VF(), "ft0", RVV0p7_PushFCSR(), RVV0p7_PopFCSR())
#define RVV0p7_UnopF_T(insn, vd, rs1)\
   RVV0p7_UnopX_I_F_M_PP_T(insn, vd, rs1, , , , , RVV0p7_VF(), "ft0", RVV0p7_PushFCSR(), RVV0p7_PopFCSR())

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

#define RVV0p7_BinopOPIVS_P_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vv_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong vs1, ULong mask) { \
      RVV0p7_BinopOPIVV_M_P_T(#insn".vs", vd, vs2, vs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vv(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong vs1, ULong mask) { \
      RVV0p7_BinopOPIVV_P_T(#insn".vs", vd, vs2, vs1); \
   } \

#define RVV0p7_BinopOPIVV_FT2(insn) \
   static UInt RVV0p7_Binop_##insn##vv_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong vs1, ULong mask) { \
      RVV0p7_BinopOPIVV_M_T2(#insn".vv", vd, vs2, vs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vv(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong vs1, ULong mask) { \
      RVV0p7_BinopOPIVV_T2(#insn".vv", vd, vs2, vs1); \
   } \

#define RVV0p7_BinopVX_FT2(insn) \
   static UInt RVV0p7_Binop_##insn##vx_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong rs1, ULong mask) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopVX_M_T2(#insn".vx", vd, vs2, rs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vx(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong rs1, ULong mask) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopVX_T2(#insn".vx", vd, vs2, rs1); \
   } \

#define RVV0p7_BinopOPIWVV_FT(name, insn) \
   static UInt RVV0p7_Binop_##name##vv_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong vs1, ULong mask) { \
      RVV0p7_BinopOPIWVV_M_T(#insn, vd, vs2, vs1); \
   } \
   static UInt RVV0p7_Binop_##name##vv(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong vs1, ULong mask) { \
      RVV0p7_BinopOPIWVV_T(#insn, vd, vs2, vs1); \
   } \

#define RVV0p7_BinopWVX_FT(name, insn) \
   static UInt RVV0p7_Binop_##name##vx_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong rs1, ULong mask) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopWVX_M_T(#insn, vd, vs2, rs1); \
   } \
   static UInt RVV0p7_Binop_##name##vx(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong rs1, ULong mask) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopWVX_T(#insn, vd, vs2, rs1); \
   } \

#define RVV0p7_BinopWVI_FT(name, insn) \
   static UInt RVV0p7_Binop_##name##vi_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong rs1, ULong mask) { \
      RVV0p7_BinopWVI_M_T(#insn, vd, vs2, rs1); \
   } \
   static UInt RVV0p7_Binop_##name##vi(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong rs1, ULong mask) { \
      RVV0p7_BinopWVI_T(#insn, vd, vs2, rs1); \
   } \

#define RVV0p7_BinopOPIWVV_FT2(insn) \
   static UInt RVV0p7_Binop_##insn##vv_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong vs1, ULong mask) { \
      RVV0p7_BinopOPIWVV_M_T2(#insn".vv", vd, vs2, vs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vv(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong vs1, ULong mask) { \
      RVV0p7_BinopOPIWVV_T2(#insn".vv", vd, vs2, vs1); \
   } \

#define RVV0p7_BinopWVX_FT2(insn) \
   static UInt RVV0p7_Binop_##insn##vx_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong rs1, ULong mask) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopWVX_M_T2(#insn".vx", vd, vs2, rs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vx(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong rs1, ULong mask) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopWVX_T2(#insn".vx", vd, vs2, rs1); \
   } \

// VVM
#define RVV0p7_BinopOPIVVM_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vv_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong vs1, ULong mask) { \
      RVV0p7_BinopOPIVVM_T(#insn".vvm", vd, vs2, vs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vv(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong vs1, ULong mask) { \
      RVV0p7_BinopOPIVVM_T(#insn".vvm", vd, vs2, vs1); \
   } \

#define RVV0p7_BinopOPIVXM_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vx_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong rs1, ULong mask) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopVXM_T(#insn".vxm", vd, vs2, rs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vx(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong rs1, ULong mask) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopVXM_T(#insn".vxm", vd, vs2, rs1); \
   } \

#define RVV0p7_BinopOPIVIM_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vi_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong rs1, ULong mask) { \
      RVV0p7_BinopVIM_T(#insn".vxm", vd, vs2, rs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vi(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong rs1, ULong mask) { \
      RVV0p7_BinopVIM_T(#insn".vxm", vd, vs2, rs1); \
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

#define RVV0p7_BinopSATWVX_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vx_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong rs1, ULong mask, \
                                         UInt xrm) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopSATWVX_M_T(#insn".vx", vd, vs2, rs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vx(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong rs1, ULong mask, \
                                       UInt xrm) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopSATWVX_T(#insn".vx", vd, vs2, rs1); \
   } \

#define RVV0p7_BinopSATWVI_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vi_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong rs1, ULong mask, \
                                         UInt xrm) { \
      RVV0p7_BinopSATWVI_M_T(#insn".vx", vd, vs2, rs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vi(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong rs1, ULong mask, \
                                       UInt xrm) { \
      RVV0p7_BinopSATWVI_T(#insn".vx", vd, vs2, rs1); \
   } \

// MASK
#define RVV0p7_BinopOPIMM_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vv(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong vs1, ULong mask) { \
      RVV0p7_BinopOPIVV_P_T(#insn".mm", vd, vs2, vs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vv_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong vs1, ULong mask) { \
      RVV0p7_BinopOPIVV_P_T(#insn".mm", vd, vs2, vs1); \
   } \

#define RVV0p7_UnopOPIM_P_FT(insn) \
   static UInt RVV0p7_Unop_##insn##v_m(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong mask) { \
      RVV0p7_UnopOPIV_M_P_T(#insn".m", vd, vs2); \
   } \
   static UInt RVV0p7_Unop_##insn##v(VexGuestRISCV64State *st, \
                                     ULong vd, ULong vs2, ULong mask) { \
      RVV0p7_UnopOPIV_P_T(#insn".m", vd, vs2); \
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

#define RVV0p7_BinopOPIVV_VX_P_FT(insn) \
   RVV0p7_BinopOPIVV_P_FT(insn) \
   RVV0p7_BinopVX_P_FT(insn) \

#define RVV0p7_BinopVX_VI_P_FT(insn) \
   RVV0p7_BinopVX_P_FT(insn) \
   RVV0p7_BinopVI_P_FT(insn) \

#define RVV0p7_BinopOPIVV_VX_VI_P_FT(insn) \
   RVV0p7_BinopOPIVV_P_FT(insn) \
   RVV0p7_BinopVX_VI_P_FT(insn) \

#define RVV0p7_BinopOPIVV_VX_FT2(insn) \
   RVV0p7_BinopOPIVV_FT2(insn) \
   RVV0p7_BinopVX_FT2(insn) \

#define RVV0p7_BinopOPIWVV_WVX_FT2(insn) \
   RVV0p7_BinopOPIWVV_FT2(insn) \
   RVV0p7_BinopWVX_FT2(insn) \

/* VVM */
#define RVV0p7_BinopOPIVVM_VXM_FT(insn) \
   RVV0p7_BinopOPIVVM_FT(insn) \
   RVV0p7_BinopOPIVXM_FT(insn) \

#define RVV0p7_BinopOPIVVM_VXM_VIM_FT(insn) \
   RVV0p7_BinopOPIVVM_VXM_FT(insn) \
   RVV0p7_BinopOPIVIM_FT(insn) \

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

#define RVV0p7_BinopSATWVV_VX_VI_FT(insn) \
   RVV0p7_BinopSATWVV_FT(insn) \
   RVV0p7_BinopSATWVX_FT(insn) \
   RVV0p7_BinopSATWVI_FT(insn) \

/*---------------------------------------------------------------*/
/*--- OPI function definitions                                ---*/
/*---------------------------------------------------------------*/

RVV0p7_BinopOPIVV_VX_VI_FT(vadd)
RVV0p7_BinopOPIVV_VX_FT(vsub)
RVV0p7_BinopVX_FT(vrsub)
RVV0p7_BinopVI_FT(vrsub)

RVV0p7_BinopOPIWVV_FT(vwaddu, vwaddu.vv)
RVV0p7_BinopWVX_FT(vwaddu, vwaddu.vx)
RVV0p7_BinopOPIWVV_FT(vwadduw, vwaddu.wv)
RVV0p7_BinopWVX_FT(vwadduw, vwaddu.wx)
RVV0p7_BinopOPIWVV_FT(vwsubu, vwsubu.vv)
RVV0p7_BinopWVX_FT(vwsubu, vwsubu.vx)
RVV0p7_BinopOPIWVV_FT(vwsubuw, vwsubu.wv)
RVV0p7_BinopWVX_FT(vwsubuw, vwsubu.wx)
RVV0p7_BinopOPIWVV_FT(vwadd, vwadd.vv)
RVV0p7_BinopWVX_FT(vwadd, vwadd.vx)
RVV0p7_BinopOPIWVV_FT(vwaddw, vwadd.wv)
RVV0p7_BinopWVX_FT(vwaddw, vwadd.wx)
RVV0p7_BinopOPIWVV_FT(vwsub, vwsub.vv)
RVV0p7_BinopWVX_FT(vwsub, vwsub.vx)
RVV0p7_BinopOPIWVV_FT(vwsubw, vwsub.wv)
RVV0p7_BinopWVX_FT(vwsubw, vwsub.wx)

RVV0p7_BinopOPIVVM_VXM_VIM_FT(vadc)
RVV0p7_BinopOPIVVM_VXM_VIM_FT(vmadc)
RVV0p7_BinopOPIVVM_VXM_FT(vsbc)
RVV0p7_BinopOPIVVM_VXM_FT(vmsbc)

RVV0p7_BinopOPIVV_VX_VI_FT(vand)
RVV0p7_BinopOPIVV_VX_VI_FT(vor)
RVV0p7_BinopOPIVV_VX_VI_FT(vxor)
RVV0p7_BinopOPIVV_VX_VI_FT(vsll)
RVV0p7_BinopOPIVV_VX_VI_FT(vsrl)
RVV0p7_BinopOPIVV_VX_VI_FT(vsra)
RVV0p7_BinopOPIWVV_FT(vnsrl, vnsrl.vv)
RVV0p7_BinopWVX_FT(vnsrl, vnsrl.vx)
RVV0p7_BinopWVI_FT(vnsrl, vnsrl.vx)
RVV0p7_BinopOPIWVV_FT(vnsra, vnsra.vv)
RVV0p7_BinopWVX_FT(vnsra, vnsra.vx)
RVV0p7_BinopWVI_FT(vnsra, vnsra.vx)

RVV0p7_BinopOPIVV_VX_VI_P_FT(vmseq)
RVV0p7_BinopOPIVV_VX_VI_P_FT(vmsne)
RVV0p7_BinopOPIVV_VX_P_FT(vmsltu)
RVV0p7_BinopOPIVV_VX_P_FT(vmslt)
RVV0p7_BinopOPIVV_VX_VI_P_FT(vmsleu)
RVV0p7_BinopOPIVV_VX_VI_P_FT(vmsle)
RVV0p7_BinopVX_VI_P_FT(vmsgtu)
RVV0p7_BinopVX_VI_P_FT(vmsgt)

RVV0p7_BinopOPIVV_VX_FT(vminu)
RVV0p7_BinopOPIVV_VX_FT(vmin)
RVV0p7_BinopOPIVV_VX_FT(vmaxu)
RVV0p7_BinopOPIVV_VX_FT(vmax)

RVV0p7_BinopOPIVV_VX_FT(vmul)
RVV0p7_BinopOPIVV_VX_FT(vmulh)
RVV0p7_BinopOPIVV_VX_FT(vmulhu)
RVV0p7_BinopOPIVV_VX_FT(vmulhsu)
RVV0p7_BinopOPIVV_VX_FT(vdivu)
RVV0p7_BinopOPIVV_VX_FT(vdiv)
RVV0p7_BinopOPIVV_VX_FT(vremu)
RVV0p7_BinopOPIVV_VX_FT(vrem)

RVV0p7_BinopOPIWVV_FT(vwmul, vwmul.vv)
RVV0p7_BinopWVX_FT(vwmul, vwmul.vx)
RVV0p7_BinopOPIWVV_FT(vwmulu, vwmulu.vv)
RVV0p7_BinopWVX_FT(vwmulu, vwmulu.vx)
RVV0p7_BinopOPIWVV_FT(vwmulsu, vwmulsu.vv)
RVV0p7_BinopWVX_FT(vwmulsu, vwmulsu.vx)

RVV0p7_BinopOPIVV_VX_FT2(vmacc)
RVV0p7_BinopOPIVV_VX_FT2(vnmsac)
RVV0p7_BinopOPIVV_VX_FT2(vmadd)
RVV0p7_BinopOPIVV_VX_FT2(vnmsub)

RVV0p7_BinopOPIWVV_WVX_FT2(vwmaccu)
RVV0p7_BinopOPIWVV_WVX_FT2(vwmacc)
RVV0p7_BinopOPIWVV_WVX_FT2(vwmaccsu)
RVV0p7_BinopWVX_FT2(vwmaccus)

RVV0p7_BinopOPIVVM_VXM_VIM_FT(vmerge)

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
RVV0p7_BinopOPIVS_P_FT(vwredsumu)
RVV0p7_BinopOPIVS_P_FT(vwredsum)

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
RVV0p7_BinopSATWVV_VX_VI_FT(vnclipu)
RVV0p7_BinopSATWVV_VX_VI_FT(vnclip)

/* MASK */
RVV0p7_BinopOPIMM_FT(vmand)
RVV0p7_BinopOPIMM_FT(vmnand)
RVV0p7_BinopOPIMM_FT(vmandnot)
RVV0p7_BinopOPIMM_FT(vmxor)
RVV0p7_BinopOPIMM_FT(vmor)
RVV0p7_BinopOPIMM_FT(vmnor)
RVV0p7_BinopOPIMM_FT(vmornot)
RVV0p7_BinopOPIMM_FT(vmxnor)
RVV0p7_UnopOPIM_P_FT(vmsbf)
RVV0p7_UnopOPIM_P_FT(vmsif)
RVV0p7_UnopOPIM_P_FT(vmsof)
RVV0p7_UnopOPIM_P_FT(viota)

/*---------------------------------------------------------------*/
/*--- OPI special function definitions                        ---*/
/*---------------------------------------------------------------*/

static UInt GETA_VUnopV(vmv)(VexGuestRISCV64State *st,
                             ULong vd, ULong vs2, ULong vs1, ULong mask) {
   RVV0p7_UnopOPIV_T("vmv.v.v", vd, vs1);
}
static UInt GETA_VUnopX(vmv)(VexGuestRISCV64State *st,
                             ULong vd, ULong vs2, ULong rs1, ULong mask) {
   rs1 += (ULong)st;
   RVV0p7_UnopX_T("vmv.v.x", vd, rs1);
}
static UInt GETA_VUnopI(vmv)(VexGuestRISCV64State *st,
                             ULong vd, ULong vs2, ULong rs1, ULong mask) {
   RVV0p7_UnopI_T("vmv.v.x", vd, rs1);
}

static ULong GETA_VBinopVX(vext)(VexGuestRISCV64State *st,
                                 ULong vs2, ULong rs1) {
   ULong ret = 0;
   RVV0p7_BinopRVX_VF_T("vext.x.v", vs2, rs1, , , RVV0p7_VX(), ",t0", "r");
}

static UInt GETA_VUnopX(vmvs)(VexGuestRISCV64State *st,
                              ULong vd, ULong rs1, ULong mask) {
   rs1 += (ULong)st;
   RVV0p7_UnopX_P_T("vmv.s.x", vd, rs1);
}

static ULong GETA_VBinopVV(vcompress)(VexGuestRISCV64State *st,
                                      ULong vd, ULong vs2, ULong vs1, ULong mask) {
   RVV0p7_BinopOPIVV_T("vcompress.vm", vd, vs2, vs1);
}

static ULong GETA_VUnopV(vmpopc)(VexGuestRISCV64State *st,
                                 ULong vs2, ULong rs1, ULong mask) {
   ULong ret = 0;
   RVV0p7_BinopRVX_VF_T("vmpopc.m", vs2, rs1, , , , , "r");
}
static ULong GETA_VUnopV_M(vmpopc)(VexGuestRISCV64State *st,
                                   ULong vs2, ULong rs1, ULong mask) {
   ULong ret = 0;
   RVV0p7_BinopRVX_VF_T("vmpopc.m", vs2, rs1, RVV0p7_Mask(), ",v0.t", , , "r");
}

static ULong GETA_VUnopV(vmfirst)(VexGuestRISCV64State *st,
                                  ULong vs2, ULong rs1, ULong mask) {
   ULong ret = 0;
   RVV0p7_BinopRVX_VF_T("vmfirst.m", vs2, rs1, , , , , "r");
}
static ULong GETA_VUnopV_M(vmfirst)(VexGuestRISCV64State *st,
                                    ULong vs2, ULong rs1, ULong mask) {
   ULong ret = 0;
   RVV0p7_BinopRVX_VF_T("vmfirst.m", vs2, rs1, RVV0p7_Mask(), ",v0.t", , , "r");
}

static UInt GETA_VUnopV(vid)(VexGuestRISCV64State *st,
                             ULong vd, ULong vs2, ULong mask) {
   UInt ret = 0;
   vd += (ULong)st;
   __asm__ __volatile__(
      "vle.v\tv8,(%0)\n\t"
      "vid.v\tv8\n\t"
      "vse.v\tv8,(%0)\n\t"
      :
      : "r"(vd)
      : "memory");
   return ret;
}
static UInt GETA_VUnopV_M(vid)(VexGuestRISCV64State *st,
                               ULong vd, ULong vs2, ULong mask) {
   UInt ret = 0;
   vd += (ULong)st;
   mask += (ULong)st;
   RVV0p7_Mask()
   __asm__ __volatile__(
      "vle.v\tv8,(%0)\n\t"
      "vid.v\tv8,v0.t\n\t"
      "vse.v\tv8,(%0)\n\t"
      :
      : "r"(vd)
      : "memory");
   return ret;
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

#define RVV0p7_BinopOPFVV_P_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vv_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong vs1, ULong mask, \
                                         UInt frm) { \
      RVV0p7_BinopOPFVV_M_P_T(#insn".vv", vd, vs2, vs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vv(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong vs1, ULong mask, \
                                       UInt frm) { \
      RVV0p7_BinopOPFVV_P_T(#insn".vv", vd, vs2, vs1); \
   } \

#define RVV0p7_BinopVF_P_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vf_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong rs1, ULong mask, \
                                         UInt frm) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopVF_M_P_T(#insn".vf", vd, vs2, rs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vf(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong rs1, ULong mask, \
                                       UInt frm) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopVF_P_T(#insn".vf", vd, vs2, rs1); \
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

#define RVV0p7_BinopOPFVS_P_FT(insn) \
   static UInt RVV0p7_Binop_##insn##vv_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong vs1, ULong mask, \
                                         UInt frm) { \
      RVV0p7_BinopOPFVV_M_P_T(#insn".vs", vd, vs2, vs1); \
   } \
   static UInt RVV0p7_Binop_##insn##vv(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong vs1, ULong mask, \
                                       UInt frm) { \
      RVV0p7_BinopOPFVV_P_T(#insn".vs", vd, vs2, vs1); \
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

#define RVV0p7_BinopOPFVV_VF_P_FT(insn) \
   RVV0p7_BinopOPFVV_P_FT(insn) \
   RVV0p7_BinopVF_P_FT(insn) \

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
RVV0p7_UnopOPFWV_FT(vfncvt_xu_f, vfncvt.xu.f)
RVV0p7_UnopOPFWV_FT(vfncvt_x_f,  vfncvt.x.f)
RVV0p7_UnopOPFWV_FT(vfncvt_f_xu, vfncvt.f.xu)
RVV0p7_UnopOPFWV_FT(vfncvt_f_x,  vfncvt.f.x)
RVV0p7_UnopOPFWV_FT(vfncvt_f_f,  vfncvt.f.f)

RVV0p7_BinopOPFVV_VF_P_FT(vmfeq)
RVV0p7_BinopOPFVV_VF_P_FT(vmfne)
RVV0p7_BinopOPFVV_VF_P_FT(vmflt)
RVV0p7_BinopOPFVV_VF_P_FT(vmfle)
RVV0p7_BinopOPFVV_VF_P_FT(vmford)
RVV0p7_BinopVF_P_FT(vmfgt)
RVV0p7_BinopVF_P_FT(vmfge)

RVV0p7_BinopOPFVS_FT(vfredosum)
RVV0p7_BinopOPFVS_FT(vfredsum)
RVV0p7_BinopOPFVS_FT(vfredmax)
RVV0p7_BinopOPFVS_FT(vfredmin)
RVV0p7_BinopOPFVS_P_FT(vfwredosum)
RVV0p7_BinopOPFVS_P_FT(vfwredsum)

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
   rs1 += (ULong)st;
   RVV0p7_UnopF_T("vfmv.v.f", vd, rs1);
}

static Double GETA_VUnopV(vfmv)(VexGuestRISCV64State *st,
                                ULong vs2, ULong rs1) {
   Double ret = 0;
   RVV0p7_BinopRVX_VF_T("vfmv.f.s", vs2, rs1, , , , , "f");
}

static UInt GETA_VUnopF(vfmvs)(VexGuestRISCV64State *st,
                               ULong vd, ULong rs1, ULong mask,
                               UInt frm) {
   rs1 += (ULong)st;
   RVV0p7_UnopX_I_F_M_PP_T("vfmv.s.f", vd, rs1, , , RVV0p7_Push(), RVV0p7_Pop(), RVV0p7_VF(), "ft0", , );
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
   GETA_VUnopV_M(vmv)         = (Addr)NULL,
   GETA_VUnopX_M(vmv)         = (Addr)NULL,
   GETA_VUnopI_M(vmv)         = (Addr)NULL,
} GETA_NULL;

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
#define GETA_VLdst_M(insn)  RVV0p7_VLdst_##insn##_vm
#define GETN_VLdst_M(insn)  "RVV0p7_VLdst_"#insn"_vm"
#define GETA_VLdst(insn)    RVV0p7_VLdst_##insn
#define GETN_VLdst(insn)    "RVV0p7_VLdst_"#insn

/* Generic template for all vector loads */
#define RVV0p7_Load_Tpl(insn, vm,            \
                        loadMask,            \
                        loadMemory,          \
                        storeGuestState)     \
do {                                         \
      /* Get rs1/vd absolute offset */       \
      ULong rs1_offs = (ULong) st + rs1;     \
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
#define RVV0p7_Store_Tpl(insn, vm,           \
                         loadMask,           \
                         loadGuestState,     \
                         storeMemory)        \
do {                                         \
      /* Get rd/vs absolute offset */        \
      ULong rs1_offs = (ULong) st + rs1;     \
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
      As each mask element in v0 has a width of LMUL/SEW, we need to convert masks to
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
   RVV0p7_Mask();

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
                                            UInt vstart,
                                            UInt nf) {
   IRExpr** maskV = NULL;
   UInt n_addrs   = vl - vstart;
   if (mask)
      return maskV;
   else
      maskV = LibVEX_Alloc_inline(n_addrs * nf * sizeof(IRExpr*));

   UInt idx = 0;
   while (idx < n_addrs) {
      /* Each iteration handles 8 mask elements until all elements consumed. */
      IRTemp   mask_segs = newTemp(irsb, Ity_I64);
      IRExpr** args = mkIRExprVec_3(IRExpr_GSPTR(), mkU64(offsetVReg(0)),
                                    mkU32(vstart + idx));
      IRDirty *m_d = unsafeIRDirty_1_N(mask_segs, 0, "dirty_get_mask",
                                       dirty_get_mask, args);
      UInt n_mask = n_addrs - idx < 8 ? n_addrs - idx : 8;
      for (UInt i = 0; i < n_mask; i++) {
         IRExpr* mask_64 = binop(Iop_And64, binop(Iop_Shr64, mkexpr(mask_segs),
                                                  mkU64(i)), mkU64(1));
         for (UInt field_idx = 0; field_idx < nf; field_idx++, idx++)
            maskV[idx] = unop(Iop_64to1, mask_64);
      }
      stmt(irsb, IRStmt_Dirty(m_d));
   }
   return maskV;
}

/* Dirty helper for obtaining addr info in an index register.
   As the index register is also vector register such that it is 
   beyond 2048 range from guest state pointer, we get its content
   using a helper instead of IRs. */
static ULong dirty_get_indexed_addr(VexGuestRISCV64State *st,
                                    ULong base, UInt idx, UInt sew, ULong vs) {
   ULong vs_off   = (ULong) st + vs + idx * (1 << sew);
   ULong base_off = (ULong) st + base;
   Long  offset;

   switch (sew) {
      case 0:
         offset = *((Char *) vs_off);
         break;
      case 1:
         offset = *((Short *) vs_off);
         break;
      case 2:
         offset = *((Int *) vs_off);
         break;
      case 3:
         offset = *((Long *) vs_off);
      default:
         break;
   }
   return *((ULong *) base_off) + offset;
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
   UInt lmul   = extract_lmul(guest_VFLAG);
   UInt vstart = extract_vstart(guest_VFLAG);
   UInt vl     = extract_vl(guest_VFLAG);
   d->nFxState = 2;
   vex_bzero(&d->fxState, sizeof(d->fxState));

   /* Mark memory effect: address and mask */
   d->mNAddrs     = (vl - vstart) * nf;
   d->mFx         = isLD ? Ifx_Read : Ifx_Write;
   d->mSize       = width;
   IRExpr** addrV = LibVEX_Alloc_inline((vl - vstart) * nf * sizeof(IRExpr*));

   /* Address info */
   UInt idx = 0;
   switch (ldst_ty) {
      case UnitStride:
         for (UInt i = vstart; i < vl; i++)
            for (UInt field_idx = 0; field_idx < nf; field_idx++, idx++)
               addrV[idx] = binop(Iop_Add64, getIReg64(r),
                                  mkU64((i * nf + field_idx) * width));
         break;
      case Strided: {
         for (UInt i = vstart; i < vl; i++) {
            IRExpr* base = binop(Iop_Add64, getIReg64(r),
                                 binop(Iop_Mul64, mkU64(i), getIReg64(s2)));
            for (UInt field_idx = 0; field_idx < nf; field_idx++, idx++)
               addrV[idx] = binop(Iop_Add64, base, mkU64(field_idx * width));
         }
         break;
      }
      case Indexed: {
         UInt sew = extract_sew(guest_VFLAG);
         for (UInt i = vstart; i < vl; i++) {
            IRTemp   addr = newTemp(irsb, Ity_I64);
            IRExpr** args = mkIRExprVec_5(IRExpr_GSPTR(), mkU64(offsetIReg64(r)),
                                          mkU32(i), mkU32(sew), mkU64(offsetVReg(s2)));
            IRDirty *a_d = unsafeIRDirty_1_N(addr, 0, "dirty_get_indexed_addr",
                                             dirty_get_indexed_addr, args);
            /* Record addresses per element in a segment */
            for (UInt field_idx = 0; field_idx < nf; field_idx++, idx++)
               addrV[idx] = binop(Iop_Add64, mkexpr(addr), mkU64(field_idx * width));
            stmt(irsb, IRStmt_Dirty(a_d));
         }
         break;
      }
      default:
         vassert(0);
   }
   d->mAddrVec = addrV;

   /* Mask info */
   d->mMask = calculate_dirty_mask(irsb, mask, vl, vstart, nf);

   /* Mark vector register modified */
   d->fxState[0].fx        = isLD ? Ifx_Write : Ifx_Read;
   d->fxState[0].offset    = offsetVReg(v);
   d->fxState[0].size      = host_VLENB;
   d->fxState[0].nRepeats  = lmul;
   d->fxState[0].repeatLen = host_VLENB;

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
         d->fxState[2].fx     = Ifx_Read;
         d->fxState[2].offset = offsetVReg(s2);
         d->fxState[2].size   = host_VLENB;
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
      fName = mask ? GETN_VLdst(insn) : GETN_VLdst_M(insn);  \
      fAddr = mask ? GETA_VLdst(insn) : GETA_VLdst_M(insn);  \
      mk_c_args                                              \
      d = unsafeIRDirty_0_N(0, fName, fAddr, args);          \
      d = mk_getd                                            \
      stmt(irsb, IRStmt_Dirty(d));                           \
                                                             \
      mk_dip                                                 \
   } while (0)

/* Factory for all load/stores, vertically list macro parameters for better readability */
#define RVV0p7_Load_Memory(insn, vm)
#define RVV0p7_Store_GuestState(nf, vm)
#define RVV0p7_Load_GuestState(nf, vm)
#define RVV0p7_Store_Memory(insn, vm)

#define DIRTY_VLOAD_BODY_VM(insn, nf)                                        \
   RVV0p7_Load_Tpl(insn, ",v0.t",                         /* insn info */    \
                   RVV0p7_Mask(),                         /* load mask*/     \
                   RVV0p7_Load_Memory(insn, ",v0.t"),     /* load memory */  \
                   RVV0p7_Store_GuestState(nf, ",v0.t")); /* store GS */     \

#define DIRTY_VLOAD_BODY(insn, nf)                                           \
   RVV0p7_Load_Tpl(insn, ,                                /* insn info */    \
                   ,                                      /* load mask*/     \
                   RVV0p7_Load_Memory(insn, ),            /* load memory */  \
                   RVV0p7_Store_GuestState(nf, ));        /* store GS */     \

#define DIRTY_VSTORE_BODY_VM(insn, nf)                                  \
   RVV0p7_Store_Tpl(insn, ",v0.t",                                      \
                    RVV0p7_Mask(),                                      \
                    RVV0p7_Load_GuestState(nf, ",v0.t"),                \
                    RVV0p7_Store_Memory(insn, ",v0.t"));

#define DIRTY_VSTORE_BODY(insn, nf)                                     \
   RVV0p7_Store_Tpl(insn, ,                                             \
                    ,                                                   \
                    RVV0p7_Load_GuestState(nf,),                        \
                    RVV0p7_Store_Memory(insn, ));

#define RVV0p7_VLdst_Generic(insn, body, nf)                             \
   static void RVV0p7_VLdst_##insn##_vm(VexGuestRISCV64State *st,        \
                               UInt v, UInt rs1, ULong mask) {           \
      body##_VM(insn, nf)                                                \
   }                                                                     \
   static void RVV0p7_VLdst_##insn(VexGuestRISCV64State *st,             \
                               UInt v, UInt rs1, ULong mask) {           \
      body(insn, nf)                                                     \
   }

#define RVV0p7_VSXLdst_Generic(insn, body, nf)                           \
   static void RVV0p7_VLdst_##insn##_vm(VexGuestRISCV64State *st,        \
                               UInt v, UInt rs1, UInt s2, ULong mask) {  \
      body##_VM(insn, nf)                                                \
   }                                                                     \
   static void RVV0p7_VLdst_##insn(VexGuestRISCV64State *st,             \
                               UInt v, UInt rs1, UInt s2, ULong mask) {  \
      body(insn, nf)                                                     \
   }

#define RVV0p7_VLdst(insn, body)           RVV0p7_VLdst_Generic(insn, body, )
#define RVV0p7_VSXLdst(insn, body)         RVV0p7_VSXLdst_Generic(insn, body, )
#define RVV0p7_VSEGLdst(insn, body, nf)    RVV0p7_VLdst_Generic(insn, body, nf)
#define RVV0p7_VSEGSXLdst(insn, body, nf)  RVV0p7_VSXLdst_Generic(insn, body, nf)

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

#define RVV0p7_Unit_Stride_Load_Store_Memory(insn, vm) \
   /* vload1: Load from memory */                      \
   __asm__ __volatile__ (                              \
      #insn ".v\tv8,(%0)\t" vm "\n\t"                  \
      ::"r" (rs1_offs)                                 \
      :                                                \
   );

/* Store to guest state for unit-stride(U), strided(S), and indexed(X) */
#define RVV0p7_USX_Store_GuestState(nf, vm)  \
   /* vstore1: Store to GuestState */        \
   __asm__ __volatile__ (                    \
      "vse.v\tv8,(%0)\t" vm "\n\t"           \
      ::"r" (vd_offs)                        \
      :                                      \
   );

#undef  RVV0p7_Load_Memory
#undef  RVV0p7_Store_GuestState
#define RVV0p7_Load_Memory       RVV0p7_Unit_Stride_Load_Store_Memory
#define RVV0p7_Store_GuestState  RVV0p7_USX_Store_GuestState

RVV0p7_VLdst(vlb,  DIRTY_VLOAD_BODY)
RVV0p7_VLdst(vlh,  DIRTY_VLOAD_BODY)
RVV0p7_VLdst(vlw,  DIRTY_VLOAD_BODY)
RVV0p7_VLdst(vle,  DIRTY_VLOAD_BODY)
RVV0p7_VLdst(vlbu, DIRTY_VLOAD_BODY)
RVV0p7_VLdst(vlhu, DIRTY_VLOAD_BODY)
RVV0p7_VLdst(vlwu, DIRTY_VLOAD_BODY)

/*----------------------------------------------------------*/
/*---   1.2 Unit-stride vector store dirty helpers       ---*/
/*----------------------------------------------------------*/

#define RVV0p7_USX_Load_GuestState(nf, vm)     \
   __asm__ __volatile__ (                      \
      "vle.v\tv8,(%0)\t" vm "\n\t"             \
      ::"r" (vs_offs)                          \
      :                                        \
   );

#undef  RVV0p7_Load_GuestState
#undef  RVV0p7_Store_Memory
#define RVV0p7_Load_GuestState  RVV0p7_USX_Load_GuestState
#define RVV0p7_Store_Memory     RVV0p7_Unit_Stride_Load_Store_Memory

RVV0p7_VLdst(vsb, DIRTY_VSTORE_BODY)
RVV0p7_VLdst(vsh, DIRTY_VSTORE_BODY)
RVV0p7_VLdst(vsw, DIRTY_VSTORE_BODY)
RVV0p7_VLdst(vse, DIRTY_VSTORE_BODY)

/*--------------------------------------------------------------------*/
/*--- end                               guest_riscv64V0p7_helpers.c --*/
/*--------------------------------------------------------------------*/
