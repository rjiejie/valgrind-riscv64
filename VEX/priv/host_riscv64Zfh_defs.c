
/*--------------------------------------------------------------------*/
/*--- begin                              host_riscv64Zfh_defs.c ------*/
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

RISCV64Instr* RISCV64Instr_FLdStH(Bool isLoad, HReg sd, HReg base, Int imm12)
{
   RISCV64Instr* i            = LibVEX_Alloc_inline(sizeof(RISCV64Instr));
   i->tag                     = RISCV64in_FLdStH;
   i->RISCV64in.FLdStH.isLoad = isLoad;
   i->RISCV64in.FLdStH.sd     = sd;
   i->RISCV64in.FLdStH.base   = base;
   i->RISCV64in.FLdStH.imm12  = imm12;
   return i;
}

RISCV64Instr*
RISCV64Instr_FTriH(IROp op, HReg rd, HReg rs1, HReg rs2, HReg rs3)
{
   RISCV64Instr* i = LibVEX_Alloc_inline(sizeof(RISCV64Instr));
   i->tag                 = RISCV64in_FTriH;
   i->RISCV64in.FTriH.op  = op;
   i->RISCV64in.FTriH.rd  = rd;
   i->RISCV64in.FTriH.rs1 = rs1;
   i->RISCV64in.FTriH.rs2 = rs2;
   i->RISCV64in.FTriH.rs3 = rs3;
   return i;
}

RISCV64Instr* RISCV64Instr_FBinH(IROp op, HReg rd, HReg rs1, HReg rs2)
{
   RISCV64Instr* i         = LibVEX_Alloc_inline(sizeof(RISCV64Instr));
   i->tag                  = RISCV64in_FBinH;
   i->RISCV64in.FBinH.op   = op;
   i->RISCV64in.FBinH.rd   = rd;
   i->RISCV64in.FBinH.rs1  = rs1;
   i->RISCV64in.FBinH.rs2  = rs2;
   return i;
}

RISCV64Instr* RISCV64Instr_FUnaryH(IROp op, HReg rd, HReg rs1)
{
   RISCV64Instr* i          = LibVEX_Alloc_inline(sizeof(RISCV64Instr));
   i->tag                   = RISCV64in_FUnaryH;
   i->RISCV64in.FUnaryH.op  = op;
   i->RISCV64in.FUnaryH.rd  = rd;
   i->RISCV64in.FUnaryH.rs1 = rs1;
   return i;
}

RISCV64Instr* RISCV64Instr_FCvtH(IROp op, HReg rd, HReg rs1)
{
   RISCV64Instr* i        = LibVEX_Alloc_inline(sizeof(RISCV64Instr));
   i->tag                 = RISCV64in_FCvtH;
   i->RISCV64in.FCvtH.op  = op;
   i->RISCV64in.FCvtH.rd  = rd;
   i->RISCV64in.FCvtH.rs1 = rs1;
   return i;
}

RISCV64Instr* RISCV64Instr_FCmpH(RISCV64CmpOp op, HReg rd, HReg rs1, HReg rs2)
{
   RISCV64Instr* i        = LibVEX_Alloc_inline(sizeof(RISCV64Instr));
   i->tag                 = RISCV64in_FCmpH;
   i->RISCV64in.FCmpH.op  = op;
   i->RISCV64in.FCmpH.rd  = rd;
   i->RISCV64in.FCmpH.rs1 = rs1;
   i->RISCV64in.FCmpH.rs1 = rs2;
   return i;
}

