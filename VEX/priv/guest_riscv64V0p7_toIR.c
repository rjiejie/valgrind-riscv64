
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

/* Read a value from VXSAT CSR */
static IRExpr* getVxsat0p7(void) {
   IRExpr* fcsr = getFCSR();
   return binop(Iop_And32, binop(Iop_Shr32, fcsr, mkU8(8)), mkU32(1));
}

/* Write a value to VXSAT CSR */
static void putVxsat0p7(IRSB* irsb, IRExpr* e) {
   IRType ty = typeOfIRExpr(irsb->tyenv, e);
   vassert(ty == Ity_I32);
   IRExpr* fcsr = getFCSR();
   IRExpr* new_fcsr = binop(Iop_Or32, binop(Iop_And32, fcsr, mkU32(0xFFFFFEFF)),
                            binop(Iop_Shl32, e, mkU8(8)));
   putFCSR(irsb, new_fcsr);
}

/* Read a value from VXRM CSR */
static IRExpr* getVxrm0p7(void) {
   IRExpr* fcsr = getFCSR();
   return binop(Iop_And32, binop(Iop_Shr32, fcsr, mkU8(9)), mkU32(3));
}

/* Write a value to VXRM CSR */
static void putVxrm0p7(IRSB* irsb, IRExpr* e) {
   IRType ty = typeOfIRExpr(irsb->tyenv, e);
   vassert(ty == Ity_I32);
   IRExpr* fcsr = getFCSR();
   IRExpr* new_fcsr = binop(Iop_Or32, binop(Iop_And32, fcsr, mkU32(0xFFFFF9FF)),
                            binop(Iop_Shl32, e, mkU8(9)));
   putFCSR(irsb, new_fcsr);
}

#include "guest_riscv64V0p7_helpers.c"

