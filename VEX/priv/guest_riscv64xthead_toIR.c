
/*--------------------------------------------------------------------*/
/*--- begin                             guest_riscv64xthead_toIR.c ---*/
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

/* Translates T-HEAD code to IR. */

#define XTHEAD_GET_FUNCT7() INSN(31, 27)
#define XTHEAD_OPC_MEM_LOAD   0b100
#define XTHEAD_OPC_MEM_STORE  0b101
#define XTHEAD_OPC_MEM_FLOAD  0b110
#define XTHEAD_OPC_MEM_FSTORE 0b111
#define XTHEAD_OPC_ARITH      0b001
#define XTHEAD_OPC_ARITH_EXT  0b010
#define XTHEAD_OPC_ARITH_EXTU 0b011

#define XTHEAD_SOPC_ADDSL     0b00000
#define XTHEAD_SOPC_SRRI      0b000100
#define XTHEAD_SOPC_SRRIW     0b0001010
#define XTHEAD_SOPC_FF0       0b1000010
#define XTHEAD_SOPC_FF1       0b1000011
#define XTHEAD_SOPC_REV       0b1000001
#define XTHEAD_SOPC_REVW      0b1001000
#define XTHEAD_SOPC_TSTNBZ    0b1000000
#define XTHEAD_SOPC_TST       0b100010
#define XTHEAD_SOPC_MVEQZ     0b0100000
#define XTHEAD_SOPC_MVNEZ     0b0100001

static void gen_xthead_store(IRSB* irsb, UInt szB, IRTemp addr, IRExpr* dataE)
{
   IRExpr* addrE = mkexpr(addr);
   switch (szB) {
   case 8:
      storeLE(irsb, addrE, dataE);
      break;
   case 4:
      storeLE(irsb, addrE, unop(Iop_64to32, dataE));
      break;
   case 2:
      storeLE(irsb, addrE, unop(Iop_64to16, dataE));
      break;
   case 1:
      storeLE(irsb, addrE, unop(Iop_64to8, dataE));
      break;
   default:
      vassert(0);
   }
}

static IRTemp gen_xthead_load(IRSB* irsb, UInt szB, IRTemp addr, Bool zextend)
{
   IRTemp  res   = newTemp(irsb, Ity_I64);
   IRExpr* addrE = mkexpr(addr);
   switch (szB) {
      case 8:
         assign(irsb, res, loadLE(Ity_I64,addrE));
         break;
      case 4:
         assign(irsb, res, unop(zextend ? Iop_32Uto64 : Iop_32Sto64,
                           loadLE(Ity_I32, addrE)));
         break;
      case 2:
         assign(irsb, res, unop(zextend ? Iop_16Uto64 : Iop_16Sto64,
                           loadLE(Ity_I16, addrE)));
         break;
      case 1:
         assign(irsb, res, unop(zextend ? Iop_8Uto64 : Iop_8Sto64,
                           loadLE(Ity_I8, addrE)));
         break;
      default:
         vassert(0);
   }
   return res;
}

