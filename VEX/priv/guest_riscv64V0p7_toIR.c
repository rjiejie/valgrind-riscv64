
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
   IRTemp xrm = newTemp(irsb, Ity_I32);

   UInt rd   = GET_RD();
   UInt rs1  = GET_RS1();
   UInt rs2  = GET_RS2();
   Bool mask = GET_VMASK();

   switch (GET_FUNCT6()) {
      case 0b000000:
         GETC_VBinopOPI(vadd);
         return True;
      /*
       * Vector Single-Width Saturating Add and Subtract
       */
      case 0b100000:
         GETC_VBinopSAT(vsaddu);
         return True;
      case 0b100001:
         GETC_VBinopSAT(vsadd);
         return True;
      case 0b100010:
         GETC_VBinopSAT_VX(vssubu);
         return True;
      case 0b100011:
         GETC_VBinopSAT_VX(vssub);
         return True;
      /*
       * Vector Single-Width Averaging Add and Subtract
       */
      case 0b100100:
         GETC_VBinopSAT(vaadd);
         return True;
      case 0b100110:
         GETC_VBinopSAT_VX(vasub);
         return True;
      /*
       * Vector Single-Width Fractional Multiply with Rounding and Saturation
       */
      case 0b100111:
         GETC_VBinopSAT_VX(vsmul);
         return True;
      /*
       * Vector Widening Saturating Scaled Multiply-Add
       */
      case 0b111100:
         GETC_VBinopSAT2_VX_VAR(vwsmaccu,  GETV_VopAccD | GETV_VopWidenD);
         return True;
      case 0b111101:
         GETC_VBinopSAT2_VX_VAR(vwsmacc,   GETV_VopAccD | GETV_VopWidenD);
         return True;
      case 0b111110:
         GETC_VBinopSAT2_VX_VAR(vwsmaccsu, GETV_VopAccD | GETV_VopWidenD);
         return True;
      case 0b111111:
         GETC_VBinopSAT2_X_VAR(vwsmaccus,  GETV_VopAccD | GETV_VopWidenD);
         return True;
      /*
       * Vector Single-Width Scaling Shift Instructions
       */
      case 0b101010:
         GETC_VBinopSAT(vssrl);
         return True;
      case 0b101011:
         GETC_VBinopSAT(vssra);
         return True;
      /*
       * Vector Narrowing Fixed-Point Clip Instructions
       */
      case 0b101110:
         GETC_VBinopSAT_VAR(vnclipu, GETV_VopWidenS2);
         return True;
      case 0b101111:
         GETC_VBinopSAT_VAR(vnclip, GETV_VopWidenS2);
         return True;
      /*
       * Vector Slideup Instructions
       */
      case 0b001110:
         GETC_VBinopOPI_XI(vslideup);
         return True;
      /*
       * Vector Slidedown Instructions
       */
      case 0b001111:
         GETC_VBinopOPI_XI(vslidedown);
         return True;
      /*
       * Vector Register Gather Instruction
       */
      case 0b001100:
         GETC_VBinopOPI(vrgather);
         return True;
      /*
       * Vector Widening Integer Reduction Instructions
       */
      case 0b110000:
         GETC_VBinopOPI_V_VAR(vwredsumu, GETV_VopM1D);
         return True;
      case 0b110001:
         GETC_VBinopOPI_V_VAR(vwredsum, GETV_VopM1D);
         return True;
      default:
         break;
   }

   return False;
}

