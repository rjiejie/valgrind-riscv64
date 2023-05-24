
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

#define GETC_VBinopOPI(insn) \
   do {                                                                    \
      if (isVOpVV(GET_FUNCT3())) {                                         \
         fName = mask ? GETN_VBinopVV(insn) : GETN_VBinopVV_M(insn);       \
         fAddr = mask ? GETA_VBinopVV(insn) : GETA_VBinopVV_M(insn);       \
         temp  = offsetVReg(rs1);                                          \
      } else if (isVOpVX(GET_FUNCT3())) {                                  \
         fName = mask ? GETN_VBinopVX(insn) : GETN_VBinopVX_M(insn);       \
         fAddr = mask ? GETA_VBinopVX(insn) : GETA_VBinopVX_M(insn);       \
         temp  = offsetIReg64(rs1);                                        \
      } else {                                                             \
         fName = mask ? GETN_VBinopVI(insn) : GETN_VBinopVI_M(insn);       \
         fAddr = mask ? GETA_VBinopVI(insn) : GETA_VBinopVI_M(insn);       \
         temp  = rs1;                                                      \
      }                                                                    \
      args = mkIRExprVec_5(IRExpr_GSPTR(),                                 \
                           mkU64(offsetVReg(rd)), mkU64(offsetVReg(rs2)),  \
                           mkU64(temp), mkU64(offsetVReg(0)));             \
      d = unsafeIRDirty_0_N(0, fName, fAddr, args);                        \
      d = SETD_VBinop(d, rd, rs2, rs1, mask, GET_FUNCT3());                \
      stmt(irsb, IRStmt_Dirty(d));                                         \
                                                                           \
      if (isVOpVI(GET_FUNCT3()))                                           \
         DIP("%s(%s, %s, %u)\n", fName, nameVReg(rd), nameVReg(rs2), rs1); \
      else                                                                 \
         DIP("%s(%s, %s, %s)\n", fName, nameVReg(rd), nameVReg(rs2),       \
             isVOpVV(GET_FUNCT3()) ? nameVReg(rs1) : nameIReg(rs1));       \
   } while (0)

static IRDirty*
SETD_VBinop(IRDirty* d, UInt vd, UInt vs2, UInt vs1, Bool mask, UInt sopc)
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

   if (isVOpVX(sopc)) {
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
      "mv\tt0,%0\n\t"               \
      "vle.v\tv0,(t0)\n\t"          \
      "vsetvl\tx0,t1,t2\n\t"        \
      :                             \
      : "r"(mask)                   \
      : "t0", "t1", "t2");

#define RVV0p7_VX() \
   __asm__ __volatile__("ld\tt0,0(t0)\n\t":::"t0");

// v8-v15, v16-v23, v24-v31
#define RVV0p7_BinopVV_T(insn, vd, vs2, vs1, mask, mreg, push, pop)\
   do {                                                     \
      vd  += (ULong)st;                                       \
      vs2 += (ULong)st;                                       \
      vs1 += (ULong)st;                                       \
                                                            \
      push                                                  \
      __asm__ __volatile__(                                 \
         "mv\tt0,%0\n\t"                                    \
         "vle.v\tv8,(t0)\n\t"                               \
         "mv\tt0,%1\n\t"                                    \
         "vle.v\tv16,(t0)\n\t"                              \
         "mv\tt0,%2\n\t"                                    \
         "vle.v\tv24,(t0)\n\t"                              \
         :                                                  \
         : "r"(vd), "r"(vs2), "r"(vs1)                      \
         : "t0");                                           \
      pop                                                   \
                                                            \
      mask                                                  \
      __asm__ __volatile__(insn "\tv8,v16,v24" mreg);       \
                                                            \
      push                                                  \
      __asm__ __volatile__(                                 \
         "mv\tt0,%0\n\t"                                    \
         "vse.v\tv8,(t0)\n\t"                               \
         :                                                  \
         : "r"(vd)                                          \
         : "t0", "memory");                                 \
      pop                                                   \
   } while (0)

// v8-v15, v16-v23, t0
#define RVV0p7_BinopVX_T(insn, vd, vs2, rs1, mask, mreg, push, pop, isvx)\
   do {                                                     \
      vd  += (ULong)st;                                       \
      vs2 += (ULong)st;                                       \
                                                            \
      push                                                  \
      __asm__ __volatile__(                                 \
         "mv\tt0,%0\n\t"                                    \
         "vle.v\tv8,(t0)\n\t"                               \
         "mv\tt0,%1\n\t"                                    \
         "vle.v\tv16,(t0)\n\t"                              \
         :                                                  \
         : "r"(vd), "r"(vs2)                                \
         : "t0");                                           \
      pop                                                   \
                                                            \
      mask                                                  \
      __asm__ __volatile__("mv\tt0,%0\n\t"::"r"(rs1):"t0"); \
      isvx                                                  \
      __asm__ __volatile__(insn "\tv8,v16,t0" mreg);        \
                                                            \
      push                                                  \
      __asm__ __volatile__(                                 \
         "mv\tt0,%0\n\t"                                    \
         "vse.v\tv8,(t0)\n\t"                               \
         :                                                  \
         : "r"(vd)                                          \
         : "t0", "memory");                                 \
      pop                                                   \
   } while (0)

#define RVV0p7_BinopVV_FT(insn) \
   static void RVV0p7_Binop_##insn##vv_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong vs1, ULong mask) { \
      RVV0p7_BinopVV_T(#insn".vv", vd, vs2, vs1, RVV0p7_Mask(), ",v0.t",,); \
   } \
   static void RVV0p7_Binop_##insn##vv(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong vs1, ULong mask) { \
      RVV0p7_BinopVV_T(#insn".vv", vd, vs2, vs1,,,,); \
   } \

#define RVV0p7_BinopVX_FT(insn) \
   static void RVV0p7_Binop_##insn##vx_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong rs1, ULong mask) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopVX_T(#insn".vx", vd, vs2, rs1, RVV0p7_Mask(), ",v0.t",,,RVV0p7_VX()); \
   } \
   static void RVV0p7_Binop_##insn##vx(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong rs1, ULong mask) { \
      rs1 += (ULong)st; \
      RVV0p7_BinopVX_T(#insn".vx", vd, vs2, rs1,,,,,RVV0p7_VX()); \
   } \

#define RVV0p7_BinopVI_FT(insn) \
   static void RVV0p7_Binop_##insn##vi_m(VexGuestRISCV64State *st, \
                                         ULong vd, ULong vs2, ULong rs1, ULong mask) { \
      RVV0p7_BinopVX_T(#insn".vx", vd, vs2, rs1, RVV0p7_Mask(), ",v0.t",,,); \
   } \
   static void RVV0p7_Binop_##insn##vi(VexGuestRISCV64State *st, \
                                       ULong vd, ULong vs2, ULong rs1, ULong mask) { \
      RVV0p7_BinopVX_T(#insn".vx", vd, vs2, rs1,,,,,); \
   } \

#define RVV0p7_BinopVV_VX_FT(insn) \
   RVV0p7_BinopVV_FT(insn) \
   RVV0p7_BinopVX_FT(insn) \

#define RVV0p7_BinopVV_VX_VI_FT(insn) \
   RVV0p7_BinopVV_VX_FT(insn) \
   RVV0p7_BinopVI_FT(insn) \

RVV0p7_BinopVV_VX_VI_FT(vadd)

/*--------------------------------------------------------------------*/
/*--- end                               guest_riscv64V0p7_helpers.c --*/
/*--------------------------------------------------------------------*/