UChar* emit_RISCV64ZfhInstr(/*MB_MOD*/ Bool*    is_profInc,
                            UChar*              buf,
                            Int                 nbuf,
                            const RISCV64Instr* i,
                            Bool                mode64,
                            VexEndness          endness_host,
                            const void*         disp_cp_chain_me_to_slowEP,
                            const void*         disp_cp_chain_me_to_fastEP,
                            const void*         disp_cp_xindir,
                            const void*         disp_cp_xassisted)
{
   UChar* p = buf;
   switch (i->tag) {
      case RISCV64in_FLdStH: {
         Bool isLoad = i->RISCV64in.FLdStH.isLoad;
         UInt sd     = fregEnc(i->RISCV64in.FLdStH.sd);
         UInt base   = iregEnc(i->RISCV64in.FLdStH.base);
         Int  imm12  = i->RISCV64in.FLdStH.imm12;
         UInt uimm12 = imm12 & 0xFFF;
         if (isLoad)
            p = emit_I(p, OPC_LOAD_FP, sd, 0b001, base, uimm12);
         else
            p = emit_S(p, OPC_STORE_FP, uimm12, 0b001, base, sd);
         break;
      }
      case RISCV64in_FTriH: {
         UInt rd  = fregEnc(i->RISCV64in.FTriH.rd);
         UInt rs1 = fregEnc(i->RISCV64in.FTriH.rs1);
         UInt rs2 = fregEnc(i->RISCV64in.FTriH.rs2);
         UInt rs3 = fregEnc(i->RISCV64in.FTriH.rs3);
         p = emit_R(p, OPC_MADD, rd, 0b111, rs1, rs2, rs3 << 2 | RV64_FMT_FH);
         break;
      }
      case RISCV64in_FBinH: {
         UInt rd  = fregEnc(i->RISCV64in.FBinH.rd);
         UInt rs1 = fregEnc(i->RISCV64in.FBinH.rs1);
         UInt rs2 = fregEnc(i->RISCV64in.FBinH.rs2);
         UInt opc = 0;
         switch (i->RISCV64in.FBinH.op) {
            case Iop_AddF16: opc = RV64_SOPC_FADD; break;
            case Iop_MulF16: opc = RV64_SOPC_FMUL; break;
            case Iop_DivF16: opc = RV64_SOPC_FDIV; break;
            case Iop_MinNumF16:
            case Iop_MaxNumF16: {
               UInt op = i->RISCV64in.FBinH.op == Iop_MinNumF16 ? 0 : 1;
               opc = RV64_SOPC_FMIN_MAX;
               return emit_R(p, OPC_OP_FP, rd, op, rs1, rs2, opc << 2 | RV64_FMT_FH);
            }
            default: return NULL;
         }
         p = emit_R(p, OPC_OP_FP, rd, 0b111, rs1, rs2, opc << 2 | RV64_FMT_FH);
         break;
      }
      case RISCV64in_FUnaryH: {
         UInt rd = 0;
         UInt rs1 = 0;
         UInt opc = 0;
         switch (i->RISCV64in.FUnaryH.op) {
            case Iop_SqrtF16:
               rd  = fregEnc(i->RISCV64in.FUnaryH.rd);
               rs1 = fregEnc(i->RISCV64in.FUnaryH.rs1);
               opc = RV64_SOPC_FSQRT;
               p = emit_R(p, OPC_OP_FP, rd, 0b111, rs1, 0, opc << 2 | RV64_FMT_FH);
               break;
            case Iop_AbsF16:
            case Iop_NegF16: {
               rd  = fregEnc(i->RISCV64in.FUnaryH.rd);
               rs1 = fregEnc(i->RISCV64in.FUnaryH.rs1);
               opc = RV64_SOPC_FSGNJ;
               UInt op = i->RISCV64in.FUnaryH.op == Iop_NegF16 ? 0b001 : 0b010;
               p = emit_R(p, OPC_OP_FP, rd, op, rs1, rs1, opc << 2 | RV64_FMT_FH);
               break;
            }
            case Iop_ReinterpF16asI16:
            case Iop_ReinterpI16asF16:
               if (i->RISCV64in.FUnaryH.op == Iop_ReinterpF16asI16) {
                  rd  = iregEnc(i->RISCV64in.FUnaryH.rd);
                  rs1 = fregEnc(i->RISCV64in.FUnaryH.rs1);
                  opc = RV64_SOPC_FMV_X_H;
               } else {
                  rd  = fregEnc(i->RISCV64in.FUnaryH.rd);
                  rs1 = iregEnc(i->RISCV64in.FUnaryH.rs1);
                  opc = RV64_SOPC_FMV_H_X;
               }
               p = emit_R(p, OPC_OP_FP, rd, 0, rs1, 0, opc << 2 | RV64_FMT_FH);
               break;
            default:
               return NULL;
         }
         break;
      }
      case RISCV64in_FCvtH: {
            UInt opc = 0;
            UInt rs1 = 0;
            UInt rd = 0;
            UInt rs2 = 0;
            UInt fmt = 0;
            switch (i->RISCV64in.FCvtH.op) {
            case Iop_F16toF32:
               rs2 = RV64_FMT_FH;
               fmt = RV64_FMT_FS;
               opc = RV64_SOPC_FCVT_FF;
               rs1 = fregEnc(i->RISCV64in.FCvtH.rs1);
               rd = fregEnc(i->RISCV64in.FCvtH.rd);
               break;
            case Iop_F16toF64:
               rs2 = RV64_FMT_FH;
               fmt = RV64_FMT_FD;
               opc = RV64_SOPC_FCVT_FF;
               rs1 = fregEnc(i->RISCV64in.FCvtH.rs1);
               rd = fregEnc(i->RISCV64in.FCvtH.rd);
               break;
            case Iop_F32toF16:
               rs2 = RV64_FMT_FS;
               fmt = RV64_FMT_FH;
               opc = RV64_SOPC_FCVT_FF;
               rs1 = fregEnc(i->RISCV64in.FCvtH.rs1);
               rd = fregEnc(i->RISCV64in.FCvtH.rd);
               break;
            case Iop_F64toF16:
               rs2 = RV64_FMT_FD;
               fmt = RV64_FMT_FH;
               opc = RV64_SOPC_FCVT_FF;
               rs1 = fregEnc(i->RISCV64in.FCvtH.rs1);
               rd = fregEnc(i->RISCV64in.FCvtH.rd);
               break;
            case Iop_I32StoF16:
               rs2 = RV64_FMT_W;
               fmt = RV64_FMT_FH;
               opc = RV64_SOPC_FCVT_FI;
               rs1 = iregEnc(i->RISCV64in.FCvtH.rs1);
               rd = fregEnc(i->RISCV64in.FCvtH.rd);
               break;
            case Iop_I32UtoF16:
               rs2 = RV64_FMT_WU;
               fmt = RV64_FMT_FH;
               opc = RV64_SOPC_FCVT_FI;
               rs1 = iregEnc(i->RISCV64in.FCvtH.rs1);
               rd = fregEnc(i->RISCV64in.FCvtH.rd);
               break;
            case Iop_I64StoF16:
               rs2 = RV64_FMT_L;
               fmt = RV64_FMT_FH;
               opc = RV64_SOPC_FCVT_FI;
               rs1 = iregEnc(i->RISCV64in.FCvtH.rs1);
               rd = fregEnc(i->RISCV64in.FCvtH.rd);
               break;
            case Iop_I64UtoF16:
               rs2 = RV64_FMT_LU;
               fmt = RV64_FMT_FH;
               opc = RV64_SOPC_FCVT_FI;
               rs1 = iregEnc(i->RISCV64in.FCvtH.rs1);
               rd = fregEnc(i->RISCV64in.FCvtH.rd);
               break;
            case Iop_F16toI64S:
               fmt = RV64_FMT_FH;
               rs2 = RV64_FMT_L;
               opc = RV64_SOPC_FCVT_IF;
               rs1 = fregEnc(i->RISCV64in.FCvtH.rs1);
               rd = iregEnc(i->RISCV64in.FCvtH.rd);
               break;
            case Iop_F16toI64U:
               fmt = RV64_FMT_FH;
               rs2 = RV64_FMT_LU;
               opc = RV64_SOPC_FCVT_IF;
               rs1 = fregEnc(i->RISCV64in.FCvtH.rs1);
               rd = iregEnc(i->RISCV64in.FCvtH.rd);
               break;
            case Iop_F16toI32S:
               fmt = RV64_FMT_FH;
               rs2 = RV64_FMT_W;
               opc = RV64_SOPC_FCVT_IF;
               rs1 = fregEnc(i->RISCV64in.FCvtH.rs1);
               rd = iregEnc(i->RISCV64in.FCvtH.rd);
               break;
            case Iop_F16toI32U:
               fmt = RV64_FMT_FH;
               rs2 = RV64_FMT_WU;
               opc = RV64_SOPC_FCVT_IF;
               rs1 = fregEnc(i->RISCV64in.FCvtH.rs1);
               rd = iregEnc(i->RISCV64in.FCvtH.rd);
               break;
            default:
               return NULL;
         }
         p = emit_R(p, OPC_OP_FP, rd, 0b111, rs1, rs2, opc << 2 | fmt);
         break;
      }
      case RISCV64in_FCmpH: {
         UInt rd  = iregEnc(i->RISCV64in.FCmpH.rd);
         UInt rs1 = fregEnc(i->RISCV64in.FCmpH.rs1);
         UInt rs2 = fregEnc(i->RISCV64in.FCmpH.rs2);
         UInt cmp_rm = 0;
         UInt opc = RV64_SOPC_FCMP;
         switch (i->RISCV64in.FCmpH.op) {
            case RV64_CMP_EQ:
            case RV64_CMP_LT:
               cmp_rm = i->RISCV64in.FCmpH.op;
            default:
               return NULL;
         }
         p = emit_R(p, OPC_OP_FP, rd, cmp_rm, rs1, rs2, opc << 2 | RV64_FMT_FH);
         break;
      }
      default:
         return NULL;
   }
   return p;
}

