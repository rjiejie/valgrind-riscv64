
/*--------------------------------------------------------------------*/
/*--- begin                              host_riscv64xthead_isel.c ---*/
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

static HReg iselXTHEADExpr_wrk(ISelEnv* env, IRExpr* e, Bool* ok)
{
   vassert(ok != NULL);
   *ok = True;
   HReg ret = {0};
   IRType ty = typeOfIRExpr(env->type_env, e);

   *ok = False;
   return ret;
}

/*--------------------------------------------------------------------*/
/*--- end                                host_riscv64xthead_isel.c ---*/
/*--------------------------------------------------------------------*/
