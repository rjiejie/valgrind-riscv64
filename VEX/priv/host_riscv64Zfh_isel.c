
/*--------------------------------------------------------------------*/
/*--- begin                              host_riscv64Zfh_isel.c ------*/
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

static HReg iselFlt16Expr_wrk(ISelEnv* env, IRExpr* e, Bool* ok)
{
   vassert(ok != NULL);
   *ok = True;
   HReg ret = {0};
   IRType ty = typeOfIRExpr(env->type_env, e);

   if (e->tag == Iex_Load && e->Iex.Load.end == Iend_LE &&
       ty == Ity_F16) {
      HReg addr = iselIntExpr_R(env, e->Iex.Load.addr);
      HReg res  = newVRegF(env);
      addInstr(env, RISCV64Instr_FLdStH(True, res, addr, 0));
      return res;
   }

   if (e->tag == Iex_Get && ty == Ity_F16) {
      HReg res  = newVRegF(env);
      HReg addr = get_baseblock_register();
      Int offs = e->Iex.Get.offset - BASEBLOCK_OFFSET_ADJUSTMENT;
      vassert(offs >= -2048 && offs < 2048);
      addInstr(env, RISCV64Instr_FLdStH(True, res, addr, offs));
      return res;
   }

   if (e->tag == Iex_Qop) {
      IRQop* qop  = e->Iex.Qop.details;
      if (qop->op == Iop_MAddF16) {
         HReg rd  = newVRegF(env);
         HReg rs1 = iselFltExpr(env, qop->arg2);
         HReg rs2 = iselFltExpr(env, qop->arg3);
         HReg rs3 = iselFltExpr(env, qop->arg4);
         set_fcsr_rounding_mode(env, qop->arg1);
         addInstr(env, RISCV64Instr_FTriH(qop->op, rd, rs1, rs2, rs3));
         return rd;
      }
   }

   if (e->tag == Iex_Triop) {
      IRTriop* triop  = e->Iex.Triop.details;
      switch (triop->op) {
         case Iop_AddF16:
         case Iop_MulF16:
         case Iop_DivF16: {
            HReg rd  = newVRegF(env);
            HReg rs1 = iselFltExpr(env, triop->arg2);
            HReg rs2 = iselFltExpr(env, triop->arg3);
            set_fcsr_rounding_mode(env, triop->arg1);
            addInstr(env, RISCV64Instr_FBinH(triop->op, rd, rs1, rs2));
            return rd;
         }
         default:
            break;
      }
   }

   if (e->tag == Iex_Binop) {
      switch (e->Iex.Binop.op) {
         case Iop_SqrtF16: {
            HReg rd  = newVRegF(env);
            HReg rs1 = iselFltExpr(env, e->Iex.Binop.arg2);
            set_fcsr_rounding_mode(env, e->Iex.Binop.arg1);
            addInstr(env, RISCV64Instr_FUnaryH(e->Iex.Binop.op, rd, rs1));
            return rd;
         }
         case Iop_MinNumF16:
         case Iop_MaxNumF16: {
            HReg rd  = newVRegF(env);
            HReg rs1 = iselFltExpr(env, e->Iex.Binop.arg1);
            HReg rs2 = iselFltExpr(env, e->Iex.Binop.arg2);
            addInstr(env, RISCV64Instr_FBinH(e->Iex.Binop.op, rd, rs1, rs2));
            return rd;
         }
         case Iop_F32toF16:
         case Iop_F64toF16: {
            HReg rd = newVRegF(env);
            HReg rs1 = iselFltExpr(env, e->Iex.Binop.arg2);
            set_fcsr_rounding_mode(env, e->Iex.Binop.arg1);
            addInstr(env, RISCV64Instr_FCvtH(e->Iex.Binop.op, rd, rs1));
            return rd;
         }
         case Iop_I32StoF16:
         case Iop_I32UtoF16:
         case Iop_I64StoF16:
         case Iop_I64UtoF16: {
            HReg rd = newVRegF(env);
            HReg rs1 = iselIntExpr_R(env, e->Iex.Binop.arg2);
            set_fcsr_rounding_mode(env, e->Iex.Binop.arg1);
            addInstr(env, RISCV64Instr_FCvtH(e->Iex.Binop.op, rd, rs1));
            return rd;
         }
         default:
            break;
      }
   }

   if (e->tag == Iex_Unop) {
      switch (e->Iex.Unop.op) {
         case Iop_AbsF16:
         case Iop_NegF16: {
            HReg rd  = newVRegF(env);
            HReg rs1 = iselFltExpr(env, e->Iex.Unop.arg);
            addInstr(env, RISCV64Instr_FUnaryH(e->Iex.Unop.op, rd, rs1));
            return rd;
         }
         case Iop_ReinterpI16asF16: {
            HReg rd  = newVRegF(env);
            HReg rs1 = iselIntExpr_R(env, e->Iex.Unop.arg);
            addInstr(env, RISCV64Instr_FUnaryH(e->Iex.Unop.op, rd, rs1));
            return rd;
         }
         case Iop_F16toF32:
         case Iop_F16toF64: {
            HReg rd = newVRegF(env);
            HReg rs1 = iselFltExpr(env, e->Iex.Unop.arg);
            addInstr(env, RISCV64Instr_FCvtH(e->Iex.Unop.op, rd, rs1));
            return rd;
         }
         default:
            break;
      }
   }

   *ok = False;
   return ret;
}

