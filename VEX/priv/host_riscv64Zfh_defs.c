
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
RISCV64Instr_FTriH(IROp op, HReg dst, HReg arg1, HReg arg2, HReg arg3)
{
   RISCV64Instr* i = LibVEX_Alloc_inline(sizeof(RISCV64Instr));
   i->tag                = RISCV64in_FTriH;
   i->RISCV64in.FTriH.op   = op;
   i->RISCV64in.FTriH.dst  = dst;
   i->RISCV64in.FTriH.arg1 = arg1;
   i->RISCV64in.FTriH.arg2 = arg2;
   i->RISCV64in.FTriH.arg3 = arg3;
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
         UInt dst = fregEnc(i->RISCV64in.FTriH.dst);
         UInt rs1 = fregEnc(i->RISCV64in.FTriH.arg1);
         UInt rs2 = fregEnc(i->RISCV64in.FTriH.arg2);
         UInt rs3 = fregEnc(i->RISCV64in.FTriH.arg3);
         p = emit_R(p, OPC_MADD, dst, 0b111, rs1, rs2, rs3 << 2 | 0b10);
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
         addHRegUse(u, HRmWrite, i->RISCV64in.FTriH.dst);
         addHRegUse(u, HRmRead,  i->RISCV64in.FTriH.arg1);
         addHRegUse(u, HRmRead,  i->RISCV64in.FTriH.arg2);
         addHRegUse(u, HRmRead,  i->RISCV64in.FTriH.arg3);
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
         mapReg(m, &i->RISCV64in.FTriH.dst);
         mapReg(m, &i->RISCV64in.FTriH.arg1);
         mapReg(m, &i->RISCV64in.FTriH.arg2);
         mapReg(m, &i->RISCV64in.FTriH.arg3);
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
         HChar* opc = i->RISCV64in.FLdStH.isLoad ? "flh" : "fsh";
         vex_printf("%s    ", opc);
         ppHRegRISCV64(i->RISCV64in.FLdStH.sd);
         vex_printf(", %d(", i->RISCV64in.FLdStH.imm12);
         ppHRegRISCV64(i->RISCV64in.FLdStH.base);
         vex_printf(")");
         return True;
      }
      case RISCV64in_FTriH:
         vex_printf("fmadd.h    ");
         ppHRegRISCV64(i->RISCV64in.FTriH.dst);
         vex_printf(", ");
         ppHRegRISCV64(i->RISCV64in.FTriH.arg1);
         vex_printf(", ");
         ppHRegRISCV64(i->RISCV64in.FTriH.arg2);
         vex_printf(", ");
         ppHRegRISCV64(i->RISCV64in.FTriH.arg3);
         return True;
      default:
         break;
   }
   return False;
}

/*--------------------------------------------------------------------*/
/*--- end                                host_riscv64Zfh_defs.c ------*/
/*--------------------------------------------------------------------*/
