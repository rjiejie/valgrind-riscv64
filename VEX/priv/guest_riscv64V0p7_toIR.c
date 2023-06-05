
/*--------------------------------------------------------------------*/
/*--- begin                                guest_riscv64V0p7_toIR.c --*/
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

#include "guest_riscv64V0p7_helpers.c"

static Bool dis_RV64V0p7_cfg(/*MB_OUT*/ DisResult* dres,
                             /*OUT*/ IRSB*         irsb,
                             UInt                  insn)
{
   return False;
}

static Bool dis_RV64V0p7_ldst(/*MB_OUT*/ DisResult* dres,
                              /*OUT*/ IRSB*         irsb,
                              UInt                  insn)
{
   Bool isLD = GET_OPCODE() == OPC_LOAD_FP;
   return False;
}

static Bool dis_RV64V0p7_arith_OPI(/*MB_OUT*/ DisResult* dres,
                                   /*OUT*/ IRSB*         irsb,
                                   UInt                  insn)
{
   IRDirty *d  = NULL;
   void *fAddr = NULL;
   const HChar *fName = NULL;
   IRExpr **args = NULL;
   UInt temp = 0;
   IRTemp ret = newTemp(irsb, Ity_I32);

   UInt rd   = GET_RD();
   UInt rs1  = GET_RS1();
   UInt rs2  = GET_RS2();
   Bool mask = GET_VMASK();

   // vadd
   if (GET_FUNCT6() == 0b000000) {
      GETC_VBinopOPI(vadd);
      return True;
   }

   return False;
}

static Bool dis_RV64V0p7_arith_OPM(/*MB_OUT*/ DisResult* dres,
                                   /*OUT*/ IRSB*         irsb,
                                   UInt                  insn)
{
   return False;
}

static Bool dis_RV64V0p7_arith_OPF(/*MB_OUT*/ DisResult* dres,
                                   /*OUT*/ IRSB*         irsb,
                                   UInt                  insn)
{
   IRDirty *d  = NULL;
   void *fAddr = NULL;
   const HChar *fName = NULL;
   IRExpr **args = NULL;
   UInt temp = 0;
   IRTemp ret = newTemp(irsb, Ity_I32);
   IRTemp frm = newTemp(irsb, Ity_I32);

   UInt rd   = GET_RD();
   UInt rs1  = GET_RS1();
   UInt rs2  = GET_RS2();
   Bool mask = GET_VMASK();

   switch (GET_FUNCT6()) {
      /*
       * Vector Single-Width Floating-Point Add/Subtract Instructions
       */
      case 0b000000:
         GETC_VBinopOPF(vfadd);
         return True;
      case 0b000010:
         GETC_VBinopOPF(vfsub);
         return True;
      case 0b100111:
         GETC_VBinopOPF_F(vfrsub);
         return True;

      /*
       * Vector Widening Floating-Point Add/Subtract Instructions
       */
      case 0b110000:
         GETC_VWBinopOPF(vfwadd, GETV_VopWidenD);
         return True;
      case 0b110010:
         GETC_VWBinopOPF(vfwsub, GETV_VopWidenD);
         return True;
      case 0b110100:
         GETC_VWBinopOPF(vfwaddw, GETV_VopWidenD | GETV_VopWidenS2);
         return True;
      case 0b110110:
         GETC_VWBinopOPF(vfwsubw, GETV_VopWidenD | GETV_VopWidenS2);
         return True;
      /*
       * Vector Single-Width Floating-Point Multiply/Divide Instructions
       */
      case 0b100100:
         GETC_VBinopOPF(vfmul);
         return True;
      case 0b100000:
         GETC_VBinopOPF(vfdiv);
         return True;
      case 0b100001:
         GETC_VBinopOPF_F(vfrdiv);
         return True;
      /*
       * Vector Widening Floating-Point Multiply
       */
      case 0b111000:
         GETC_VWBinopOPF(vfwmul, GETV_VopWidenD);
         return True;
      /*
       * Vector Single-Width Floating-Point Fused Multiply-Add Instructions
       */
      case 0b101100:
         GETC_VBinopOPF2(vfmacc, GETV_VopAccD);
         return True;
      case 0b101101:
         GETC_VBinopOPF2(vfnmacc, GETV_VopAccD);
         return True;
      case 0b101110:
         GETC_VBinopOPF2(vfmsac, GETV_VopAccD);
         return True;
      case 0b101111:
         GETC_VBinopOPF2(vfnmsac, GETV_VopAccD);
         return True;
      case 0b101000:
         GETC_VBinopOPF2(vfmadd, GETV_VopAccD);
         return True;
      case 0b101001:
         GETC_VBinopOPF2(vfnmadd, GETV_VopAccD);
         return True;
      case 0b101010:
         GETC_VBinopOPF2(vfmsub, GETV_VopAccD);
         return True;
      case 0b101011:
         GETC_VBinopOPF2(vfnmsub, GETV_VopAccD);
         return True;
      /*
       * Vector Widening Floating-Point Fused Multiply-Add Instructions
       */
      case 0b111100:
         GETC_VBinopOPF2(vfwmacc, GETV_VopAccD | GETV_VopWidenD);
         return True;
      case 0b111101:
         GETC_VBinopOPF2(vfwnmacc, GETV_VopAccD | GETV_VopWidenD);
         return True;
      case 0b111110:
         GETC_VBinopOPF2(vfwmsac, GETV_VopAccD | GETV_VopWidenD);
         return True;
      case 0b111111:
         GETC_VBinopOPF2(vfwnmsac, GETV_VopAccD | GETV_VopWidenD);
         return True;
      /* VFUnary1 */
      case 0b100011:
         /*
          * Vector Floating-Point Square-Root Instruction
          */
         if (rs1 == 0b00000) {
            GETC_VUnopOPF_V(vfsqrt);
            return True;
         /*
          * Vector Floating-Point Classify Instruction
          */
         } else if (rs1 == 0b10000) {
            GETC_VUnopOPF_V(vfclass);
            return True;
         }
         break;
      /*
       * Vector Floating-Point MIN/MAX Instructions
       */
      case 0b000100:
         GETC_VBinopOPF(vfmin);
         return True;
      case 0b000110:
         GETC_VBinopOPF(vfmax);
         return True;
      /*
       * Vector Floating-Point Sign-Injection Instructions
       */
      case 0b001000:
         GETC_VBinopOPF(vfsgnj);
         return True;
      case 0b001001:
         GETC_VBinopOPF(vfsgnjn);
         return True;
      case 0b001010:
         GETC_VBinopOPF(vfsgnjx);
         return True;

      /*
       * Vector Floating-Point Compare Instructions
       */
      // TODO

      /*
       * Vector Floating-Point Merge Instruction
       */
      case 0b010111:
         if (!mask) {
            GETC_VBinopOPF_F(vfmerge);
         } else {
            GETC_VUnopOPF_F(vfmerge);
         }
         return True;

      /* VFUnary0 */
      case 0b100010:
         /*
          * Single-Width Floating-Point/Integer Type-Convert Instructions
          */
         if (rs1 == 0b00000) {
            GETC_VUnopOPF_V(vfcvt_xu_f);
            return True;
         } else if (rs1 == 0b00001) {
            GETC_VUnopOPF_V(vfcvt_x_f);
            return True;
         } else if (rs1 == 0b00010) {
            GETC_VUnopOPF_V(vfcvt_f_xu);
            return True;
         } else if (rs1 == 0b00011) {
            GETC_VUnopOPF_V(vfcvt_f_x);
            return True;
         }
         /*
          * Widening Floating-Point/Integer Type-Convert Instructions
          */
         // TODO

         /*
          * Narrowing Floating-Point/Integer Type-Convert Instructions
          */
         // TODO

      default:
         break;
   }

   return False;
}