static Bool dis_XTHEAD_load_store(/*MB_OUT*/ DisResult* dres,
                                  /*OUT*/ IRSB*         irsb,
                                  UInt                  insn,
                                  Bool                  sigill_diag)
{
   Bool isLD = GET_FUNCT3() == XTHEAD_OPC_MEM_LOAD;

   /*
   #define XTHEAD_SOPC_LDD    0b11111
   #define XTHEAD_SOPC_LWD    0b11100
   #define XTHEAD_SOPC_LWUD   0b11110
   */
   if (INSN(31,29) == 0b111) {
      UInt rd1 = GET_RD();
      UInt rs1 = GET_RS1();
      UInt rd2 = GET_RS2();
      UInt imm2 = INSN(26,25);
      Bool isLDD = INSN(27,27) == 1;
      Bool isZextend = (INSN(28,28) == 1) && !isLDD;

      UInt offs = isLDD ? imm2 << 4 : imm2 << 3;
      IRTemp ta1 = newTemp(irsb, Ity_I64);
      IRTemp ta2 = newTemp(irsb, Ity_I64);
      assign(irsb, ta1, binop(Iop_Add64, getIReg64(rs1), mkU64(offs)));
      assign(irsb, ta2, binop(Iop_Add64, getIReg64(rs1), mkU64(offs + (isLDD ? 8 : 4))));

      if (isLD) {
         putIReg64(irsb, rd1, mkexpr(gen_xthead_load(irsb, isLDD ? 8 : 4, ta1, isZextend)));
         putIReg64(irsb, rd2, mkexpr(gen_xthead_load(irsb, isLDD ? 8 : 4, ta2, isZextend)));
      } else {
         gen_xthead_store(irsb, isLDD ? 8 : 4, ta1, getIReg64(rd1));
         gen_xthead_store(irsb, isLDD ? 8 : 4, ta2, getIReg64(rd2));
      }

      const HChar* ld_insn[2] = {
         "lw%sd %s,%s,(%s),%u,3\n", "ld%sd %s,%s,(%s),%u,4\n",
      };
      const HChar* st_insn[2] = {
         "sw%sd %s,%s,(%s),%u,3\n", "sd%sd %s,%s,(%s),%u,4\n",
      };
      DIP((isLD ? ld_insn : st_insn)[isLDD], isZextend ? "u" : "",
          nameIReg(rd1), nameIReg(rd2), nameIReg(rs1), imm2);
      return True;
   }

   /*
   #define XTHEAD_SOPC_LRB    0b00000
   #define XTHEAD_SOPC_LRBU   0b10000
   #define XTHEAD_SOPC_LRH    0b00100
   #define XTHEAD_SOPC_LRHU   0b10100
   #define XTHEAD_SOPC_LRW    0b01000
   #define XTHEAD_SOPC_LRWU   0b11000
   #define XTHEAD_SOPC_LRD    0b01100

   #define XTHEAD_SOPC_LURB   0b00010
   #define XTHEAD_SOPC_LURBU  0b10010
   #define XTHEAD_SOPC_LURH   0b00110
   #define XTHEAD_SOPC_LURHU  0b10110
   #define XTHEAD_SOPC_LURW   0b01010
   #define XTHEAD_SOPC_LURWU  0b11010
   #define XTHEAD_SOPC_LURD   0b01110
   */
   if (INSN(28,27) == 0b00 || INSN(28,27) == 0b10) {
      UInt rd  = GET_RD();
      UInt rs1 = GET_RS1();
      UInt rs2 = GET_RS2();
      UInt imm2 = INSN(26,25);
      UInt szLg2 = INSN(30,29);
      Bool isZextend = INSN(31,31) == 1;
      IRExpr* isZindex =
         (INSN(28, 27) == 0b10) ? unop(Iop_32Uto64, getIReg32(rs2)) : NULL;

      IRTemp  ta  = newTemp(irsb, Ity_I64);
      IRExpr* offs = binop(Iop_Shl64, isZindex ? isZindex : getIReg64(rs2), mkU8(imm2));
      assign(irsb, ta, binop(Iop_Add64, getIReg64(rs1), offs));

      if (isLD) {
         putIReg64(irsb, rd, mkexpr(gen_xthead_load(irsb, 1 << szLg2, ta, isZextend)));
      } else {
         gen_xthead_store(irsb, 1 << szLg2, ta, getIReg64(rd));
      }

      const HChar* ld_insn[4] = {
         "l%srb%s %s,%s,%s,%u\n", "l%srh%s %s,%s,%s,%u\n",
         "l%srw%s %s,%s,%s,%u\n", "l%srd%s %s,%s,%s,%u\n"
      };
      const HChar* st_insn[4] = {
         "s%srb%s %s,%s,%s,%u\n", "s%srh%s %s,%s,%s,%u\n",
         "s%srw%s %s,%s,%s,%u\n", "s%srd%s %s,%s,%s,%u\n"
      };
      DIP((isLD ? ld_insn : st_insn)[szLg2], isZindex ? "u" : "", isZextend ? "u" : "",
          nameIReg(rd), nameIReg(rs1), nameIReg(rs2), imm2);
      return True;
   }

   /*
   #define XTHEAD_SOPC_LBIA   0b00011
   #define XTHEAD_SOPC_LBIB   0b00001
   #define XTHEAD_SOPC_LBUIA  0b10011
   #define XTHEAD_SOPC_LBUIB  0b10001
   #define XTHEAD_SOPC_LHIA   0b00111
   #define XTHEAD_SOPC_LHIB   0b00101
   #define XTHEAD_SOPC_LHUIA  0b10111
   #define XTHEAD_SOPC_LHUIB  0b10101
   #define XTHEAD_SOPC_LWIA   0b01011
   #define XTHEAD_SOPC_LWIB   0b01001
   #define XTHEAD_SOPC_LWUIA  0b11011
   #define XTHEAD_SOPC_LWUIB  0b11001
   #define XTHEAD_SOPC_LDIA   0b01111
   #define XTHEAD_SOPC_LDIB   0b01101
   */
   if (INSN(28,27) == 0b11 || INSN(28,27) == 0b01) {
      UInt rd        = GET_RD();
      UInt rs1       = GET_RS1();
      UInt imm5      = GET_RS2();
      UInt imm2      = INSN(26,25);
      UInt szLg2     = INSN(30,29);
      Bool isZextend = INSN(31,31) == 1;
      Bool isAfter   = INSN(28,28) == 1;

      ULong offs = vex_sx_to_64(imm5, 5) << imm2;
      if (!isAfter)
         putIReg64(irsb, rs1, binop(Iop_Add64, getIReg64(rs1), mkU64(offs)));
      switch (szLg2) {
         case 0:
            if (isLD) {
               putIReg64(irsb, rd, unop(isZextend ? Iop_8Uto64 : Iop_8Sto64,
                                        loadLE(Ity_I8, getIReg64(rs1))));
            } else {
               storeLE(irsb, getIReg64(rs1), unop(Iop_64to8, getIReg64(rd)));
            }
            break;
         case 1:
            if (isLD) {
               putIReg64(irsb, rd, unop(isZextend ? Iop_16Uto64 : Iop_16Sto64,
                                        loadLE(Ity_I16, getIReg64(rs1))));
            } else {
               storeLE(irsb, getIReg64(rs1), unop(Iop_64to16, getIReg64(rd)));
            }
            break;
         case 2:
            if (isLD) {
               putIReg64(irsb, rd, unop(isZextend ? Iop_32Uto64 : Iop_32Sto64,
                                        loadLE(Ity_I32, getIReg64(rs1))));
            } else {
               storeLE(irsb, getIReg64(rs1), unop(Iop_64to32, getIReg64(rd)));
            }
            break;
         case 3:
            if (isLD) {
               putIReg64(irsb, rd, loadLE(Ity_I64, getIReg64(rs1)));
            } else {
               storeLE(irsb, getIReg64(rs1), getIReg64(rd));
            }
            break;
         default:
            vassert(0);
      }
      if (isAfter)
         putIReg64(irsb, rs1, binop(Iop_Add64, getIReg64(rs1), mkU64(offs)));

      const HChar* ld_insn[4] = {
         "lb%si%s %s,(%s),%u,%u\n", "lh%si%s %s,(%s),%u,%u\n",
         "lw%si%s %s,(%s),%u,%u\n", "ld%si%s %s,(%s),%u,%u\n"
      };
      const HChar* st_insn[4] = {
         "sb%si%s %s,(%s),%u,%u\n", "sh%si%s %s,(%s),%u,%u\n",
         "sw%si%s %s,(%s),%u,%u\n", "sd%si%s %s,(%s),%u,%u\n"
      };
      DIP((isLD ? ld_insn : st_insn)[szLg2], isZextend ? "u" : "", isAfter ? "a" : "b",
          nameIReg(rd), nameIReg(rs1), imm5, imm2);
      return True;
   }

   if (sigill_diag)
      vex_printf("XTHEAD front end: load_store\n");

   return False;
}

