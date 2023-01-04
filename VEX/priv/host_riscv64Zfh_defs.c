
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
      default:
         return NULL;
   }
   return p;
}

/*--------------------------------------------------------------------*/
/*--- end                                host_riscv64Zfh_defs.c ------*/
/*--------------------------------------------------------------------*/