Bool getRegUsage_RISCV64ZfhInstr(HRegUsage* u, const RISCV64Instr* i)
{
   switch (i->tag) {
      case RISCV64in_FLdStH:
         addHRegUse(u, HRmRead, i->RISCV64in.FLdStH.base);
         if (i->RISCV64in.FLdStH.isLoad)
            addHRegUse(u, HRmWrite, i->RISCV64in.FLdStH.sd);
         else
            addHRegUse(u, HRmRead, i->RISCV64in.FLdStH.sd);
         return True;
      case RISCV64in_FTriH:
         addHRegUse(u, HRmWrite, i->RISCV64in.FTriH.rd);
         addHRegUse(u, HRmRead,  i->RISCV64in.FTriH.rs1);
         addHRegUse(u, HRmRead,  i->RISCV64in.FTriH.rs2);
         addHRegUse(u, HRmRead,  i->RISCV64in.FTriH.rs3);
         return True;
      case RISCV64in_FBinH:
         addHRegUse(u, HRmWrite, i->RISCV64in.FBinH.rd);
         addHRegUse(u, HRmRead, i->RISCV64in.FBinH.rs1);
         addHRegUse(u, HRmRead, i->RISCV64in.FBinH.rs2);
         return True;
      case RISCV64in_FUnaryH:
         addHRegUse(u, HRmWrite, i->RISCV64in.FUnaryH.rd);
         addHRegUse(u, HRmRead, i->RISCV64in.FUnaryH.rs1);
         return True;
      case RISCV64in_FCvtH:
         addHRegUse(u, HRmWrite, i->RISCV64in.FCvtH.rd);
         addHRegUse(u, HRmRead, i->RISCV64in.FCvtH.rs1);
         return True;
      case RISCV64in_FCmpH:
         addHRegUse(u, HRmWrite, i->RISCV64in.FCmpH.rd);
         addHRegUse(u, HRmRead, i->RISCV64in.FCmpH.rs1);
         addHRegUse(u, HRmRead, i->RISCV64in.FCmpH.rs2);
         return True;
      default:
         break;
   }
   return False;
}

