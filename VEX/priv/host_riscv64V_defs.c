
/*--------------------------------------------------------------------*/
/*--- begin                              host_riscv64V_defs.c --------*/
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

#include "host_riscv64V0p7_defs.c"

RISCV64Instr* RISCV64Instr_VLdStWholeReg(UChar m, Bool isLoad, HReg sd, HReg base) {
   RISCV64Instr* i            = LibVEX_Alloc_inline(sizeof(RISCV64Instr));
   i->tag                     = RISCV64in_VLdStWholeReg;
   i->RISCV64in.VLdStWholeReg.isLoad = isLoad;
   i->RISCV64in.VLdStWholeReg.sd     = sd;
   i->RISCV64in.VLdStWholeReg.base   = base;
   i->RISCV64in.VLdStWholeReg.m      = m;
   return i;
}

RISCV64Instr* RISCV64Instr_VMV(UChar m, HReg dst, HReg src) {
   RISCV64Instr* i        = LibVEX_Alloc_inline(sizeof(RISCV64Instr));
   i->tag                 = RISCV64in_VMV;
   i->RISCV64in.VMV.dst   = dst;
   i->RISCV64in.VMV.src   = src;
   i->RISCV64in.VMV.m     = m;
   return i;
}

UChar* emit_RISCV64VInstr(/*MB_MOD*/ Bool*    is_profInc,
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
   UChar* ret = NULL;

   ret = emit_RISCV64V0p7Instr(is_profInc, buf, nbuf, i, mode64, endness_host,
                               disp_cp_chain_me_to_slowEP,
                               disp_cp_chain_me_to_fastEP, disp_cp_xindir,
                               disp_cp_xassisted);
   if (ret != NULL)
      return ret;

   return NULL;
}

Bool getRegUsage_RISCV64VInstr(HRegUsage* u, const RISCV64Instr* i)
{
   if (getRegUsage_RISCV64V0p7Instr(u, i))
      return True;
   return False;
}

Bool mapRegs_RISCV64VInstr(HRegRemap* m, RISCV64Instr* i)
{
   if (mapRegs_RISCV64V0p7Instr(m, i))
      return True;
   return False;
}

Bool ppRISCV64VInstr(const RISCV64Instr* i)
{
   if (ppRISCV64V0p7Instr(i))
      return True;
   return False;
}

/*--------------------------------------------------------------------*/
/*--- end                                host_riscv64V_defs.c --------*/
/*--------------------------------------------------------------------*/
