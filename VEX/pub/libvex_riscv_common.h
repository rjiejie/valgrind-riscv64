
/*--------------------------------------------------------------------*/
/*--- begin                                  libvex_riscv_common.h ---*/
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

#ifndef __LIBVEX_PUB_RISCV_H
#define __LIBVEX_PUB_RISCV_H

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
#define GET_FUNCT5()    INSN(31, 27)
#define GET_FUNCT7()    INSN(31, 25)
#define GET_RD()        INSN(11, 7)
#define GET_RS1()       INSN(19, 15)
#define GET_RS2()       INSN(24, 20)

#define OPC_IS_CUSTOMS() (GET_OPCODE() == OPC_CUSTOM_0 \
                          || GET_OPCODE() == OPC_CUSTOM_1 \
                          || GET_OPCODE() == OPC_CUSTOM_2 \
                          || GET_OPCODE() == OPC_CUSTOM_3)

#define RV64_FMT_FS 0b00
#define RV64_FMT_FD 0b01
#define RV64_FMT_FH 0b10

#define RV64_FMT_W  0b00
#define RV64_FMT_WU 0b01
#define RV64_FMT_L  0b10
#define RV64_FMT_LU 0b11

#define RV64_SOPC_FADD     0b00000
#define RV64_SOPC_FSUB     0b00001
#define RV64_SOPC_FMUL     0b00010
#define RV64_SOPC_FDIV     0b00011
#define RV64_SOPC_FSQRT    0b01011
#define RV64_SOPC_FSGNJ    0b00100
#define RV64_SOPC_FMIN_MAX 0b00101
#define RV64_SOPC_FCLASS   0b11100
#define RV64_SOPC_FMV_X_H  0b11100
#define RV64_SOPC_FMV_H_X  0b11110
#define RV64_SOPC_FCMP     0b10100
#define RV64_SOPC_FCVT_FI  0b11010   // integer-floating-point conversion
#define RV64_SOPC_FCVT_IF  0b11000   // floating-point-integer conversion
#define RV64_SOPC_FCVT_FF  0b01000   // floating-point conversion

#define RV64_SOPC_CSRRW    0b001
#define RV64_SOPC_CSRRS    0b010
#define RV64_SOPC_CSRRC    0b011

#define RV64_SOPC_OPIVV    0b000
#define RV64_SOPC_OPFVV    0b001
#define RV64_SOPC_OPMVV    0b010
#define RV64_SOPC_OPIVI    0b011
#define RV64_SOPC_OPIVX    0b100
#define RV64_SOPC_OPFVF    0b101
#define RV64_SOPC_OPMVX    0b110
#define RV64_SOPC_OPCFG    0b111

#endif /* __LIBVEX_PUB_RISCV_H */

/*--------------------------------------------------------------------*/
/*--- end                                    libvex_riscv_common.h ---*/
/*--------------------------------------------------------------------*/
