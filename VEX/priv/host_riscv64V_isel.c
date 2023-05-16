
/*--------------------------------------------------------------------*/
/*--- begin                              host_riscv64V_isel.c --------*/
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

#include "host_riscv64V0p7_isel.c"

static HReg iselVExpr_wrk(ISelEnv* env, IRExpr* e)
{
   IRType ty = typeOfIRExpr(env->type_env, e);
   vassert(e);
   vassert(ty == Ity_V128);

   Bool ok;
   HReg ret = iselV0p7Expr_wrk(env, e, &ok);
   if (ok)
      return ret;

   ppIRExpr(e);
   vpanic("iselVExpr(riscv64)");
}

static HReg iselVExpr(ISelEnv* env, IRExpr* e)
{
   HReg r = iselVExpr_wrk( env, e );
   vassert(hregClass(r) == HRcVec);
   vassert(hregIsVirtual(r));
   return r;
}

/*--------------------------------------------------------------------*/
/*--- end                                host_riscv64V_isel.c --------*/
/*--------------------------------------------------------------------*/
