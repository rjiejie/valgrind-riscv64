
/*--------------------------------------------------------------------*/
/*--- begin                              host_riscv64xthead_defs.c ---*/
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

Int emit_XTHEAD64Instr(/*MB_MOD*/ Bool*    is_profInc,
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
   return -1;
}

/*--------------------------------------------------------------------*/
/*--- end                                host_riscv64xthead_defs.c ---*/
/*--------------------------------------------------------------------*/
