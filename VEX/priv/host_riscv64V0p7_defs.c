
/*--------------------------------------------------------------------*/
/*--- begin                              host_riscv64V0p7_defs.c -----*/
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

static UChar* emit_RISCV64V0p7Instr(/*MB_MOD*/ Bool*    is_profInc,
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
      case RISCV64in_VLdStWholeReg: {
         Bool isLoad = i->RISCV64in.VLdStWholeReg.isLoad;
         UInt sd     = vregEnc(i->RISCV64in.VLdStWholeReg.sd);
         UInt base   = iregEnc(i->RISCV64in.VLdStWholeReg.base);
         UChar m     = i->RISCV64in.VLdStWholeReg.m;
         /*
          csrr t1, vl
          csrr t2, vtype
          vsetvli x0, x0, e8, m1
          vle/vse
          vsetvl  x0, t1, t2
         */
         p = emit_I(p, OPC_SYSTEM, 6, RV64_SOPC_CSRRS, 0, 0xc20);
         p = emit_I(p, OPC_SYSTEM, 7, RV64_SOPC_CSRRS, 0, 0xc21);
         p = emit_I(p, OPC_OP_V,   0, RV64_SOPC_OPCFG, 0, 0);
         UChar OPC = isLoad ? OPC_LOAD_FP : OPC_STORE_FP;
         p = emit_I(p, OPC,      sd, 0b111, base, 0b000000100000);
         p = emit_R(p, OPC_OP_V, 0,  RV64_SOPC_OPCFG, 6, 7, 0b1000000);
         break;
      }
      case RISCV64in_VMV: {
         UInt dst    = vregEnc(i->RISCV64in.VMV.dst);
         UInt src    = vregEnc(i->RISCV64in.VMV.src);
         UChar m     = i->RISCV64in.VMV.m;
         /*
          csrr t1, vl
          csrr t2, vtype
          vsetvli x0, x0, e8, m1
          vmv
          vsetvl  x0, t1, t2
         */
         p = emit_I(p, OPC_SYSTEM, 6, RV64_SOPC_CSRRS, 0, 0xc20);
         p = emit_I(p, OPC_SYSTEM, 7, RV64_SOPC_CSRRS, 0, 0xc21);
         p = emit_I(p, OPC_OP_V, 0,   RV64_SOPC_OPCFG, 0, 0);
         p = emit_R(p, OPC_OP_V, dst, RV64_SOPC_OPIVV, src, 0, 0b0101111);
         p = emit_R(p, OPC_OP_V, 0,   RV64_SOPC_OPCFG, 6,   7, 0b1000000);
         break;
      }
      default:
         return NULL;
   }
   return p;
}

static Bool getRegUsage_RISCV64V0p7Instr(HRegUsage* u, const RISCV64Instr* i)
{
   return False;
}

static Bool mapRegs_RISCV64V0p7Instr(HRegRemap* m, RISCV64Instr* i)
{
   return False;
}

static Bool ppRISCV64V0p7Instr(const RISCV64Instr* i)
{
   return False;
}

/*--------------------------------------------------------------------*/
/*--- end                                host_riscv64V0p7_defs.c -----*/
/*--------------------------------------------------------------------*/