static Bool dis_XTHEAD_fload_fstore(/*MB_OUT*/ DisResult* dres,
                                    /*OUT*/ IRSB*         irsb,
                                    UInt                  insn,
                                    Bool                  sigill_diag)
{
   /*
   #define XTHEAD_SOPC_FLRD    0b01100
   #define XTHEAD_SOPC_FLRW    0b01000
   #define XTHEAD_SOPC_FLURD   0b01110
   #define XTHEAD_SOPC_FLURW   0b01010
   */
   vassert(INSN(31,30) == 0b01);
   vassert(INSN(27,27) == 0b00);

   Bool isLD = GET_FUNCT3() == XTHEAD_OPC_MEM_FLOAD;

   UInt rd  = GET_RD();
   UInt rs1 = GET_RS1();
   UInt rs2 = GET_RS2();
   UInt imm2 = INSN(26,25);
   UInt isDouble = INSN(29,29) == 1;
   IRExpr* isZindex =
      (INSN(28,28) == 1) ? unop(Iop_32Uto64, getIReg32(rs2)) : NULL;

   IRTemp  ta  = newTemp(irsb, Ity_I64);
   IRExpr* offs = binop(Iop_Shl64, isZindex ? isZindex : getIReg64(rs2), mkU8(imm2));
   assign(irsb, ta, binop(Iop_Add64, getIReg64(rs1), offs));

   if (isLD) {
      if (isDouble)
         putFReg64(irsb, rd, loadLE(Ity_F64, mkexpr(ta)));
      else
         putFReg32(irsb, rd, loadLE(Ity_F32, mkexpr(ta)));
   } else {
      storeLE(irsb, mkexpr(ta), isDouble ? getFReg64(rd) : getFReg32(rd));
   }

   const HChar* ld_insn[2] = {
      "fl%srw %s,%s,%s,%u\n", "fl%srd %s,%s,%s,%u\n"
   };
   const HChar* st_insn[2] = {
      "fs%srw %s,%s,%s,%u\n", "fs%srd %s,%s,%s,%u\n"
   };
   DIP((isLD ? ld_insn : st_insn)[isDouble], isZindex ? "u" : "",
       nameFReg(rd), nameIReg(rs1), nameIReg(rs2), imm2);
   return True;
}