Bool mapRegs_RISCV64ZfhInstr(HRegRemap* m, RISCV64Instr* i)
{
   switch (i->tag) {
      case RISCV64in_FLdStH:
         mapReg(m, &i->RISCV64in.FLdStH.base);
         mapReg(m, &i->RISCV64in.FLdStH.sd);
         return True;
      case RISCV64in_FTriH:
         mapReg(m, &i->RISCV64in.FTriH.rd);
         mapReg(m, &i->RISCV64in.FTriH.rs1);
         mapReg(m, &i->RISCV64in.FTriH.rs2);
         mapReg(m, &i->RISCV64in.FTriH.rs3);
         return True;
      case RISCV64in_FBinH:
         mapReg(m, &i->RISCV64in.FBinH.rd);
         mapReg(m, &i->RISCV64in.FBinH.rs1);
         mapReg(m, &i->RISCV64in.FBinH.rs2);
         return True;
      case RISCV64in_FUnaryH:
         mapReg(m, &i->RISCV64in.FUnaryH.rd);
         mapReg(m, &i->RISCV64in.FUnaryH.rs1);
         return True;
      case RISCV64in_FCvtH:
         mapReg(m, &i->RISCV64in.FCvtH.rd);
         mapReg(m, &i->RISCV64in.FCvtH.rs1);
         return True;
      case RISCV64in_FCmpH:
         mapReg(m, &i->RISCV64in.FCmpH.rd);
         mapReg(m, &i->RISCV64in.FCmpH.rs1);
         mapReg(m, &i->RISCV64in.FCmpH.rs2);
         return True;
      default:
         break;
   }
   return False;
}