static Bool dis_RV64V0p7_csr(/*MB_OUT*/ DisResult* dres,
                             /*OUT*/ IRSB* irsb,
                             UInt insn) {
   UInt rd  = GET_RD();
   UInt rs1 = GET_RS1();
   UInt csr = INSN(31, 20);

   if (GET_FUNCT3() == RV64_SOPC_CSRRW) {
      DIP("csrrw %s, %s, %s\n", nameIReg(rd), nameVCSR(csr), nameIReg(rs1));
      switch (csr) {
         case 0x008: {
            /* vstart: URW */
            IRExpr *eR1 = getIReg64(rs1);
            IRExpr *orig_vstart = getVStart();

            /* This is quoted from RVV 0.7.1 spec 3.4:
               The vstart CSR is defined to have only enough writable bits to hold the
               largest element index (one less than the maximum VLMAX) or lg2(VLEN) bits.
               The upper bits of the vstart CSR are hardwired to zero (reads zero, writes
               ignored).*/
            ULong eff_mask = 0xFFFFFFFFFFFFFFFF >> (__builtin_clzll(host_VLENB) - 2);
            IRExpr* eff_vstart = binop(Iop_And64, mkU64(eff_mask), eR1);
            putVStart(irsb, eff_vstart);
            if (rd != 0)
               putIReg64(irsb, rd, orig_vstart);
            return True;
         }
         case 0x009: {
            /* vxsat: URW */
            IRExpr *eR1 = getIReg32(rs1);

            /* vxsat has a single effective bit. */
            IRExpr *eff_vxsat = binop(Iop_And32, mkU32(0x01), eR1);
            IRExpr *orig_vxsat = getVxsat0p7();
            putVxsat0p7(irsb, eff_vxsat);
            if (rd != 0)
               putIReg32(irsb, rd, orig_vxsat);
            return True;
         }
         case 0x00A: {
            /* vxrm: URW */
            IRExpr *eR1 = getIReg32(rs1);
            IRExpr *orig_vxrm = getVxrm0p7();

            /* vxrm has 2 effective bits in [1:0], [XLEN-1:2] bits should be zeros. */
            IRExpr *eff_vxrm = binop(Iop_And32, mkU32(0x03), eR1);
            putVxrm0p7(irsb, eff_vxrm);
            if (rd != 0)
               putIReg32(irsb, rd, orig_vxrm);
            return True;
         }
         /* VL/VTYPE are read-only CSRs */
         default:
            return False;
      }
   }

   if (GET_FUNCT3() == RV64_SOPC_CSRRS) {
      DIP("csrrs %s, %s, %s\n", nameIReg(rd), nameVCSR(csr), nameIReg(rs1));
      switch (csr) {
         case 0x008: {
            /* vstart: URW */
            IRExpr *orig_vstart = getVStart();
            IRExpr *eR1 = getIReg64(rs1);
            ULong eff_mask = 0xFFFFFFFFFFFFFFFF >> (__builtin_clzll(host_VLENB) - 2);
            IRExpr* eff_bits = binop(Iop_And64, mkU64(eff_mask), eR1);
            IRExpr* eff_vstart = binop(Iop_Or64, eff_bits, orig_vstart);
            putVStart(irsb, eff_vstart);
            if (rd != 0)
               putIReg64(irsb, rd, orig_vstart);
            return True;
         }
         case 0x009: {
            /* vxsat: URW */
            IRExpr *orig_vxsat = getVxsat0p7();
            IRExpr *eR1 = getIReg32(rs1);
            IRExpr* eff_bits = binop(Iop_And32, mkU32(0x01), eR1);
            IRExpr* eff_vxsat = binop(Iop_Or32, eff_bits, orig_vxsat);
            putVxsat0p7(irsb, eff_vxsat);
            if (rd != 0)
               putIReg32(irsb, rd, orig_vxsat);
            return True;
         }
         case 0x00A: {
            /* vxrm: URW */
            IRExpr *orig_vxrm = getVxrm0p7();
            IRExpr *eR1 = getIReg32(rs1);
            IRExpr* eff_bits = binop(Iop_And32, mkU32(0x03), eR1);
            IRExpr* eff_vxrm = binop(Iop_Or32, eff_bits, orig_vxrm);
            putVxrm0p7(irsb, eff_vxrm);
            if (rd != 0)
               putIReg32(irsb, rd, orig_vxrm);
            return True;
         }
         case 0xC20: {
            /* VL: URO */
            if (rs1 != 0)
               /* Attempt to write a value to URO CSR. */
               return False;
            if (rd != 0)
               putIReg64(irsb, rd, getVL());
            return True;
         }
         case 0xC21: {
            /* VTYPE: URO */
            if (rs1 != 0)
               /* Attempt to write a value to URO CSR. */
               return False;
            if (rd != 0)
               putIReg64(irsb, rd, getVType());
            return True;
         }
         default:
            return False;
      }
   }

   if (GET_FUNCT3() == RV64_SOPC_CSRRC) {
      DIP("csrrc %s, %s, %s\n", nameIReg(rd), nameVCSR(csr), nameIReg(rs1));
      switch (csr) {
         case 0x008: {
            /* vstart: URW */
            IRExpr *orig_vstart = getVStart();
            IRExpr *eR1 = getIReg64(rs1);
            ULong eff_mask = 0xFFFFFFFFFFFFFFFF >> (__builtin_clzll(host_VLENB) - 2);
            IRExpr* eff_bits = binop(Iop_And64, mkU64(eff_mask), eR1);
            IRExpr* eff_vstart = binop(Iop_And64, binop(Iop_Xor64, eff_bits, orig_vstart),
                                       orig_vstart);
            putVStart(irsb, eff_vstart);
            if (rd != 0)
               putIReg64(irsb, rd, orig_vstart);
            return True;
         }
         case 0x009: {
            /* vxsat: URW */
            IRExpr *orig_vxsat = getVxsat0p7();
            IRExpr *eR1 = getIReg32(rs1);
            IRExpr* eff_bits = binop(Iop_And32, mkU32(0x01), eR1);
            IRExpr* eff_vxsat = binop(Iop_And32, binop(Iop_Xor32, eff_bits, orig_vxsat),
                                      orig_vxsat);
            putVxsat0p7(irsb, eff_vxsat);
            if (rd != 0)
               putIReg32(irsb, rd, orig_vxsat);
            return True;
         }
         case 0x00A: {
            /* vxrm: URW */
            IRExpr *orig_vxrm = getVxrm0p7();
            IRExpr *eR1 = getIReg32(rs1);
            IRExpr* eff_bits = binop(Iop_And32, mkU32(0x03), eR1);
            IRExpr* eff_vxrm = binop(Iop_And32, binop(Iop_Xor32, eff_bits, orig_vxrm),
                                     orig_vxrm);
            putVxrm0p7(irsb, eff_vxrm);
            if (rd != 0)
               putIReg32(irsb, rd, orig_vxrm);
            return True;
         }
         case 0xC20: {
            /* VL: URO */
            if (rs1 != 0)
               /* Attempt to write a value to URO CSR. */
               return False;
            if (rd != 0)
               putIReg64(irsb, rd, getVL());
            return True;
         }
         case 0xC21: {
            /* VTYPE: URO */
            if (rs1 != 0)
               /* Attempt to write a value to URO CSR. */
               return False;
            if (rd != 0)
               putIReg64(irsb, rd, getVType());
            return True;
         }
         default:
            return False;
      }
   }
   return False;
}