static Bool dis_XTHEAD_arithmetic(/*MB_OUT*/ DisResult* dres,
                                  /*OUT*/ IRSB*         irsb,
                                  UInt                  insn,
                                  Bool                  sigill_diag)
{
   if (GET_FUNCT3() == XTHEAD_OPC_ARITH && GET_FUNCT5() == XTHEAD_SOPC_ADDSL) {
      UInt rd   = GET_RD();
      UInt rs1  = GET_RS1();
      UInt rs2  = GET_RS2();
      UInt imm2 = INSN(26, 25);
      IRExpr* offs = binop(Iop_Shl64, getIReg64(rs2), mkU8(imm2));
      putIReg64(irsb, rd, binop(Iop_Add64, getIReg64(rs1), offs));
      DIP("addsl %s,%s,%s,%u\n", nameIReg(rd), nameIReg(rs1), nameIReg(rs2), imm2);
      return True;
   }

   if (GET_FUNCT3() == XTHEAD_OPC_ARITH && (GET_FUNCT7() == XTHEAD_SOPC_SRRIW ||
                                            INSN(31, 26) == XTHEAD_SOPC_SRRI)) {
      UInt rd   = GET_RD();
      UInt rs1  = GET_RS1();
      UInt imm  = 0;
      UInt immL = 0;
      IRExpr* eR1 = NULL;

      if (GET_FUNCT7() == XTHEAD_SOPC_SRRIW) {
         imm  = GET_RS2();
         immL = 32 - GET_RS2();
         eR1 = unop(Iop_32Uto64, getIReg32(rs1));
      } else {
         imm  = INSN(25, 20);
         immL = 64 - INSN(25, 20);
         eR1 = getIReg64(rs1);
      }
      putIReg64(irsb, rd,
                binop(Iop_Or64, binop(Iop_Shr64, eR1, mkU8(imm)),
                                binop(Iop_Shl64, eR1, mkU8(immL))));
      DIP("%s %s,%s,%u\n", GET_FUNCT7() == XTHEAD_SOPC_SRRIW ? "srriw" : "srri",
          nameIReg(rd), nameIReg(rs1), imm);
      return True;
   }

   if (GET_FUNCT3() == XTHEAD_OPC_ARITH && GET_RS2() == 0 &&
       (GET_FUNCT7() == XTHEAD_SOPC_FF0 || GET_FUNCT7() == XTHEAD_SOPC_FF1)) {
      ;
   }

   if (GET_FUNCT3() == XTHEAD_OPC_ARITH && GET_RS2() == 0 &&
       (GET_FUNCT7() == XTHEAD_SOPC_REV || GET_FUNCT7() == XTHEAD_SOPC_REVW)) {
      UInt rd   = GET_RD();
      UInt rs1  = GET_RS1();
      IRExpr* eSUM = NULL;
      if (GET_FUNCT7() == XTHEAD_SOPC_REV) {
         IRExpr* eR1 = getIReg64(rs1);
         IRExpr* eB0 = binop(Iop_Shr64, binop(Iop_And64, eR1, mkU64(0xFFUL << 56)), mkU8(56));
         IRExpr* eB1 = binop(Iop_Shr64, binop(Iop_And64, eR1, mkU64(0xFFUL << 48)), mkU8(40));
         IRExpr* eB2 = binop(Iop_Shr64, binop(Iop_And64, eR1, mkU64(0xFFUL << 40)), mkU8(24));
         IRExpr* eB3 = binop(Iop_Shr64, binop(Iop_And64, eR1, mkU64(0xFFUL << 32)), mkU8(8));
         IRExpr* eB4 = binop(Iop_Shl64, binop(Iop_And64, eR1, mkU64(0xFFUL << 24)), mkU8(8));
         IRExpr* eB5 = binop(Iop_Shl64, binop(Iop_And64, eR1, mkU64(0xFFUL << 16)), mkU8(24));
         IRExpr* eB6 = binop(Iop_Shl64, binop(Iop_And64, eR1, mkU64(0xFFUL << 8)),  mkU8(40));
         IRExpr* eB7 = binop(Iop_Shl64, binop(Iop_And64, eR1, mkU64(0xFFUL << 0)),  mkU8(56));
         eSUM = binop(Iop_Or64, binop(Iop_Or64, binop(Iop_Or64, eB0, eB1),
                                                binop(Iop_Or64, eB2, eB3)),
                                binop(Iop_Or64, binop(Iop_Or64, eB4, eB5),
                                                binop(Iop_Or64, eB6, eB7)));
         putIReg64(irsb, rd, eSUM);
      } else {
         IRExpr* eR1 = getIReg32(rs1);
         IRExpr* eB0 = binop(Iop_Shr32, binop(Iop_And32, eR1, mkU32(0xFF << 24)), mkU8(24));
         IRExpr* eB1 = binop(Iop_Shr32, binop(Iop_And32, eR1, mkU32(0xFF << 16)), mkU8(8));
         IRExpr* eB2 = binop(Iop_Shl32, binop(Iop_And32, eR1, mkU32(0xFF << 8)),  mkU8(8));
         IRExpr* eB3 = binop(Iop_Shl32, binop(Iop_And32, eR1, mkU32(0xFF << 0)),  mkU8(24));
         eSUM = binop(Iop_Or32, binop(Iop_Or32, eB0, eB1),
                                binop(Iop_Or32, eB2, eB3));
         putIReg32(irsb, rd, eSUM);
      }

      DIP("%s %s,%s\n", GET_FUNCT7() == XTHEAD_SOPC_REV ? "rev" : "revw",
          nameIReg(rd), nameIReg(rs1));
      return True;
   }

   if (GET_FUNCT3() == XTHEAD_OPC_ARITH && GET_RS2() == 0 &&
       GET_FUNCT7() == XTHEAD_SOPC_TSTNBZ) {
      UInt rd  = GET_RD();
      UInt rs1 = GET_RS1();
      IRExpr* eRD = getIReg64(rd);
      IRExpr* eR1 = getIReg64(rs1);
      IRExpr* eCMP0 = binop(Iop_CmpEQ64, binop(Iop_And64, eR1, mkU64(0xFFUL << 0)),  mkU64(0));
      IRExpr* eCMP1 = binop(Iop_CmpEQ64, binop(Iop_And64, eR1, mkU64(0xFFUL << 8)),  mkU64(0));
      IRExpr* eCMP2 = binop(Iop_CmpEQ64, binop(Iop_And64, eR1, mkU64(0xFFUL << 16)), mkU64(0));
      IRExpr* eCMP3 = binop(Iop_CmpEQ64, binop(Iop_And64, eR1, mkU64(0xFFUL << 24)), mkU64(0));
      IRExpr* eCMP4 = binop(Iop_CmpEQ64, binop(Iop_And64, eR1, mkU64(0xFFUL << 32)), mkU64(0));
      IRExpr* eCMP5 = binop(Iop_CmpEQ64, binop(Iop_And64, eR1, mkU64(0xFFUL << 40)), mkU64(0));
      IRExpr* eCMP6 = binop(Iop_CmpEQ64, binop(Iop_And64, eR1, mkU64(0xFFUL << 48)), mkU64(0));
      IRExpr* eCMP7 = binop(Iop_CmpEQ64, binop(Iop_And64, eR1, mkU64(0xFFUL << 56)), mkU64(0));
      putIReg64(irsb, rd, mkU64(0));
      putIReg64(irsb, rd, binop(Iop_Or64, eRD, IRExpr_ITE(eCMP0, mkU64(0xFFUL << 0),  mkU64(0))));
      putIReg64(irsb, rd, binop(Iop_Or64, eRD, IRExpr_ITE(eCMP1, mkU64(0xFFUL << 8),  mkU64(0))));
      putIReg64(irsb, rd, binop(Iop_Or64, eRD, IRExpr_ITE(eCMP2, mkU64(0xFFUL << 16), mkU64(0))));
      putIReg64(irsb, rd, binop(Iop_Or64, eRD, IRExpr_ITE(eCMP3, mkU64(0xFFUL << 24), mkU64(0))));
      putIReg64(irsb, rd, binop(Iop_Or64, eRD, IRExpr_ITE(eCMP4, mkU64(0xFFUL << 32), mkU64(0))));
      putIReg64(irsb, rd, binop(Iop_Or64, eRD, IRExpr_ITE(eCMP5, mkU64(0xFFUL << 40), mkU64(0))));
      putIReg64(irsb, rd, binop(Iop_Or64, eRD, IRExpr_ITE(eCMP6, mkU64(0xFFUL << 48), mkU64(0))));
      putIReg64(irsb, rd, binop(Iop_Or64, eRD, IRExpr_ITE(eCMP7, mkU64(0xFFUL << 56), mkU64(0))));
      DIP("tstnbz %s,%s\n", nameIReg(rd), nameIReg(rs1));
      return True;
   }

   if (GET_FUNCT3() == XTHEAD_OPC_ARITH && INSN(31, 26) == XTHEAD_SOPC_TST) {
      UInt rd  = GET_RD();
      UInt rs1 = GET_RS1();
      UInt imm6 = INSN(25, 20);
      IRExpr* eMask = mkU64(1UL << imm6);
      IRExpr* eCMP = binop(Iop_CmpEQ64, binop(Iop_And64, getIReg64(rs1), eMask),  eMask);
      putIReg64(irsb, rd, IRExpr_ITE(eCMP, mkU64(1), mkU64(0)));
      DIP("tst %s,%s,%u\n", nameIReg(rd), nameIReg(rs1), imm6);
      return True;
   }

   if (GET_FUNCT3() == XTHEAD_OPC_ARITH && (GET_FUNCT7() == XTHEAD_SOPC_MVEQZ ||
                                            GET_FUNCT7() == XTHEAD_SOPC_MVNEZ)) {
      UInt rd  = GET_RD();
      UInt rs1 = GET_RS1();
      UInt rs2 = GET_RS2();
      IRExpr* eCMP = binop(GET_FUNCT7() == XTHEAD_SOPC_MVEQZ ? Iop_CmpEQ64 : Iop_CmpNE64,
                           getIReg64(rs2), mkU64(0));
      putIReg64(irsb, rd, IRExpr_ITE(eCMP, getIReg64(rs1), getIReg64(rd)));
      DIP("%s %s,%s,%s\n",
          GET_FUNCT7() == XTHEAD_SOPC_MVEQZ ? "mveqz" : "mvnez", nameIReg(rd),
          nameIReg(rs1), nameIReg(rs2));
      return True;
   }

   if (GET_FUNCT3() == XTHEAD_OPC_ARITH_EXT ||
       GET_FUNCT3() == XTHEAD_OPC_ARITH_EXTU) {
      UInt rd    = GET_RD();
      UInt rs1   = GET_RS1();
      UInt imm6H = INSN(31, 26);
      UInt imm6L = INSN(25, 20);
      UInt len = imm6H - imm6L + 1;
      putIReg64(irsb, rd,
                binop(Iop_Shl64, getIReg64(rs1), mkU8(64 - len - imm6L)));
      putIReg64(irsb, rd,
                binop(GET_FUNCT3() == XTHEAD_OPC_ARITH_EXT ? Iop_Sar64 : Iop_Shr64,
                      getIReg64(rd), mkU8(64 - len)));
      DIP("%s %s,%s,%u,%u\n",
          GET_FUNCT7() == XTHEAD_OPC_ARITH_EXT ? "ext" : "extu",
          nameIReg(rd), nameIReg(rs1), imm6H, imm6L);
      return True;
   }

   if (sigill_diag)
      vex_printf("XTHEAD front end: arithmetic\n");

   return False;
}

