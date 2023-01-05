
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

#define OPC_LOAD        0b0000011
#define OPC_LOAD_FP     0b0000111
#define OPC_CUSTOM_0    0b0001011
#define OPC_MISC_MEM    0b0001111
#define OPC_OP_IMM      0b0010011
#define OPC_AUIPC       0b0010111
#define OPC_OP_IMM_32   0b0011011
#define OPC_STORE       0b0100011
#define OPC_STORE_FP    0b0100111
#define OPC_CUSTOM_1    0b0101011
#define OPC_AMO         0b0101111
#define OPC_OP          0b0110011
#define OPC_LUI         0b0110111
#define OPC_OP_32       0b0111011
#define OPC_MADD        0b1000011
#define OPC_MSUB        0b1000111
#define OPC_NMSUB       0b1001011
#define OPC_NMADD       0b1001111
#define OPC_OP_FP       0b1010011
#define OPC_OP_V        0b1010111
#define OPC_CUSTOM_2    0b1011011
#define OPC_BRANCH      0b1100011
#define OPC_JALR        0b1100111
#define OPC_JAL         0b1101111
#define OPC_SYSTEM      0b1110011
#define OPC_CUSTOM_3    0b1111011

#define GET_OPCODE()    INSN(6, 0)
#define GET_FUNCT3()    INSN(14, 12)
#define GET_FUNCT7()    INSN(31, 25)
#define GET_RD()        INSN(11, 7)
#define GET_RS1()       INSN(19, 15)
#define GET_RS2()       INSN(24, 20)

#define OPC_IS_CUSTOMS() (GET_OPCODE() == OPC_CUSTOM_0 \
                          || GET_OPCODE() == OPC_CUSTOM_1 \
                          || GET_OPCODE() == OPC_CUSTOM_2 \
                          || GET_OPCODE() == OPC_CUSTOM_3)

#define XTHEAD_GET_FUNCT7() INSN(31, 27)
#define XTHEAD_OPC_MEM_LOAD   0b100
#define XTHEAD_OPC_MEM_STORE  0b101
#define XTHEAD_OPC_MEM_FLOAD  0b110
#define XTHEAD_OPC_MEM_FSTORE 0b111

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

static Bool dis_RISCV64_xthead(/*MB_OUT*/ DisResult* dres,
                               /*OUT*/ IRSB*         irsb,
                               UInt                  insn,
                               Addr                  guest_pc_curr_instr,
                               const VexAbiInfo*     abiinfo,
                               Bool                  sigill_diag)
{
   Bool ok = False;

#ifdef __riscv_xthead
   if (GET_OPCODE() != OPC_CUSTOM_0)
      return ok;

   switch (GET_FUNCT3()) {
      case XTHEAD_OPC_MEM_LOAD:
      case XTHEAD_OPC_MEM_STORE:
         ok = dis_XTHEAD_load_store(dres, irsb, insn, sigill_diag);
         break;
      case XTHEAD_OPC_MEM_FLOAD:
      case XTHEAD_OPC_MEM_FSTORE:
         ok = dis_XTHEAD_fload_fstore(dres, irsb, insn, sigill_diag);
         break;
      default:
         vassert(0); /* Can't happen */
   }

#endif
   return ok;
}

/*--------------------------------------------------------------------*/
/*--- end                               guest_riscv64xthead_toIR.c ---*/
/*--------------------------------------------------------------------*/