static Bool dis_RV64V0p7_cfg(/*MB_OUT*/ DisResult* dres,
                             /*OUT*/ IRSB*         irsb,
                             UInt                  insn,
                             Addr                  guest_pc_curr_instr)
{
   Bool is_vsetvl = INSN(31, 31);
   UInt rd  = GET_RD();
   UInt rs1 = GET_RS1();

   if (is_vsetvl) {
      /* In vsetvl case, we cannot immediately get the written value from GPR,
         therefore we terminate translation and return to scheduler. */
      UInt   rs2 = GET_RS2();
      IRExpr* s2 = getIReg64(rs2);
      putVType(irsb, binop(Iop_And64, s2, mkU64(0x007F)));
   } else {
      UInt imm11    = INSN(30, 20);
      UInt reserved = imm11 & 0xFFFFFF80;
      UInt ediv     = imm11 & 0x00000060;
      vassert(ediv == 0 && reserved == 0);
      putVType(irsb, binop(Iop_And64, mkU64(imm11), mkU64(0x007F)));
   }

   /* Update VL */
   IRExpr* new_vl = NULL;
   IRExpr* vtype  = getVType();
   IRExpr* lmul   = binop(Iop_And64, vtype, mkU64(0x03));
   IRExpr* sew    = binop(Iop_Shr64, binop(Iop_And64, vtype, mkU64(0x001C)), mkU8(2));
   IRExpr* vl_max = binop(Iop_Shr64, binop(Iop_Shl64, mkU64(host_VLENB), unop(Iop_64to8, lmul)),
                          unop(Iop_64to8, sew));
   if (rs1 == 0)
      new_vl = vl_max;
   else
      new_vl = IRExpr_ITE(binop(Iop_CmpLT64U, getIReg64(rs1), vl_max),
                          getIReg64(rs1), vl_max);
   putVL(irsb, new_vl);

   if (rd != 0)
      putIReg64(irsb, rd, new_vl);

   /* Update PC as we will exit translation */
   putPC(irsb, mkU64(guest_pc_curr_instr + 4));

   /* There is a potential vector CSR changing, we stop here */
   dres->whatNext    = Dis_StopHere;
   dres->jk_StopHere = Ijk_ExitBB;

   return True;
}

/* Criteria are described in RVV 0.7.1 spec 7.2:
   Vector Load/Store Addressing Modes */
static inline Bool RVV0p7_is_unitstride(UInt insn, Bool isLD) {
   if (isLD)
      /* zero-exteneded unit-stride || sign-extended unit-stride */
      return GET_MOP() == 0b000 || GET_MOP() == 0b100;
   else
      /* unit-stride */
      return GET_MOP() == 0b000;
}

static inline Bool RVV0p7_is_strided(UInt insn, Bool isLD) {
   if (isLD)
      /* zero-exteneded strided || sign-extended strided */
      return GET_MOP() == 0b010 || GET_MOP() == 0b110;
   else
      /* strided */
      return GET_MOP() == 0b010;
}

static inline Bool RVV0p7_is_indexed(UInt insn, Bool isLD, UInt nf) {
   if (isLD)
      /* zero-exteneded indexed || sign-extended indexed */
      return GET_MOP() == 0b011 || GET_MOP() == 0b111;
   else {
      if (nf == 1)
         /* ordered indexed || unordered indexed */
         return GET_MOP() == 0b011 || GET_MOP() == 0b111;
      else
         /* ordered indexed in segment */
         return GET_MOP() == 0b011;
   }
} 

static inline Bool RVV0p7_is_normal_load(UInt nf, Bool isLD) {
   return nf == 1 && isLD;
}

static inline Bool RVV0p7_is_normal_store(UInt nf, Bool isLD) {
   return nf == 1 && !isLD;
}

static inline Bool RVV0p7_is_seg_load(UInt nf, Bool isLD) {
   return nf != 1 && isLD;
}

static inline Bool RVV0p7_is_seg_store(UInt nf, Bool isLD) {
   return nf != 1 && !isLD;
}

