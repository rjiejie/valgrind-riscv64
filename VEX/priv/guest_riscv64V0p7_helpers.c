
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

#define GETC_VBinopOP_T(insn, V, X, I, REGO, REGN, ARGS)                       \
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
      ARGS()                                                                   \
      d = unsafeIRDirty_1_N(ret, 0, fName, fAddr, args);                       \
      d = GETD_VBinop(d, rd, rs2, rs1, mask, GET_FUNCT3());                    \
      stmt(irsb, IRStmt_Dirty(d));                                             \
                                                                               \
      if (isVOpVI(GET_FUNCT3()))                                               \
         DIP("%s(%s, %s, %u)\n", fName, nameVReg(rd), nameVReg(rs2), rs1);     \
      else                                                                     \
         DIP("%s(%s, %s, %s)\n", fName, nameVReg(rd), nameVReg(rs2),           \
             isVOpVV(GET_FUNCT3()) ? nameVReg(rs1) : REGN(rs1));               \
   } while (0)

#define GETR_VBinopOPI()                                                       \
   args = mkIRExprVec_5(IRExpr_GSPTR(), mkU64(offsetVReg(rd)),                 \
                        mkU64(offsetVReg(rs2)), mkU64(temp),                   \
                        mkU64(offsetVReg(0)));
#define GETC_VBinopOPI(insn)                                                   \
   GETC_VBinopOP_T(insn, V, X, I, offsetIReg64, nameIReg, GETR_VBinopOPI)