static Bool dis_RV64V0p7_arith_OPM(/*MB_OUT*/ DisResult* dres,
                                   /*OUT*/ IRSB*         irsb,
                                   UInt                  insn)
{
   IRDirty *d  = NULL;
   void *fAddr = NULL;
   const HChar *fName = NULL;
   IRExpr **args = NULL;
   UInt temp = 0;
   IRTemp ret = newTemp(irsb, Ity_I64);

   UInt rd   = GET_RD();
   UInt rs1  = GET_RS1();
   UInt rs2  = GET_RS2();
   Bool mask = GET_VMASK();

   switch (GET_FUNCT6()) {
      /*
       * Integer Extract Instruction
       */
      case 0b001100:
         fName = GETN_VBinopVX(vext);
         fAddr = GETA_VBinopVX(vext);

         args = mkIRExprVec_3(IRExpr_GSPTR(), mkU64(offsetVReg(rs2)), mkU64(offsetIReg64(rs1)));
         d = unsafeIRDirty_1_N(ret, 0, fName, fAddr, args);

         d->nFxState = 2;
         d->fxState[0].fx        = Ifx_Read;
         d->fxState[0].offset    = offsetVReg(rs2);
         d->fxState[0].size      = host_VLENB;
         d->fxState[1].fx        = Ifx_Read;
         d->fxState[1].offset    = offsetIReg64(rs1);
         d->fxState[1].size      = 8;
         stmt(irsb, IRStmt_Dirty(d));

         // TODO get sew
         UInt sew = 0;
         putIReg64(irsb, rd,
                   sew == 8    ? unop(Iop_8Uto64,  mkexpr(ret))
                   : sew == 16 ? unop(Iop_16Uto64, mkexpr(ret))
                   : sew == 32 ? unop(Iop_32Uto64, mkexpr(ret))
                               : mkexpr(ret));
         DIP("%s(%s, %s, %s)\n", fName, nameIReg(rd), nameVReg(rs2), nameIReg(rs1));
         return True;
      /*
       * Integer Scalar Move Instruction
       */
      case 0b001101:
         GETC_VUnopOPI_X_VAR(vmvs, GETV_VopM1D);
         return True;
      /*
       * Vector Slide1up Instructions
       */
      case 0b001110:
         GETC_VBinopOPI_X(vslide1up);
         return True;
      /*
       * Vector Slide1down Instructions
       */
      case 0b001111:
         GETC_VBinopOPI_X(vslide1down);
         return True;
      /*
       * Vector Compress Instruction
       */
      case 0b010111:
         GETC_VBinopOPI_V(vcompress);
         return True;
      /*
       * Vector Single-Width Integer Reduction Instructions
       */
      case 0b000000:
         GETC_VBinopOPI_V_VAR(vredsum, GETV_VopM1D);
         return True;
      case 0b000110:
         GETC_VBinopOPI_V_VAR(vredmaxu, GETV_VopM1D);
         return True;
      case 0b000111:
         GETC_VBinopOPI_V_VAR(vredmax, GETV_VopM1D);
         return True;
      case 0b000100:
         GETC_VBinopOPI_V_VAR(vredminu, GETV_VopM1D);
         return True;
      case 0b000101:
         GETC_VBinopOPI_V_VAR(vredmin, GETV_VopM1D);
         return True;
      case 0b000001:
         GETC_VBinopOPI_V_VAR(vredand, GETV_VopM1D);
         return True;
      case 0b000010:
         GETC_VBinopOPI_V_VAR(vredor, GETV_VopM1D);
         return True;
      case 0b000011:
         GETC_VBinopOPI_V_VAR(vredxor, GETV_VopM1D);
         return True;
      /*
       * Vector Mask-Register Logical Instructions
       */
      case 0b011001:
         GETC_VBinopOPI_V_VAR(vmand, GETV_VopM1D | GETV_VopM1S);
         return True;
      case 0b011101:
         GETC_VBinopOPI_V_VAR(vmnand, GETV_VopM1D | GETV_VopM1S);
         return True;
      case 0b011000:
         GETC_VBinopOPI_V_VAR(vmandnot, GETV_VopM1D | GETV_VopM1S);
         return True;
      case 0b011011:
         GETC_VBinopOPI_V_VAR(vmxor, GETV_VopM1D | GETV_VopM1S);
         return True;
      case 0b011010:
         GETC_VBinopOPI_V_VAR(vmor, GETV_VopM1D | GETV_VopM1S);
         return True;
      case 0b011110:
         GETC_VBinopOPI_V_VAR(vmnor, GETV_VopM1D | GETV_VopM1S);
         return True;
      case 0b011100:
         GETC_VBinopOPI_V_VAR(vmornot, GETV_VopM1D | GETV_VopM1S);
         return True;
      case 0b011111:
         GETC_VBinopOPI_V_VAR(vmxnor, GETV_VopM1D | GETV_VopM1S);
         return True;
      /*
       * Vector mask population count vmpopc
       */
      case 0b010100: {
         IRTemp dret = newTemp(irsb, Ity_I64);
         fName       = mask ? GETN_VUnopV(vmpopc) : GETN_VUnopV_M(vmpopc);
         fAddr       = mask ? GETA_VUnopV(vmpopc) : GETA_VUnopV_M(vmpopc);

         args = mkIRExprVec_4(IRExpr_GSPTR(), mkU64(offsetVReg(rs2)), mkU64(0),
                              mkU64(offsetVReg(0)));
         d    = unsafeIRDirty_1_N(dret, 0, fName, fAddr, args);

         d->nFxState          = mask ? 1 : 2;
         d->fxState[0].fx     = Ifx_Read;
         d->fxState[0].offset = offsetVReg(rs2);
         d->fxState[0].size   = host_VLENB;
         d->fxState[1].fx     = Ifx_Read;
         d->fxState[1].offset = offsetVReg(0);
         d->fxState[1].size   = host_VLENB;
         stmt(irsb, IRStmt_Dirty(d));

         putIReg64(irsb, rd, mkexpr(dret));
         DIP("%s(%s, %s)\n", fName, nameIReg(rd), nameVReg(rs2));
         return True;
      }
      /*
       * vmfirst find-first-set mask bit
       */
      // TODO

      /*
       * VMUNARY0
       */
      case 0b010110:
         switch (rs1) {
            /*
             * vmsbf.m set-before-first mask bit
             */
            case 0b00001:
               GETC_VUnopOPI_V_VAR(vmsbf, GETV_VopM1D | GETV_VopM1S);
               return True;
            /*
             * vmsif.m set-including-first mask bit
             */
            case 0b00011:
               GETC_VUnopOPI_V_VAR(vmsif, GETV_VopM1D | GETV_VopM1S);
               return True;
            /*
             * vmsof.m set-only-first mask bit
             */
            case 0b00010:
               GETC_VUnopOPI_V_VAR(vmsof, GETV_VopM1D | GETV_VopM1S);
               return True;
            /*
             * Vector Iota Instruction
             */
            case 0b10000:
               GETC_VUnopOPI_V_VAR(viota, GETV_VopM1S);
               return True;
            /*
             * Vector Element Index Instruction
             */
            case 0b10001:
               GETC_VUnopOPI_V(vid);
               return True;
            default:
               break;
         }
         break;
      default:
         break;
   }

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
         GETC_VBinopOPF_VAR(vfwadd, GETV_VopWidenD);
         return True;
      case 0b110010:
         GETC_VBinopOPF_VAR(vfwsub, GETV_VopWidenD);
         return True;
      case 0b110100:
         GETC_VBinopOPF_VAR(vfwaddw, GETV_VopWidenD | GETV_VopWidenS2);
         return True;
      case 0b110110:
         GETC_VBinopOPF_VAR(vfwsubw, GETV_VopWidenD | GETV_VopWidenS2);
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
         GETC_VBinopOPF_VAR(vfwmul, GETV_VopWidenD);
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
      case 0b011000:
         GETC_VBinopOPF_VAR(vmfeq, GETV_VopM1D);
         return True;
      case 0b011100:
         GETC_VBinopOPF_VAR(vmfne, GETV_VopM1D);
         return True;
      case 0b011011:
         GETC_VBinopOPF_VAR(vmflt, GETV_VopM1D);
         return True;
      case 0b011001:
         GETC_VBinopOPF_VAR(vmfle, GETV_VopM1D);
         return True;
      case 0b011010:
         GETC_VBinopOPF_VAR(vmford, GETV_VopM1D);
         return True;
      case 0b011101:
         GETC_VBinopOPF_F_VAR(vmfgt, GETV_VopM1D);
         return True;
      case 0b011111:
         GETC_VBinopOPF_F_VAR(vmfge, GETV_VopM1D);
         return True;
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
         switch (rs1) {
            /*
             * Single-Width Floating-Point/Integer Type-Convert Instructions
             */
            case 0b00000:
               GETC_VUnopOPF_V(vfcvt_xu_f);
               return True;
            case 0b00001:
               GETC_VUnopOPF_V(vfcvt_x_f);
               return True;
            case 0b00010:
               GETC_VUnopOPF_V(vfcvt_f_xu);
               return True;
            case 0b00011:
               GETC_VUnopOPF_V(vfcvt_f_x);
               return True;
            /*
             * Widening Floating-Point/Integer Type-Convert Instructions
             */
            case 0b01000:
               GETC_VUnopOPF_V_VAR(vfwcvt_xu_f, GETV_VopWidenD);
               return True;
            case 0b01001:
               GETC_VUnopOPF_V_VAR(vfwcvt_x_f, GETV_VopWidenD);
               return True;
            case 0b01010:
               GETC_VUnopOPF_V_VAR(vfwcvt_f_xu, GETV_VopWidenD);
               return True;
            case 0b01011:
               GETC_VUnopOPF_V_VAR(vfwcvt_f_x, GETV_VopWidenD);
               return True;
            case 0b01100:
               GETC_VUnopOPF_V_VAR(vfwcvt_f_f, GETV_VopWidenD);
               return True;
            /*
             * Narrowing Floating-Point/Integer Type-Convert Instructions
             */
            case 0b10000:
               GETC_VUnopOPF_V_VAR(vfncvt_xu_f, GETV_VopWidenS2);
               return True;
            case 0b10001:
               GETC_VUnopOPF_V_VAR(vfncvt_x_f, GETV_VopWidenS2);
               return True;
            case 0b10010:
               GETC_VUnopOPF_V_VAR(vfncvt_f_xu, GETV_VopWidenS2);
               return True;
            case 0b10011:
               GETC_VUnopOPF_V_VAR(vfncvt_f_x, GETV_VopWidenS2);
               return True;
            case 0b10100:
               GETC_VUnopOPF_V_VAR(vfncvt_f_f, GETV_VopWidenS2);
               return True;
            default:
               break;
         }
         break;
      /*
       * Floating-Point Scalar Move Instructions
       */
      case 0b001100: {
         IRTemp dret = newTemp(irsb, Ity_F64);
         fName = GETN_VUnopV(vfmv);
         fAddr = GETA_VUnopV(vfmv);

         args = mkIRExprVec_3(IRExpr_GSPTR(), mkU64(offsetVReg(rs2)), mkU64(0));
         d = unsafeIRDirty_1_N(dret, 0, fName, fAddr, args);

         d->nFxState = 1;
         d->fxState[0].fx        = Ifx_Read;
         d->fxState[0].offset    = offsetVReg(rs2);
         d->fxState[0].size      = host_VLENB;
         stmt(irsb, IRStmt_Dirty(d));

         // TODO get sew
         UInt sew = 0;
         putFReg64(irsb, rd,
                   sew == 8    ? binop(Iop_Or64, mkexpr(dret), mkU64(~0xFFUL))
                   : sew == 16 ? binop(Iop_Or64, mkexpr(dret), mkU64(~0xFFFFUL))
                   : sew == 32 ? binop(Iop_Or64, mkexpr(dret), mkU64(~0xFFFFFFFFUL))
                               : mkexpr(dret));
         DIP("%s(%s, %s)\n", fName, nameFReg(rd), nameVReg(rs2));
         return True;
      }
      case 0b001101:
         GETC_VUnopOPF_F_VAR(vfmvs, GETV_VopM1D);
         return True;
      /*
       * Vector Single-Width Floating-Point Reduction Instructions
       */
      case 0b000011:
         GETC_VBinopOPF_V_VAR(vfredosum, GETV_VopM1D);
         return True;
      case 0b000001:
         GETC_VBinopOPF_V_VAR(vfredsum, GETV_VopM1D);
         return True;
      case 0b000111:
         GETC_VBinopOPF_V_VAR(vfredmax, GETV_VopM1D);
         return True;
      case 0b000101:
         GETC_VBinopOPF_V_VAR(vfredmin, GETV_VopM1D);
         return True;
      /*
       * Vector Widening Floating-Point Reduction Instructions
       */
      case 0b110011:
         GETC_VBinopOPF_V_VAR(vfwredosum, GETV_VopM1D);
         return True;
      case 0b110001:
         GETC_VBinopOPF_V_VAR(vfwredsum, GETV_VopM1D);
         return True;
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
