
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

   *ok = False;
   return ret;
}

/*--------------------------------------------------------------------*/
/*--- end                                host_riscv64Zfh_isel.c ------*/
/*--------------------------------------------------------------------*/