static IRDirty*
GETD_VBinop(IRDirty* d, UInt vd, UInt vs2, UInt vs1, Bool mask, UInt sopc)
{
   /* TODO */
   UInt lmul   = 0;
   d->nFxState = isVOpVV(sopc) ? 3 : 2;
   vex_bzero(&d->fxState, sizeof(d->fxState));

   UInt regNos[3] = {vd, vs2, vs1};
   for (int i = 0; i < d->nFxState; i++) {
      d->fxState[i].fx     = i == 0 ? Ifx_Write : Ifx_Read;
      d->fxState[i].offset = offsetVReg(regNos[i]);
      d->fxState[i].size   = host_VLENB;
      d->fxState[i].nRepeats = lmul;
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

#define RVV0p7_Push()         \
   __asm__ __volatile__(      \
      "csrr\tt1,vl\n\t"       \
      "csrr\tt2,vtype\n\t"    \
      "vsetvl\tx0,x0,t2\n\t"  \
      ::: "t1", "t2");

#define RVV0p7_Pop()          \
   __asm__ __volatile__(      \
      "vsetvl\tx0,t1,t2\n\t"  \
      :::);

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

#define RVV0p7_VX() \
   __asm__ __volatile__("ld\tt0,0(%0)\n\t"::"r"(rs1):"t0");
#define RVV0p7_VI() \
   __asm__ __volatile__("mv\tt0,%0\n\t"::"r"(rs1):"t0");
#define RVV0p7_VF() \
   __asm__ __volatile__("fld\tft0,0(%0)\n\t"::"r"(rs1):"ft0");

#define RVV0p7_PushFCSR()           \
   __asm__ __volatile__(            \
      "csrr\tt1,fcsr\n\t"           \
      "csrw\tfrm,%0\n\t"            \
      "csrw\tfflags,x0\n\t"         \
      :                             \
      : "r"(frm)                    \
      : "t1");
#define RVV0p7_PopFCSR()            \
   __asm__ __volatile__(            \
      "csrr\t%0,fflags\n\t"         \
      "csrw\tfcsr,t1\n\t"           \
      : "=r"(ret)                   \
      :                             \
      :);

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
#define RVV0p7_BinopOPIVV_M_T(insn, vd, vs2, vs1)\
   RVV0p7_BinopVV_M_PP_T(insn, vd, vs2, vs1, RVV0p7_Mask(), ",v0.t", , , , )
#define RVV0p7_BinopOPIVV_T(insn, vd, vs2, vs1)\
   RVV0p7_BinopVV_M_PP_T(insn, vd, vs2, vs1, , , , , , )
#define RVV0p7_BinopOPFVV_M_T(insn, vd, vs2, vs1)\
   RVV0p7_BinopVV_M_PP_T(insn, vd, vs2, vs1, RVV0p7_Mask(), ",v0.t", , , RVV0p7_PushFCSR(), RVV0p7_PopFCSR())
#define RVV0p7_BinopOPFVV_T(insn, vd, vs2, vs1)\
   RVV0p7_BinopVV_M_PP_T(insn, vd, vs2, vs1, , , , , RVV0p7_PushFCSR(), RVV0p7_PopFCSR())

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
#define RVV0p7_BinopVX_M_T(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, RVV0p7_Mask(), ",v0.t", , , RVV0p7_VX(), "t0", , )
#define RVV0p7_BinopVX_T(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, , , , , RVV0p7_VX(), "t0", , )
#define RVV0p7_BinopVI_M_T(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, RVV0p7_Mask(), ",v0.t", , , RVV0p7_VI(), "t0", , )
#define RVV0p7_BinopVI_T(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, , , , , RVV0p7_VI(), "t0", , )
#define RVV0p7_BinopVF_M_T(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, RVV0p7_Mask(), ",v0.t", , , RVV0p7_VF(), "ft0", RVV0p7_PushFCSR(), RVV0p7_PopFCSR())
#define RVV0p7_BinopVF_T(insn, vd, vs2, rs1)\
   RVV0p7_BinopVX_VI_VF_M_PP_T(insn, vd, vs2, rs1, , , , , RVV0p7_VF(), "ft0", RVV0p7_PushFCSR(), RVV0p7_PopFCSR())

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

#define RVV0p7_BinopOPIVV_VX_FT(insn) \
   RVV0p7_BinopOPIVV_FT(insn) \
   RVV0p7_BinopVX_FT(insn) \

#define RVV0p7_BinopOPIVV_VX_VI_FT(insn) \
   RVV0p7_BinopOPIVV_VX_FT(insn) \
   RVV0p7_BinopVI_FT(insn) \

RVV0p7_BinopOPIVV_VX_VI_FT(vadd)

#define GETN_VBinopVF_M(insn)  "RVV0p7_Binop_"#insn"vf_m"
#define GETA_VBinopVF_M(insn)  RVV0p7_Binop_##insn##vf_m
#define GETN_VBinopVF(insn)    "RVV0p7_Binop_"#insn"vf"
#define GETA_VBinopVF(insn)    RVV0p7_Binop_##insn##vf

#define GETR_VBinopOPF()                                                       \
   assign(irsb, frm,                                                           \
          binop(Iop_And32, binop(Iop_Shr32, getFCSR(), mkU8(5)), mkU32(7)));   \
   args = mkIRExprVec_6(IRExpr_GSPTR(), mkU64(offsetVReg(rd)),                 \
                        mkU64(offsetVReg(rs2)), mkU64(temp),                   \
                        mkU64(offsetVReg(0)), mkexpr(frm));
#define GETC_VBinopOPF(insn)                                                   \
   GETC_VBinopOP_T(insn, V, F, F, offsetFReg, nameFReg, GETR_VBinopOPF)
#define GETC_VBinopOPF_F(insn)                                                 \
   GETC_VBinopOP_T(insn, F, F, F, offsetFReg, nameFReg, GETR_VBinopOPF)

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

#define RVV0p7_BinopOPFVV_VF_FT(insn) \
   RVV0p7_BinopOPFVV_FT(insn) \
   RVV0p7_BinopVF_FT(insn) \

RVV0p7_BinopOPFVV_VF_FT(vfadd)
RVV0p7_BinopOPFVV_VF_FT(vfsub)
RVV0p7_BinopVF_FT(vfrsub)
RVV0p7_BinopOPFVV_VF_FT(vfmul)
RVV0p7_BinopOPFVV_VF_FT(vfdiv)
RVV0p7_BinopVF_FT(vfrdiv)

/*--------------------------------------------------------------------*/
/*--- end                               guest_riscv64V0p7_helpers.c --*/
/*--------------------------------------------------------------------*/