Bool ppRISCV64ZfhInstr(const RISCV64Instr* i)
{
   switch (i->tag) {
      case RISCV64in_FLdStH: {
         HChar* opc = i->RISCV64in.FLdStH.isLoad ? "fl" : "fs";
         vex_printf("%s.h    ", opc);
         ppHRegRISCV64(i->RISCV64in.FLdStH.sd);
         vex_printf(", %d(", i->RISCV64in.FLdStH.imm12);
         ppHRegRISCV64(i->RISCV64in.FLdStH.base);
         vex_printf(")");
         return True;
      }
      case RISCV64in_FTriH:
         vex_printf("fmadd.h    ");
         ppHRegRISCV64(i->RISCV64in.FTriH.rd);
         vex_printf(", ");
         ppHRegRISCV64(i->RISCV64in.FTriH.rs1);
         vex_printf(", ");
         ppHRegRISCV64(i->RISCV64in.FTriH.rs2);
         vex_printf(", ");
         ppHRegRISCV64(i->RISCV64in.FTriH.rs3);
         return True;
      case RISCV64in_FBinH: {
         HChar* opc = "???";
         switch (i->RISCV64in.FBinH.op) {
            case Iop_AddF16:    opc = "fadd"; break;
            case Iop_MulF16:    opc = "fmul"; break;
            case Iop_DivF16:    opc = "fdiv"; break;
            case Iop_MinNumF16: opc = "fmin"; break;
            case Iop_MaxNumF16: opc = "fmax"; break;
            default: break;
         }
         vex_printf("%s.h    ", opc);
         ppHRegRISCV64(i->RISCV64in.FBinH.rd);
         vex_printf(", ");
         ppHRegRISCV64(i->RISCV64in.FBinH.rs1);
         vex_printf(", ");
         ppHRegRISCV64(i->RISCV64in.FBinH.rs2);
         return True;
      }
      case RISCV64in_FUnaryH: {
         HChar* opc = "???";
         switch (i->RISCV64in.FUnaryH.op) {
            case Iop_SqrtF16: opc = "fsqrt.h"; break;
            case Iop_AbsF16:  opc = "fabs.h"; break;
            case Iop_NegF16:  opc = "fneg.h"; break;
            case Iop_ReinterpF16asI16: opc = "fmv.x.h"; break;
            case Iop_ReinterpI16asF16: opc = "fmv.h.x"; break;
            default: break;
         }
         vex_printf("%s    ", opc);
         ppHRegRISCV64(i->RISCV64in.FUnaryH.rd);
         vex_printf(", ");
         ppHRegRISCV64(i->RISCV64in.FUnaryH.rs1);
         return True;
      }
      case RISCV64in_FCvtH: {
         HChar* opc = "???";
         switch (i->RISCV64in.FCvtH.op) {
            case Iop_F16toF32:  opc = "fcvt.s.h";  break;
            case Iop_F16toF64:  opc = "fcvt.d.h";  break;
            case Iop_F32toF16:  opc = "fcvt.h.s";  break;
            case Iop_F64toF16:  opc = "fcvt.h.d";  break;
            case Iop_F16toI32S: opc = "fcvt.w.h";  break;
            case Iop_F16toI32U: opc = "fcvt.wu.h"; break;
            case Iop_F16toI64S: opc = "fcvt.l.h";  break;
            case Iop_F16toI64U: opc = "fcvt.lu.h"; break;
            case Iop_I32StoF16: opc = "fcvt.h.w";  break;
            case Iop_I32UtoF16: opc = "fcvt.h.wu"; break;
            case Iop_I64StoF16: opc = "fcvt.h.l";  break;
            case Iop_I64UtoF16: opc = "fcvt.h.lu"; break;
            default: break;
         }
         vex_printf("%s    ", opc);
         ppHRegRISCV64(i->RISCV64in.FCvtH.rd);
         vex_printf(", ");
         ppHRegRISCV64(i->RISCV64in.FCvtH.rs1);
         return True;
      }
      case RISCV64in_FCmpH: {
         HChar* opc = "???";
         switch (i->RISCV64in.FCvtH.op) {
         case RV64_CMP_EQ: opc = "feq.h"; break;
         case RV64_CMP_LT: opc = "flt.h"; break;
         default: break;
         }
         vex_printf("%s    ", opc);
         ppHRegRISCV64(i->RISCV64in.FCmpH.rd);
         vex_printf(", ");
         ppHRegRISCV64(i->RISCV64in.FCmpH.rs1);
         vex_printf(", ");
         ppHRegRISCV64(i->RISCV64in.FCmpH.rs2);
         return True;
      }
      default:
         break;
   }
   return False;
}

/*--------------------------------------------------------------------*/
/*--- end                                host_riscv64Zfh_defs.c ------*/
/*--------------------------------------------------------------------*/