static Bool dis_RISCV64_xthead(/*MB_OUT*/ DisResult* dres,
                               /*OUT*/ IRSB*         irsb,
                               UInt                  insn,
                               Addr                  guest_pc_curr_instr,
                               const VexAbiInfo*     abiinfo,
                               Bool                  sigill_diag)
{
   vassert(GET_OPCODE() == OPC_CUSTOM_0);

   Bool ok = False;

   switch (GET_FUNCT3()) {
      case XTHEAD_OPC_MEM_LOAD:
      case XTHEAD_OPC_MEM_STORE:
         ok = dis_XTHEAD_load_store(dres, irsb, insn, sigill_diag);
         break;
      case XTHEAD_OPC_MEM_FLOAD:
      case XTHEAD_OPC_MEM_FSTORE:
         ok = dis_XTHEAD_fload_fstore(dres, irsb, insn, sigill_diag);
         break;
      case XTHEAD_OPC_ARITH:
      case XTHEAD_OPC_ARITH_EXT:
      case XTHEAD_OPC_ARITH_EXTU:
         ok = dis_XTHEAD_arithmetic(dres, irsb, insn, sigill_diag);
         break;
      default:
         vassert(0); /* Can't happen */
   }

   return ok;
}

/*--------------------------------------------------------------------*/
/*--- end                               guest_riscv64xthead_toIR.c ---*/
/*--------------------------------------------------------------------*/