static Bool dis_RV64V0p7_ldst(/*MB_OUT*/ DisResult* dres,
                              /*OUT*/ IRSB*         irsb,
                              UInt                  insn)
{
   Bool isLD          = GET_OPCODE() == OPC_LOAD_FP;
   IRDirty *d         = NULL;
   void *fAddr        = NULL;
   const HChar *fName = NULL;
   IRExpr **args      = NULL;
   ULong width;

   UInt v    = GET_RD();     /* vd for load *or* vs3 for store */
   UInt rs   = GET_RS1();    /* base address register rs1 */
   UInt nf   = GET_NF() + 1; /* nf value */
   Bool mask = GET_VMASK();  /* if mask is enabled? */

   /* unit stride */
   if (RVV0p7_is_unitstride(insn, isLD)) {
      /* unit-stride normal load */
      if (RVV0p7_is_normal_load(nf, isLD)) {
         switch (GET_FUNCT3()) {
            case 0b000: {                 /* 8-bit load */
               width = 1;
               if (GET_MOP() == 0b000) {  /* zero-extended */
                  GETC_VLDST(vlbu);
               } else {                   /* signed-extended */
                  GETC_VLDST(vlb);
               }
               return True;
            }
            case 0b101: {                 /* 16-bit load */
               width = 2;
               if (GET_MOP() == 0b000) {  /* zero-extended */
                  GETC_VLDST(vlhu);
               } else {                   /* signed-extended */
                  GETC_VLDST(vlh);
               }
               return True;
            }
            case 0b110: {                 /* 32-bit load */
               width = 4;
               if (GET_MOP() == 0b000) {  /* zero-extended */
                  GETC_VLDST(vlwu);
               } else {                   /* signed-extended */
                  GETC_VLDST(vlw);
               }
               return True;
            }
            case 0b111: {                 /* SEW load */
               width = extract_sew(guest_VFLAG);
               GETC_VLDST(vle);
               return True;
            }
            default:
               return False;
         }
      }

      if (RVV0p7_is_seg_load(nf, isLD)) { /* unit-stride segment load */
         switch (GET_FUNCT3()) {
            case 0b000: {                 /* 8-bit load */
               width = 1;
               if (GET_MOP() == 0b000) {  /* zero-extended */
                  VSEG_DIS_NF_CASES(GETC_VSEGLDST, vlseg, bu);
               } else {                   /* signed-extended */
                  VSEG_DIS_NF_CASES(GETC_VSEGLDST, vlseg, b);
               }
               return True;
            }
            case 0b101: {                 /* 16-bit load */
               width = 2;
               if (GET_MOP() == 0b000) {  /* zero-extended */
                  VSEG_DIS_NF_CASES(GETC_VSEGLDST, vlseg, hu);
               } else {                   /* signed-extended */
                  VSEG_DIS_NF_CASES(GETC_VSEGLDST, vlseg, h);
               }
               return True;
            }
            case 0b110: {                 /* 32-bit load */
               width = 4;
               if (GET_MOP() == 0b000) {  /* zero-extended */
                  VSEG_DIS_NF_CASES(GETC_VSEGLDST, vlseg, wu);
               } else {                   /* signed-extended */
                  VSEG_DIS_NF_CASES(GETC_VSEGLDST, vlseg, w);
               }
               return True;
            }
            case 0b111: {                 /* SEW load */
               width = extract_sew(guest_VFLAG);
               VSEG_DIS_NF_CASES(GETC_VSEGLDST, vlseg, e);
               return True;
            }
            default:
               return False;
         }
      }
      /* unit-stride normal store */
      if (RVV0p7_is_normal_store(nf, isLD)){
         switch (GET_FUNCT3()) {
            case 0b000: {                 /* 8-bit store */
               width = 1;
               GETC_VLDST(vsb);
               return True;
            }
            case 0b101: {                 /* 16-bit store */
               width = 2;
               GETC_VLDST(vsh);
               return True;
            }
            case 0b110: {                 /* 32-bit store */
               width = 4;
               GETC_VLDST(vsw);
               return True;
            }
            case 0b111: {                 /* SEW store */
               width = extract_sew(guest_VFLAG);
               GETC_VLDST(vse);
               return True;
            }
            default:
               return False;
         }
      }

      if (RVV0p7_is_seg_store(nf, isLD)) { /* unit-stride segment store */
         switch (GET_FUNCT3()) {
            case 0b000: {                 /* 8-bit store */
               width = 1;
               VSEG_DIS_NF_CASES(GETC_VSEGLDST, vsseg, b);
               return True;
            }
            case 0b101: {                 /* 16-bit store */
               width = 2;
               VSEG_DIS_NF_CASES(GETC_VSEGLDST, vsseg, h);
               return True;
            }
            case 0b110: {                 /* 32-bit store */
               width = 4;
               VSEG_DIS_NF_CASES(GETC_VSEGLDST, vsseg, w);
               return True;
            }
            case 0b111: {                 /* SEW store */
               width = extract_sew(guest_VFLAG);
               VSEG_DIS_NF_CASES(GETC_VSEGLDST, vsseg, e);
               return True;
            }
            default:
               return False;
         }
      }
      return False;
   }

   /* strided */
   if (RVV0p7_is_strided(insn, isLD)) {                /* strided store */
      UInt rs2 = GET_RS2(); /* stride register */
      /* strided normal load */
      if (RVV0p7_is_normal_load(nf, isLD)) {
         switch (GET_FUNCT3()) {
            case 0b000: {                 /* 8-bit load */
               width = 1;
               if (GET_MOP() == 0b000) {  /* zero-extended */
                  GETC_VSLDST(vlsbu);
               } else {                   /* signed-extended */
                  GETC_VSLDST(vlsb);
               }
               return True;
            }
            case 0b101: {                 /* 16-bit load */
               width = 2;
               if (GET_MOP() == 0b000) {  /* zero-extended */
                  GETC_VSLDST(vlshu);
               } else {                   /* signed-extended */
                  GETC_VSLDST(vlsh);
               }
               return True;
            }
            case 0b110: {                 /* 32-bit load */
               width = 4;
               if (GET_MOP() == 0b000) {  /* zero-extended */
                  GETC_VSLDST(vlswu);
               } else {                   /* signed-extended */
                  GETC_VSLDST(vlsw);
               }
               return True;
            }
            case 0b111: {                 /* SEW load */
               width = extract_sew(guest_VFLAG);
               GETC_VSLDST(vlse);
               return True;
            }
         }
      }

      if (RVV0p7_is_seg_load(nf, isLD)) { /* strided segment load */
         switch (GET_FUNCT3()) {
            case 0b000: {                 /* 8-bit load */
               width = 1;
               if (GET_MOP() == 0b000) {  /* zero-extended */
                  VSEG_DIS_NF_CASES(GETC_VSSEGLDST, vlsseg, bu);
               } else {                   /* signed-extended */
                  VSEG_DIS_NF_CASES(GETC_VSSEGLDST, vlsseg, b);
               }
               return True;
            }
            case 0b101: {                 /* 16-bit load */
               width = 2;
               if (GET_MOP() == 0b000) {  /* zero-extended */
                  VSEG_DIS_NF_CASES(GETC_VSSEGLDST, vlsseg, hu);
               } else {                   /* signed-extended */
                  VSEG_DIS_NF_CASES(GETC_VSSEGLDST, vlsseg, h);
               }
               return True;
            }
            case 0b110: {                 /* 32-bit load */
               width = 4;
               if (GET_MOP() == 0b000) {  /* zero-extended */
                  VSEG_DIS_NF_CASES(GETC_VSSEGLDST, vlsseg, wu);
               } else {                   /* signed-extended */
                  VSEG_DIS_NF_CASES(GETC_VSSEGLDST, vlsseg, w);
               }
               return True;
            }
            case 0b111: {                 /* SEW load */
               width = extract_sew(guest_VFLAG);
               VSEG_DIS_NF_CASES(GETC_VSSEGLDST, vlsseg, e);
               return True;
            }
            default:
               return False;
         }
      }

      /* strided normal store */
      if (RVV0p7_is_normal_store(nf, isLD)) {
         switch (GET_FUNCT3()) {
            case 0b000: {                 /* 8-bit store */
               width = 1;
               GETC_VSLDST(vssb);
               return True;
            }
            case 0b101: {                 /* 16-bit store */
               width = 2;
               GETC_VSLDST(vssh);
               return True;
            }
            case 0b110: {                 /* 32-bit store */
               width = 4;
               GETC_VSLDST(vssw);
               return True;
            }
            case 0b111: {                 /* SEW store */
               width = extract_sew(guest_VFLAG);
               GETC_VSLDST(vsse);
               return True;
            }
            default:
               return False;
         }
      }

      if (RVV0p7_is_seg_store(nf, isLD)) { /* strided segment store */
         switch (GET_FUNCT3()) {
            case 0b000: {                 /* 8-bit store */
               width = 1;
               VSEG_DIS_NF_CASES(GETC_VSSEGLDST, vssseg, b);
               return True;
            }
            case 0b101: {                 /* 16-bit store */
               width = 2;
               VSEG_DIS_NF_CASES(GETC_VSSEGLDST, vssseg, h);
               return True;
            }
            case 0b110: {                 /* 32-bit store */
               width = 4;
               VSEG_DIS_NF_CASES(GETC_VSSEGLDST, vssseg, w);
               return True;
            }
            case 0b111: {                 /* SEW store */
               width = extract_sew(guest_VFLAG);
               VSEG_DIS_NF_CASES(GETC_VSSEGLDST, vssseg, e);
               return True;
            }
            default:
               return False;
         }
      }
      return False;
   }

   /* indexed */
   if (RVV0p7_is_indexed(insn, isLD, nf)) {
      UInt vs2 = GET_RS2(); /* index register */
      /* indexed normal load */
      if (RVV0p7_is_normal_load(nf, isLD)) {
         switch (GET_FUNCT3()) {
            case 0b000: {                 /* 8-bit load */
               width = 1;
               if (GET_MOP() == 0b000) {  /* zero-extended */
                  GETC_VXLDST(vlxbu);
               } else {                   /* signed-extended */
                  GETC_VXLDST(vlxb);
               }
               return True;
            }
            case 0b101: {                 /* 16-bit load */
               width = 2;
               if (GET_MOP() == 0b000) {  /* zero-extended */
                  GETC_VXLDST(vlxhu);
               } else {                   /* signed-extended */
                  GETC_VXLDST(vlxh);
               }
               return True;
            }
            case 0b110: {                 /* 32-bit load */
               width = 4;
               if (GET_MOP() == 0b000) {  /* zero-extended */
                  GETC_VXLDST(vlxwu);
               } else {                   /* signed-extended */
                  GETC_VXLDST(vlxw);
               }
               return True;
            }
            case 0b111: {                 /* SEW load */
               width = extract_sew(guest_VFLAG);
               GETC_VXLDST(vlxe);
               return True;
            }
         }
      }

      if (RVV0p7_is_seg_load(nf, isLD)) { /* indexed segment load */
         switch (GET_FUNCT3()) {
            case 0b000: {                 /* 8-bit load */
               width = 1;
               if (GET_MOP() == 0b000) {  /* zero-extended */
                  VSEG_DIS_NF_CASES(GETC_VXSEGLDST, vlxseg, bu);
               } else {                   /* signed-extended */
                  VSEG_DIS_NF_CASES(GETC_VXSEGLDST, vlxseg, b);
               }
               return True;
            }
            case 0b101: {                 /* 16-bit load */
               width = 2;
               if (GET_MOP() == 0b000) {  /* zero-extended */
                  VSEG_DIS_NF_CASES(GETC_VXSEGLDST, vlxseg, hu);
               } else {                   /* signed-extended */
                  VSEG_DIS_NF_CASES(GETC_VXSEGLDST, vlxseg, h);
               }
               return True;
            }
            case 0b110: {                 /* 32-bit load */
               width = 4;
               if (GET_MOP() == 0b000) {  /* zero-extended */
                  VSEG_DIS_NF_CASES(GETC_VXSEGLDST, vlxseg, wu);
               } else {                   /* signed-extended */
                  VSEG_DIS_NF_CASES(GETC_VXSEGLDST, vlxseg, w);
               }
               return True;
            }
            case 0b111: {                 /* SEW load */
               width = extract_sew(guest_VFLAG);
               VSEG_DIS_NF_CASES(GETC_VXSEGLDST, vlxseg, e);
               return True;
            }
            default:
               return False;
         }
      }

      /* indexed normal store */
      if (RVV0p7_is_normal_store(nf, isLD)) {
         switch (GET_FUNCT3()) {
            case 0b000: {                 /* 8-bit store */
               width = 1;
               if (GET_MOP() == 0b011) {
                  GETC_VXLDST(vsxb);
               } else {
                  GETC_VXLDST(vsuxb);
               }
               return True;
            }
            case 0b101: {                 /* 16-bit store */
               width = 2;
               if (GET_MOP() == 0b011) {
                  GETC_VXLDST(vsxh);
               } else {
                  GETC_VXLDST(vsuxh);
               }
               return True;
            }
            case 0b110: {                 /* 32-bit store */
               width = 4;
               if (GET_MOP() == 0b011) {
                  GETC_VXLDST(vsxw);
               } else {
                  GETC_VXLDST(vsuxw);
               }
               return True;
            }
            case 0b111: {                 /* SEW store */
               width = extract_sew(guest_VFLAG);
               if (GET_MOP() == 0b011) {
                  GETC_VXLDST(vsxe);
               } else {
                  GETC_VXLDST(vsuxe);
               }
               return True;
            }
            default:
               return False;
         }
      }

      if (RVV0p7_is_seg_store(nf, isLD)) { /* indexed segment store */
         switch (GET_FUNCT3()) {
            case 0b000: {                 /* 8-bit store */
               width = 1;
               VSEG_DIS_NF_CASES(GETC_VXSEGLDST, vsxseg, b);
               return True;
            }
            case 0b101: {                 /* 16-bit store */
               width = 2;
               VSEG_DIS_NF_CASES(GETC_VXSEGLDST, vsxseg, h);
               return True;
            }
            case 0b110: {                 /* 32-bit store */
               width = 4;
               VSEG_DIS_NF_CASES(GETC_VXSEGLDST, vsxseg, w);
               return True;
            }
            case 0b111: {                 /* SEW store */
               width = extract_sew(guest_VFLAG);
               VSEG_DIS_NF_CASES(GETC_VXSEGLDST, vsxseg, e);
               return True;
            }
            default:
               return False;
         }
      }
      return False;
   }
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
      /*
       * Vector Single-Width Integer Add and Subtract
       */
      case 0b000000:
         GETC_VBinopOPI(vadd);
         return True;
      case 0b000010:
         GETC_VBinopOPI_VX(vsub);
         return True;
      case 0b000011:
         GETC_VBinopOPI_XI(vrsub);
         return True;
      /*
       * Vector Integer Add-with-Carry / Subtract-with-Borrow Instructions
       */
      case 0b010000:
         GETC_VBinopOPI_VAR(vadc, GETV_VopMask);
         return True;
      case 0b010001:
         GETC_VBinopOPI_VAR(vmadc, GETV_VopMask);
         return True;
      case 0b010010:
         GETC_VBinopOPI_VX_VAR(vsbc, GETV_VopMask);
         return True;
      case 0b010011:
         GETC_VBinopOPI_VX_VAR(vmsbc, GETV_VopMask);
         return True;
      /*
       * Vector Bitwise Logical Instructions
       */
      case 0b001001:
         GETC_VBinopOPI(vand);
         return True;
      case 0b001010:
         GETC_VBinopOPI(vor);
         return True;
      case 0b001011:
         GETC_VBinopOPI(vxor);
         return True;
      /*
       * Vector Single-Width Bit Shift Instructions
       */
      case 0b100101:
         GETC_VBinopOPI(vsll);
         return True;
      case 0b101000:
         GETC_VBinopOPI(vsrl);
         return True;
      case 0b101001:
         GETC_VBinopOPI(vsra);
         return True;
      /*
       * Vector Narrowing Integer Right Shift Instructions
       */
      case 0b101100:
         GETC_VBinopOPI_VAR(vnsrl, GETV_VopWidenS2);
         return True;
      case 0b101101:
         GETC_VBinopOPI_VAR(vnsra, GETV_VopWidenS2);
         return True;
      /*
       * Vector Integer Comparison Instructions
       */
      case 0b011000:
         GETC_VBinopOPI_VAR(vmseq, GETV_VopM1D);
         return True;
      case 0b011001:
         GETC_VBinopOPI_VAR(vmsne, GETV_VopM1D);
         return True;
      case 0b011010:
         GETC_VBinopOPI_VX_VAR(vmsltu, GETV_VopM1D);
         return True;
      case 0b011011:
         GETC_VBinopOPI_VX_VAR(vmslt, GETV_VopM1D);
         return True;
      case 0b011100:
         GETC_VBinopOPI_VAR(vmsleu, GETV_VopM1D);
         return True;
      case 0b011101:
         GETC_VBinopOPI_VAR(vmsle, GETV_VopM1D);
         return True;
      case 0b011110:
         GETC_VBinopOPI_XI_VAR(vmsgtu, GETV_VopM1D);
         return True;
      case 0b011111:
         GETC_VBinopOPI_XI_VAR(vmsgt, GETV_VopM1D);
         return True;
      /*
       * Vector Integer Min/Max Instructions
       */
      case 0b000100:
         GETC_VBinopOPI_VX(vminu);
         return True;
      case 0b000101:
         GETC_VBinopOPI_VX(vmin);
         return True;
      case 0b000110:
         GETC_VBinopOPI_VX(vmaxu);
         return True;
      case 0b000111:
         GETC_VBinopOPI_VX(vmax);
         return True;
      /*
       * Vector Integer Merge and Move Instructions
       */
      case 0b010111:
         if (!mask)
            GETC_VBinopOPI(vmerge);
         else {
            rs2 = rs1;
            GETC_VUnopOPI(vmv);
         }
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
       * Vector Widening Integer Add/Subtract
       */
      case 0b110000:
         GETC_VBinopOPI_VX_VAR(vwaddu, GETV_VopWidenD);
         return True;
      case 0b110010:
         GETC_VBinopOPI_VX_VAR(vwsubu, GETV_VopWidenD);
         return True;
      case 0b110001:
         GETC_VBinopOPI_VX_VAR(vwadd, GETV_VopWidenD);
         return True;
      case 0b110011:
         GETC_VBinopOPI_VX_VAR(vwsub, GETV_VopWidenD);
         return True;
      case 0b110100:
         GETC_VBinopOPI_VX_VAR(vwadduw, GETV_VopWidenD | GETV_VopWidenS2);
         return True;
      case 0b110110:
         GETC_VBinopOPI_VX_VAR(vwsubuw, GETV_VopWidenD | GETV_VopWidenS2);
         return True;
      case 0b110101:
         GETC_VBinopOPI_VX_VAR(vwaddw, GETV_VopWidenD | GETV_VopWidenS2);
         return True;
      case 0b110111:
         GETC_VBinopOPI_VX_VAR(vwsubw, GETV_VopWidenD | GETV_VopWidenS2);
         return True;
      /*
       * Vector Single-Width Integer Multiply Instructions
       */
      case 0b100101:
         GETC_VBinopOPI_VX(vmul);
         return True;
      case 0b100111:
         GETC_VBinopOPI_VX(vmulh);
         return True;
      case 0b100100:
         GETC_VBinopOPI_VX(vmulhu);
         return True;
      case 0b100110:
         GETC_VBinopOPI_VX(vmulhsu);
         return True;
      /*
       * Vector Integer Divide Instructions
       */
      case 0b100000:
         GETC_VBinopOPI_VX(vdivu);
         return True;
      case 0b100001:
         GETC_VBinopOPI_VX(vdiv);
         return True;
      case 0b100010:
         GETC_VBinopOPI_VX(vremu);
         return True;
      case 0b100011:
         GETC_VBinopOPI_VX(vrem);
         return True;
      /*
       * Vector Widening Integer Multiply Instructions
       */
      case 0b111011:
         GETC_VBinopOPI_VX_VAR(vwmul, GETV_VopWidenD);
         return True;
      case 0b111000:
         GETC_VBinopOPI_VX_VAR(vwmulu, GETV_VopWidenD);
         return True;
      case 0b111010:
         GETC_VBinopOPI_VX_VAR(vwmulsu, GETV_VopWidenD);
         return True;
      /*
       * Vector Single-Width Integer Multiply-Add Instructions
       */
      case 0b101101:
         GETC_VBinopOPI_VX_VAR(vmacc, GETV_VopAccD);
         return True;
      case 0b101111:
         GETC_VBinopOPI_VX_VAR(vnmsac, GETV_VopAccD);
         return True;
      case 0b101001:
         GETC_VBinopOPI_VX_VAR(vmadd, GETV_VopAccD);
         return True;
      case 0b101011:
         GETC_VBinopOPI_VX_VAR(vnmsub, GETV_VopAccD);
         return True;
      /*
       * Vector Widening Integer Multiply-Add Instructions
       */
      case 0b111100:
         GETC_VBinopOPI_VX_VAR(vwmaccu, GETV_VopAccD | GETV_VopWidenD);
         return True;
      case 0b111101:
         GETC_VBinopOPI_VX_VAR(vwmacc, GETV_VopAccD | GETV_VopWidenD);
         return True;
      case 0b111110:
         GETC_VBinopOPI_VX_VAR(vwmaccsu, GETV_VopAccD | GETV_VopWidenD);
         return True;
      case 0b111111:
         GETC_VBinopOPI_X_VAR(vwmaccus, GETV_VopAccD | GETV_VopWidenD);
         return True;
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

         UInt sew = extract_sew(guest_VFLAG);

         if (rd != 0)
            putIReg64(irsb, rd,
                      sew == 1    ? unop(Iop_8Uto64,  mkexpr(ret))
                      : sew == 2 ? unop(Iop_16Uto64, mkexpr(ret))
                      : sew == 4 ? unop(Iop_32Uto64, mkexpr(ret))
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
      case 0b010100:
      /*
       * vmfirst find-first-set mask bit
       */
      case 0b010101: {
         IRTemp dret = newTemp(irsb, Ity_I64);
         if (GET_FUNCT6() == 0b010100) {
            fName = mask ? GETN_VUnopV(vmpopc)  : GETN_VUnopV_M(vmpopc);
            fAddr = mask ? GETA_VUnopV(vmpopc)  : GETA_VUnopV_M(vmpopc);
         } else {
            fName = mask ? GETN_VUnopV(vmfirst) : GETN_VUnopV_M(vmfirst);
            fAddr = mask ? GETA_VUnopV(vmfirst) : GETA_VUnopV_M(vmfirst);
         }

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

         UInt sew = extract_sew(guest_VFLAG);
         putFReg64(irsb, rd,
                   sew == 1    ? binop(Iop_Or64, mkexpr(dret), mkU64(~0xFFUL))
                   : sew == 2 ? binop(Iop_Or64, mkexpr(dret), mkU64(~0xFFFFUL))
                   : sew == 4 ? binop(Iop_Or64, mkexpr(dret), mkU64(~0xFFFFFFFFUL))
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
                         UInt                  insn,
                         Addr                  guest_pc_curr_instr)
{
   Bool ok = False;

   switch (GET_OPCODE()) {
      case OPC_OP_V:
         if (GET_FUNCT3() == RV64_SOPC_OPCFG)
            return dis_RV64V0p7_cfg(dres, irsb, insn, guest_pc_curr_instr);
         ok = dis_RV64V0p7_arith(dres, irsb, insn);
         break;
      case OPC_LOAD_FP:
      case OPC_STORE_FP:
         ok = dis_RV64V0p7_ldst(dres, irsb, insn);
         break;
      case OPC_SYSTEM:
         ok = dis_RV64V0p7_csr(dres, irsb, insn);
         break;
      default:
         break;
   }

   /* Reset vstart if necessary and stop the translation if reset occurs. */
   if (extract_vstart(guest_VFLAG) && GET_OPCODE() != OPC_SYSTEM) {
      putVStart(irsb, mkU64(0));
      /* Reset occurs, terminate translation. As cfg instructions will
         directly return, there is no need to handle it. */
      dres->whatNext    = Dis_StopHere;
      dres->jk_StopHere = Ijk_ExitBB;
   }

   return ok;
}

/*--------------------------------------------------------------------*/
/*--- end                                  guest_riscv64V0p7_toIR.c --*/
/*--------------------------------------------------------------------*/
