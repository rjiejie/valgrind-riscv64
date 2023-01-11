
/*--------------------------------------------------------------------*/
/*--- begin                                guest_riscv64Zfh_toIR.c ---*/
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

static Bool dis_RV64Zfh(/*MB_OUT*/ DisResult* dres,
                        /*OUT*/ IRSB*         irsb,
                        UInt                  insn)
{
   Bool ok = False;

#ifdef __riscv_zfh

   if (GET_OPCODE() == OPC_LOAD_FP && GET_FUNCT3() == 0b001) {
      UInt  rd      = GET_RD();
      UInt  rs1     = GET_RS1();
      UInt  imm12   = INSN(31, 20);

      ULong offs = vex_sx_to_64(imm12, 12);
      putFReg16(irsb, rd,
                loadLE(Ity_F16, binop(Iop_Add64, getIReg64(rs1), mkU64(offs))));
      DIP("flh %s,%lld(%s)\n", nameFReg(rd), (Long)offs, nameIReg(rs1));
      return True;
   }

   if (GET_OPCODE() == OPC_STORE_FP && GET_FUNCT3() == 0b001) {
      UInt  rs1     = GET_RS1();
      UInt  rs2     = GET_RS2();
      UInt  imm12   = INSN(31, 25) << 5 | INSN(11, 7);

      ULong offs = vex_sx_to_64(imm12, 12);
      storeLE(irsb, binop(Iop_Add64, getIReg64(rs1), mkU64(offs)),
              getFReg16(rs2));
      DIP("fsh %s,%lld(%s)\n", nameFReg(rs2), (Long)offs, nameIReg(rs1));
      return True;
   }

   if ((GET_OPCODE() == OPC_MADD  || GET_OPCODE() == OPC_MSUB ||
        GET_OPCODE() == OPC_NMADD || GET_OPCODE() == OPC_NMSUB)
       && INSN(26, 25) == RV64_FMT_FH) {
      UInt  rd      = GET_RD();
      UInt  rs1     = GET_RS1();
      UInt  rs2     = GET_RS2();
      UInt  rs3     = INSN(31, 27);
      UInt  rm      = GET_FUNCT3();

      IRTemp rmEncd, rmIR;
      mk_get_rounding_mode(irsb, &rmEncd, &rmIR, rm);
      IRTemp  res   = newTemp(irsb, Ity_F16);
      IRExpr* eR1   = getFReg16(rs1);
      IRExpr* eR2   = getFReg16(rs2);
      IRExpr* eR3   = getFReg16(rs3);
      IRExpr* eRM   = mkexpr(rmIR);

      const HChar* opcs[4] = { "fmadd", "fmsub", "fnmsub", "fnmadd" };
      const UInt   opci    = INSN(3, 2);

      switch (GET_OPCODE()) {
         case OPC_MADD:
            assign(irsb, res, qop(Iop_MAddF16, eRM, eR1, eR2, eR3));
            break;
         case OPC_MSUB:
            assign(irsb, res, qop(Iop_MAddF16, eRM, eR1, eR2, unop(Iop_NegF16, eR3)));
            break;
         case OPC_NMADD:
            assign(irsb, res, qop(Iop_MAddF16, eRM, unop(Iop_NegF16, eR1), eR2,
                                  unop(Iop_NegF16, eR3)));
            break;
         case OPC_NMSUB:
            assign(irsb, res, qop(Iop_MAddF16, eRM, unop(Iop_NegF16, eR1), eR2, eR3));
            break;
         default:
            vassert(0);
      }
      putFReg16(irsb, rd, mkexpr(res));
      accumulateFFLAGS(
         irsb,
         mkIRExprCCall(
            Ity_I32, 0 /*regparms*/, "riscv64g_calculate_fflags_fmadd_h",
            riscv64g_calculate_fflags_fmadd_h,
            mkIRExprVec_4(eR1, eR2, eR3, mkexpr(rmEncd))));
      DIP("%s.h %s,%s,%s,%s%s\n", opcs[opci], nameFReg(rd), nameFReg(rs1),
          nameFReg(rs2), nameFReg(rs3), nameRMOperand(rm));
      return True;
   }

   if ((GET_FUNCT5() == RV64_SOPC_FADD || GET_FUNCT5() == RV64_SOPC_FSUB ||
        GET_FUNCT5() == RV64_SOPC_FMUL || GET_FUNCT5() == RV64_SOPC_FDIV)
       && GET_OPCODE() == OPC_OP_FP && INSN(26, 25) == RV64_FMT_FH) {
      UInt  rd      = GET_RD();
      UInt  rs1     = GET_RS1();
      UInt  rs2     = GET_RS2();
      UInt  rm      = GET_FUNCT3();

      IRTemp rmEncd, rmIR;
      mk_get_rounding_mode(irsb, &rmEncd, &rmIR, rm);
      IRTemp  res   = newTemp(irsb, Ity_F16);
      IRExpr* eR1   = getFReg16(rs1);
      IRExpr* eR2   = getFReg16(rs2);
      IRExpr* eRM   = mkexpr(rmIR);

      void* helpers[4]
         = { &riscv64g_calculate_fflags_fadd_h, &riscv64g_calculate_fflags_fadd_h,
             &riscv64g_calculate_fflags_fmul_h, &riscv64g_calculate_fflags_fdiv_h };
      const HChar* hNames[4]
         = { "riscv64g_calculate_fflags_fadd_h", "riscv64g_calculate_fflags_fadd_h",
             "riscv64g_calculate_fflags_fmul_h", "riscv64g_calculate_fflags_fdiv_h" };
      const HChar* opcs[4] = { "fadd", "fsub", "fmul", "fdiv" };
      const UInt   opci    = INSN(28,27);

      switch (GET_FUNCT7()) {
         case RV64_SOPC_FADD:
            assign(irsb, res, triop(Iop_AddF16, eRM, eR1, eR2));
            break;
         case RV64_SOPC_FSUB:
            assign(irsb, res, triop(Iop_AddF16, eRM, eR1, unop(Iop_NegF16, eR2)));
            break;
         case RV64_SOPC_FMUL:
            assign(irsb, res, triop(Iop_MulF16, eRM, eR1, eR2));
            break;
         case RV64_SOPC_FDIV:
            assign(irsb, res, triop(Iop_DivF16, eRM, eR1, eR2));
            break;
         default:
            vassert(0);
      }
      putFReg16(irsb, rd, mkexpr(res));
      accumulateFFLAGS(
         irsb, mkIRExprCCall(Ity_I32, 0 /*regparms*/, hNames[opci], helpers[opci],
                             mkIRExprVec_3(eR1, eR2, mkexpr(rmEncd))));
      DIP("%s.h %s,%s,%s%s\n", opcs[opci], nameFReg(rd), nameFReg(rs1),
          nameFReg(rs2), nameRMOperand(rm));
      return True;
   }

   if ((GET_FUNCT5() == RV64_SOPC_FSQRT && GET_RS2() == 0)
       && GET_OPCODE() == OPC_OP_FP && INSN(26, 25) == RV64_FMT_FH) {
      UInt  rd      = GET_RD();
      UInt  rs1     = GET_RS1();
      UInt  rm      = GET_FUNCT3();

      IRTemp rmEncd, rmIR;
      mk_get_rounding_mode(irsb, &rmEncd, &rmIR, rm);
      putFReg16(irsb, rd, binop(Iop_SqrtF16, mkexpr(rmIR), getFReg16(rs1)));
      accumulateFFLAGS(
         irsb, mkIRExprCCall(Ity_I32, 0 /*regparms*/,
                             "riscv64g_calculate_fflags_fsqrt_h",
                             riscv64g_calculate_fflags_fsqrt_h,
                             mkIRExprVec_2(getFReg16(rs1), mkexpr(rmEncd))));
      DIP("fsqrt.h %s,%s%s\n", nameFReg(rd), nameFReg(rs1), nameRMOperand(rm));
      return True;
   }

   if ((GET_FUNCT3() == 0 || GET_FUNCT3() == 1 || GET_FUNCT3() == 2)
       && GET_FUNCT5() == RV64_SOPC_FSGNJ && GET_OPCODE() == OPC_OP_FP
       && INSN(26, 25) == RV64_FMT_FH) {
      UInt  rd      = GET_RD();
      UInt  rs1     = GET_RS1();
      UInt  rs2     = GET_RS2();
      UInt  opc     = GET_FUNCT3();

      IRExpr* eR1 = getFReg16(rs1);
      IRExpr* eR2 = getFReg16(rs2);

      if (rs1 == rs2) {
         const HChar* opcs[3] = { "fmv", "fneg", "fabs" };
         switch (opc) {
            case 0: putFReg16(irsb, rd, eR1); break;
            case 1: putFReg16(irsb, rd, unop(Iop_NegF16, eR1)); break;
            case 2: putFReg16(irsb, rd, unop(Iop_AbsF16, eR1)); break;
            default: vassert(0);
         }
         DIP("%s.h %s,%s\n", opcs[opc], nameFReg(rd), nameFReg(rs1));
         return True;
      }

      const HChar* opcs[3] = { "fsgnj", "fsgnjn", "fsgnjx" };
      putFReg16(irsb, rd,
                unop(Iop_ReinterpI16asF16,
                     binop(opc == 2 ? Iop_Xor16 : Iop_Or16,
                           binop(Iop_And16, unop(Iop_ReinterpF16asI16, eR1),
                                 mkU16(0X7FFF)),
                           binop(Iop_And16, unop(Iop_ReinterpF16asI16, opc == 1 ? unop(Iop_NegF16, eR2) : eR2),
                                 mkU16(0x8000)))));
      DIP("%s.h %s,%s,%s\n", opcs[opc], nameFReg(rd), nameFReg(rs1), nameFReg(rs2));
      return True;
   }

   if ((GET_FUNCT3() == 0 || GET_FUNCT3() == 1)
       && GET_FUNCT5() == RV64_SOPC_FMIN_MAX && GET_OPCODE() == OPC_OP_FP
       && INSN(26, 25) == RV64_FMT_FH) {
      UInt  rd      = GET_RD();
      UInt  rs1     = GET_RS1();
      UInt  rs2     = GET_RS2();
      UInt  opc     = GET_FUNCT3();

      IRExpr* eR1 = getFReg16(rs1);
      IRExpr* eR2 = getFReg16(rs2);

      void* helpers[2]
         = { &riscv64g_calculate_fflags_fmin_h, &riscv64g_calculate_fflags_fmax_h };
      const HChar* hNames[2]
         = { "riscv64g_calculate_fflags_fmin_h", "riscv64g_calculate_fflags_fmax_h" };
      const HChar* opcs[2] = { "fmin", "fmax" };

      putFReg16(irsb, rd,
                binop(opc == 0 ? Iop_MinNumF16 : Iop_MaxNumF16, eR1, eR2));
      accumulateFFLAGS(
         irsb, mkIRExprCCall(Ity_I32, 0 /*regparms*/, hNames[opc], helpers[opc],
                             mkIRExprVec_2(eR1, eR2)));
      DIP("%s.h %s,%s,%s\n", opcs[opc], nameFReg(rd), nameFReg(rs1), nameFReg(rs2));
      return True;
   }

#define RV64_SOPC_FCVT_F    0b01000
#define RV64_SOPC_FCVT_S_H  0b0000010
#define RV64_SOPC_FCVT_H_S  0b1000000
#define RV64_SOPC_FCVT_D_H  0b0100010
#define RV64_SOPC_FCVT_H_D  0b1000001

   if ((INSN(26, 20) == RV64_SOPC_FCVT_S_H || INSN(26, 20) == RV64_SOPC_FCVT_H_S ||
        INSN(26, 20) == RV64_SOPC_FCVT_D_H || INSN(26, 20) == RV64_SOPC_FCVT_H_D)
       && GET_FUNCT5() == RV64_SOPC_FCVT_F && GET_OPCODE() == OPC_OP_FP) {
      UInt  rd      = GET_RD();
      UInt  rs1     = GET_RS1();
      UInt  rm      = GET_FUNCT3();
      UInt  opc     = INSN(26, 20);

      IRTemp rmEncd, rmIR;
      mk_get_rounding_mode(irsb, &rmEncd, &rmIR, rm);
      IRExpr* eR1 = getFReg16(rs1);
      IRExpr* eRM = mkexpr(rmIR);

      HChar* hName = NULL;
      HChar* helper = NULL;
      const HChar* opcodestr = "???";
      switch (opc) {
         case RV64_SOPC_FCVT_S_H:
            opcodestr = "fcvt.s.h";
            putFReg32(irsb, rd, unop(Iop_F16toF32, eR1));
            break;
         case RV64_SOPC_FCVT_D_H:
            opcodestr = "fcvt.d.h";
            putFReg64(irsb, rd, unop(Iop_F16toF64, eR1));
            break;
         case RV64_SOPC_FCVT_H_S:
            opcodestr = "fcvt.h.s";
            putFReg16(irsb, rd, binop(Iop_F32toF16, eRM, eR1));
            hName  = "riscv64g_calculate_fflags_fcvt_h_s";
            helper = riscv64g_calculate_fflags_fcvt_h_s;
            break;
         case RV64_SOPC_FCVT_H_D:
            opcodestr = "fcvt.h.d";
            putFReg16(irsb, rd, binop(Iop_F64toF16, eRM, eR1));
            hName  = "riscv64g_calculate_fflags_fcvt_h_d";
            helper = riscv64g_calculate_fflags_fcvt_h_d;
            break;
         default:
            vassert(0);
      }
      if (hName) {
         accumulateFFLAGS(
            irsb, mkIRExprCCall(Ity_I32, 0 /*regparms*/, hName, helper,
                                mkIRExprVec_2(eR1, mkexpr(rmEncd))));
      }
      DIP("%s %s,%s%s\n", opcodestr, nameFReg(rd), nameFReg(rs1), nameRMOperand(rm));
      return True;
   }

#define RV64_SOPC_FCMP 0b1010010

   if ((GET_FUNCT3() == 0 || GET_FUNCT3() == 1 || GET_FUNCT3() == 2)
       && GET_FUNCT7() == RV64_SOPC_FCMP && GET_OPCODE() == OPC_OP_FP) {
      UInt  rd      = GET_RD();
      UInt  rs1     = GET_RS1();
      UInt  rs2     = GET_RS2();
      UInt  opc     = GET_FUNCT3();

      IRTemp cmp = newTemp(irsb, Ity_I32);
      IRTemp res = newTemp(irsb, Ity_I1);
      IRExpr* eR1 = getFReg16(rs1);
      IRExpr* eR2 = getFReg16(rs2);

      void* helpers[3]
         = { &riscv64g_calculate_fflags_fle_h, &riscv64g_calculate_fflags_flt_h,
             &riscv64g_calculate_fflags_feq_h };
      const HChar* hNames[3]
         = { "riscv64g_calculate_fflags_fle_h", "riscv64g_calculate_fflags_flt_h",
             "riscv64g_calculate_fflags_feq_h" };
      const HChar* opcs[3] = { "fle", "flt", "feq" };

      assign(irsb, cmp, binop(Iop_CmpF16, eR1, eR2));
      switch (opc) {
         case 0:
            assign(irsb, res,
                   binop(Iop_Or1, binop(Iop_CmpEQ32, mkexpr(cmp), mkU32(Ircr_LT)),
                                  binop(Iop_CmpEQ32, mkexpr(cmp), mkU32(Ircr_EQ))));
            break;
         case 1:
            assign(irsb, res, binop(Iop_CmpEQ32, mkexpr(cmp), mkU32(Ircr_LT)));
            break;
         case 2:
            assign(irsb, res, binop(Iop_CmpEQ32, mkexpr(cmp), mkU32(Ircr_EQ)));
            break;
         default:
            vassert(0);
      }
      putIReg64(irsb, rd, unop(Iop_1Uto64, mkexpr(res)));
      accumulateFFLAGS(
         irsb, mkIRExprCCall(Ity_I32, 0 /*regparms*/, hNames[opc], helpers[opc],
                             mkIRExprVec_2(eR1, eR2)));
      DIP("%s.h %s,%s,%s\n", opcs[opc], nameIReg(rd), nameFReg(rs1), nameFReg(rs2));
      return True;
   }

   if ((GET_FUNCT3() == 0b001 && GET_RS2() == 0)
       && GET_FUNCT5() == RV64_SOPC_FCLASS && GET_OPCODE() == OPC_OP_FP
       && INSN(26, 25) == RV64_FMT_FH) {
      UInt  rd      = GET_RD();
      UInt  rs1     = GET_RS1();

      putIReg64(irsb, rd,
                mkIRExprCCall(
                   Ity_I64, 0 /*regparms*/, "riscv64g_calculate_fclass_h",
                   riscv64g_calculate_fclass_h, mkIRExprVec_1(getFReg16(rs1))));
      DIP("fclass.h %s,%s\n", nameIReg(rd), nameFReg(rs1));
      return True;
   }

#define RV64_SOPC_FCVT_IF 0b1100010

   if ((GET_RS2() == 0 || GET_RS2() == 1 || GET_RS2() == 2 || GET_RS2() == 3)
       && GET_FUNCT7() == RV64_SOPC_FCVT_IF && GET_OPCODE() == OPC_OP_FP) {
      UInt  rd      = GET_RD();
      UInt  rs1     = GET_RS1();
      UInt  opc     = GET_RS2();
      UInt  rm      = GET_FUNCT3();

      IRTemp rmEncd, rmIR;
      mk_get_rounding_mode(irsb, &rmEncd, &rmIR, rm);
      IRExpr* eR1 = getFReg16(rs1);
      IRExpr* eRM = mkexpr(rmIR);

      void* helpers[4]
         = { &riscv64g_calculate_fflags_fcvt_w_h, &riscv64g_calculate_fflags_fcvt_wu_h,
             &riscv64g_calculate_fflags_fcvt_l_h, &riscv64g_calculate_fflags_fcvt_lu_h };
      const HChar* hNames[4]
         = { "riscv64g_calculate_fflags_fcvt_w_h", "riscv64g_calculate_fflags_fcvt_wu_h",
             "riscv64g_calculate_fflags_fcvt_l_h", "riscv64g_calculate_fflags_fcvt_lu_h" };
      const HChar* opcs[4] = { "fcvt.w", "fcvt.wu", "fcvt.l", "fcvt.lu" };

      switch (opc) {
         case 0:
         case 1:
            putIReg32(irsb, rd,
                      binop(opc == 0 ? Iop_F16toI32S : Iop_F16toI32U, eRM, eR1));
            break;
         case 2:
         case 3:
            putIReg64(irsb, rd,
                      binop(opc == 2 ? Iop_F16toI64S : Iop_F16toI64U, eRM, eR1));
            break;
         default:
            vassert(0);
      }
      accumulateFFLAGS(irsb, mkIRExprCCall(Ity_I32, 0 /*regparms*/, hNames[opc],
                                           helpers[opc],
                                           mkIRExprVec_2(eR1, mkexpr(rmEncd))));
      DIP("%s.h %s,%s%s\n", opcs[opc], nameIReg(rd), nameFReg(rs1), nameRMOperand(rm));
      return True;
   }

#define RV64_SOPC_FCVT_FI 0b1101010

   if ((GET_RS2() == 0 || GET_RS2() == 1 || GET_RS2() == 2 || GET_RS2() == 3)
       && GET_FUNCT7() == RV64_SOPC_FCVT_FI && GET_OPCODE() == OPC_OP_FP) {
      UInt  rd      = GET_RD();
      UInt  rs1     = GET_RS1();
      UInt  opc     = GET_RS2();
      UInt  rm      = GET_FUNCT3();

      IRTemp rmEncd, rmIR;
      mk_get_rounding_mode(irsb, &rmEncd, &rmIR, rm);
      IRExpr* eR1 = (opc == 0 || opc == 1) ? getIReg32(rs1) : getIReg64(rs1);

      void* helpers[4]
         = { &riscv64g_calculate_fflags_fcvt_h_w, &riscv64g_calculate_fflags_fcvt_h_wu,
             &riscv64g_calculate_fflags_fcvt_h_l, &riscv64g_calculate_fflags_fcvt_h_lu };
      const HChar* hNames[4]
         = { "riscv64g_calculate_fflags_fcvt_h_w", "riscv64g_calculate_fflags_fcvt_h_wu",
             "riscv64g_calculate_fflags_fcvt_h_l", "riscv64g_calculate_fflags_fcvt_h_lu" };
      const HChar* opcs[4] = { "fcvt.h.w", "fcvt.h.wu", "fcvt.h.l", "fcvt.h.lu" };

      putFReg16(irsb, rd,
                binop(opc == 0   ? Iop_I32StoF16
                      : opc == 1 ? Iop_I32UtoF16
                      : opc == 2 ? Iop_I64StoF16
                                 : Iop_I64UtoF16,
                      mkexpr(rmIR), eR1));
      accumulateFFLAGS(irsb, mkIRExprCCall(Ity_I32, 0 /*regparms*/, hNames[opc],
                                           helpers[opc],
                                           mkIRExprVec_2(eR1, mkexpr(rmEncd))));
      DIP("%s %s,%s%s\n", opcs[opc], nameFReg(rd), nameIReg(rs1), nameRMOperand(rm));
      return True;
   }

   if (GET_OPCODE() == OPC_OP_FP && GET_FUNCT3() == 0 && GET_RS2() == 0 &&
       (GET_FUNCT5() == RV64_SOPC_FMV_X_H || GET_FUNCT5() == RV64_SOPC_FMV_H_X)
       && INSN(26, 25) == RV64_FMT_FH) {
      UInt  rd      = GET_RD();
      UInt  rs1     = GET_RS1();

      if (GET_FUNCT5() == RV64_SOPC_FMV_X_H) {
         putIReg16(irsb, rd, unop(Iop_ReinterpF16asI16, getFReg16(rs1)));
         DIP("fmv.x.h %s,%s\n", nameIReg(rd), nameFReg(rs1));
      } else {
         putFReg16(irsb, rd, unop(Iop_ReinterpI16asF16, getIReg16(rs1)));
         DIP("fmv.h.x %s,%s\n", nameFReg(rd), nameIReg(rs1));
      }
      return True;
   }

#endif
   return ok;
}

/*--------------------------------------------------------------------*/
/*--- end                                  guest_riscv64Zfh_toIR.c ---*/
/*--------------------------------------------------------------------*/