static HReg iselIntZfhExpr_R_wrk(ISelEnv* env, IRExpr* e, Bool* ok) {
   vassert(ok != NULL);
   *ok = True;
   HReg ret = {0};

   if (e->tag == Iex_Unop) {
      switch (e->Iex.Unop.op) {
         case Iop_ReinterpF16asI16: {
            HReg rd  = newVRegI(env);
            HReg rs1 = iselFltExpr(env, e->Iex.Unop.arg);
            addInstr(env, RISCV64Instr_FUnaryH(e->Iex.Unop.op, rd, rs1));
            return rd;
         }
         default:
            break;
      }
   }

   if (e->tag == Iex_Binop) {
      switch (e->Iex.Binop.op) {
      case Iop_F16toI32S:
      case Iop_F16toI32U:
      case Iop_F16toI64S:
      case Iop_F16toI64U: {
         HReg rd = newVRegI(env);
         HReg rs1 = iselFltExpr(env, e->Iex.Binop.arg2);
         set_fcsr_rounding_mode(env, e->Iex.Binop.arg1);
         addInstr(env, RISCV64Instr_FCvtH(e->Iex.Binop.op, rd, rs1));
         return rd;
      }
      case Iop_CmpF16: {
         HReg argL = iselFltExpr(env, e->Iex.Binop.arg1);
         HReg argR = iselFltExpr(env, e->Iex.Binop.arg2);

         HReg eq = newVRegI(env);
         HReg lt = newVRegI(env);
         HReg gt = newVRegI(env);

         addInstr(env, RISCV64Instr_FCmpH(RV64_CMP_EQ, eq, argL, argR));
         addInstr(env, RISCV64Instr_FCmpH(RV64_CMP_LT, gt, argR, argL));
         addInstr(env, RISCV64Instr_FCmpH(RV64_CMP_LT, lt, argL, argR));
         /*
            t0 = Ircr_UN
            t1 = Ircr_LT
            t2 = csel t1, t0, lt
            t3 = Ircr_GT
            t4 = csel t3, t2, gt
            t5 = Ircr_EQ
            dst = csel t5, t4, eq
         */
         HReg t0 = newVRegI(env);
         addInstr(env, RISCV64Instr_LI(t0, Ircr_UN));
         HReg t1 = newVRegI(env);
         addInstr(env, RISCV64Instr_LI(t1, Ircr_LT));
         HReg t2 = newVRegI(env);
         addInstr(env, RISCV64Instr_CSEL(t2, t1, t0, lt));
         HReg t3 = newVRegI(env);
         addInstr(env, RISCV64Instr_LI(t3, Ircr_GT));
         HReg t4 = newVRegI(env);
         addInstr(env, RISCV64Instr_CSEL(t4, t3, t2, gt));
         HReg t5 = newVRegI(env);
         addInstr(env, RISCV64Instr_LI(t5, Ircr_EQ));
         HReg rd = newVRegI(env);
         addInstr(env, RISCV64Instr_CSEL(rd, t5, t4, eq));
         return rd;
      }
      default:
         break;
      }
   }
   *ok = False;
   return ret;
}

/*--------------------------------------------------------------------*/
/*--- end                                host_riscv64Zfh_isel.c ------*/
/*--------------------------------------------------------------------*/
