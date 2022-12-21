
/*--------------------------------------------------------------------*/
/*--- begin                              host_riscv64xthead_defs.h ---*/
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

#ifndef __VEX_HOST_RISCV64XTHEAD_DEFS_H
#define __VEX_HOST_RISCV64XTHEAD_DEFS_H

/*--------------------------------------------------------------------*/
/*--- Instructions                                            --------*/
/*--------------------------------------------------------------------*/

typedef enum {
   XTHEAD64in_XX,
} XTHEAD64InstrTag;

typedef struct {
   XTHEAD64InstrTag;
} XTHEAD64Instr;

Int emit_XTHEAD64Instr(/*MB_MOD*/ Bool*    is_profInc,
                       UChar*              buf,
                       Int                 nbuf,
                       const void*         i,
                       Bool                mode64,
                       VexEndness          endness_host,
                       const void*         disp_cp_chain_me_to_slowEP,
                       const void*         disp_cp_chain_me_to_fastEP,
                       const void*         disp_cp_xindir,
                       const void*         disp_cp_xassisted);

#endif /* ndef __VEX_HOST_RISCV64XTHEAD_DEFS_H */

/*--------------------------------------------------------------------*/
/*--- end                                host_riscv64xthead_defs.h ---*/
/*--------------------------------------------------------------------*/