static Bool dis_RV64V0p7_arith(/*MB_OUT*/ DisResult* dres,
                               /*OUT*/ IRSB*         irsb,
                               UInt                  insn)
{
   Bool ok = False;

   switch (GET_FUNCT3()) {
      case RV64_SOPC_OPIVV:
      case RV64_SOPC_OPIVX:
      case RV64_SOPC_OPIVI:
         ok = dis_RV64V0p7_arith_OPI(dres, irsb, insn);
         break;
      case RV64_SOPC_OPMVV:
      case RV64_SOPC_OPMVX:
         ok = dis_RV64V0p7_arith_OPM(dres, irsb, insn);
         break;
      case RV64_SOPC_OPFVV:
      case RV64_SOPC_OPFVF:
         ok = dis_RV64V0p7_arith_OPF(dres, irsb, insn);
         break;
      default:
         vassert(0);
   }
   return ok;
}

static Bool dis_RV64V0p7(/*MB_OUT*/ DisResult* dres,
                         /*OUT*/ IRSB*         irsb,
                         UInt                  insn)
{
   Bool ok = False;

   switch (GET_OPCODE()) {
      case OPC_OP_V:
         if (GET_FUNCT3() == RV64_SOPC_OPCFG)
            return dis_RV64V0p7_cfg(dres, irsb, insn);
         ok = dis_RV64V0p7_arith(dres, irsb, insn);
         break;
      case OPC_LOAD_FP:
      case OPC_STORE_FP:
         ok = dis_RV64V0p7_ldst(dres, irsb, insn);
         break;
      default:
         break;
   }

   return ok;
}

/*--------------------------------------------------------------------*/
/*--- end                                  guest_riscv64V0p7_toIR.c --*/
/*--------------------------------------------------------------------*/
