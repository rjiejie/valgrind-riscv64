/* Tests for the T-HEAD vendor extension instruction set. */

#include "testinst.h"

#define TESTINST_1_2_SETRD(length, instruction, rd_val, rs1_val, rs2_val, rd, rs1, rs2)      \
   {                                                                           \
      unsigned long w[1 /*out*/ + 3 /*in*/ + 3 /*spill*/] = {                  \
         0, (unsigned long)rd_val,(unsigned long)rs1_val,                      \
         (unsigned long)rs2_val, 0, 0, 0};                                     \
      /* w[0] = output rd value                                                \
         w[1] = input rd value                                                 \
         w[2] = input rs1 value                                                \
         w[3] = input rs2 value                                                \
         w[4] = spill slot for rd                                              \
         w[5] = spill slot for rs1                                             \
         w[6] = spill slot for rs2                                             \
       */                                                                      \
      register unsigned long* t1 asm("t1") = w;                                \
      __asm__ __volatile__(                                                    \
         "sd " #rd ", 32(%[w]);"       /* Spill rd. */                         \
         "sd " #rs1 ", 40(%[w]);"      /* Spill rs1. */                        \
         "sd " #rs2 ", 48(%[w]);"      /* Spill rs2. */                        \
         "ld " #rd ", 8(%[w]);"        /* Load the first input. */             \
         "ld " #rs1 ", 16(%[w]);"      /* Load the second input. */            \
         "ld " #rs2 ", 24(%[w]);"      /* Load the third input. */             \
         ASMINST_##length(instruction) ";"                                     \
         "sd " #rd ", 0(%[w]);"        /* Save result of the operation. */     \
         "ld " #rd ", 32(%[w]);"       /* Reload rd. */                        \
         "ld " #rs1 ", 40(%[w]);"      /* Reload rs1. */                       \
         "ld " #rs2 ", 48(%[w]);"      /* Reload rs2. */                       \
         :                                                                     \
         : [w] "r"(t1)                                                         \
         : "memory");                                                          \
      printf("%s ::\n", instruction);                                          \
      printf("  inputs: %s=0x%016lx, %s=0x%016lx, %s=0x%016lx\n", #rd,         \
             (unsigned long)rd_val, #rs1, (unsigned long)rs1_val,              \
             #rs2, (unsigned long)rs2_val);                                    \
      printf("  output: %s=0x%016lx\n", #rd, w[0]);                            \
   }

static __attribute__((noinline)) void test_integer_arithmetic(void)
{
   //printf("sync\n");
   //printf("syncs\n");
   //printf("synci\n");
   //printf("syncis\n");

   printf("Address calculation instructions (XTheadBa)\n");
   //printf("addsl rd, rs1, rs2, imm2\n");
   TESTINST_1_2(4, "addsl a0, a1, a2, 0", 0x0000000100000000, 0x0000000000001000, a0, a1, a2);
   TESTINST_1_2(4, "addsl a0, a1, a2, 1", 0x0000000100000000, 0x0000000000001000, a0, a1, a2);
   TESTINST_1_2(4, "addsl a0, a1, a2, 2", 0x0000000100000000, 0x0000000000001000, a0, a1, a2);
   TESTINST_1_2(4, "addsl a0, a1, a2, 3", 0x0000000100000000, 0x0000000000001000, a0, a1, a2);

   printf("Basic bit-manipulation (XTheadBb)\n");
   //printf("srri rd, rs1, imm6\n");
   TESTINST_1_1(4, "srri a0, a1, 3",  0x9999999966666666, a0, a1);
   TESTINST_1_1(4, "srri a0, a1, 8",  0x9999999966666666, a0, a1);
   TESTINST_1_1(4, "srri a0, a1, 15", 0x9999999966666666, a0, a1);
   TESTINST_1_1(4, "srri a0, a1, 32", 0x9999999966666666, a0, a1);

   //printf("srriw rd, rs1, imm5\n");
   TESTINST_1_1(4, "srriw a0, a1, 0",  0x6969696969696969, a0, a1);
   TESTINST_1_1(4, "srriw a0, a1, 8",  0x6969696969696969, a0, a1);
   TESTINST_1_1(4, "srriw a0, a1, 15", 0x6969696969696969, a0, a1);
   TESTINST_1_1(4, "srriw a0, a1, 30", 0x6969696969696969, a0, a1);

   //printf("ext rd, rs1, imm1, imm2\n");
   TESTINST_1_1(4, "ext a0, a1, 4, 0",   0x66666666ffffffff, a0, a1);
   TESTINST_1_1(4, "ext a0, a1, 7, 0",   0x66666666ffffff0f, a0, a1);
   TESTINST_1_1(4, "ext a0, a1, 12, 0",  0x66666666ffffffff, a0, a1);
   TESTINST_1_1(4, "ext a0, a1, 15, 0",  0x66666666ffff0fff, a0, a1);
   TESTINST_1_1(4, "ext a0, a1, 15, 0",  0x0000000000008000, a0, a1);

   //printf("extu rd, rs1, imm1, imm2\n");
   TESTINST_1_1(4, "extu a0, a1, 4, 0",   0x6666666699999999, a0, a1);
   TESTINST_1_1(4, "extu a0, a1, 7, 0",   0x6666666699999999, a0, a1);
   TESTINST_1_1(4, "extu a0, a1, 12, 0",  0x6666666699999999, a0, a1);
   TESTINST_1_1(4, "extu a0, a1, 15, 0",  0x6666666699999999, a0, a1);

   //printf("ff0 rd, rs1\n");
   TESTINST_1_1(4, "ff0 a0, a1", 0x0000000000000000, a0, a1);
   TESTINST_1_1(4, "ff0 a0, a1", 0xffff000000000000, a0, a1);
   TESTINST_1_1(4, "ff0 a0, a1", 0xffffff0000000000, a0, a1);
   TESTINST_1_1(4, "ff0 a0, a1", 0xffffffffff000000, a0, a1);
   TESTINST_1_1(4, "ff0 a0, a1", 0xffffffffffffffff, a0, a1);

   //printf("ff1 rd, rs1\n");
   TESTINST_1_1(4, "ff1 a0, a1", 0x0000000000000000, a0, a1);
   TESTINST_1_1(4, "ff1 a0, a1", 0x0000000000000001, a0, a1);
   TESTINST_1_1(4, "ff1 a0, a1", 0x0000000000000010, a0, a1);
   TESTINST_1_1(4, "ff1 a0, a1", 0x0000000100000000, a0, a1);
   TESTINST_1_1(4, "ff1 a0, a1", 0x0000001000000000, a0, a1);
   TESTINST_1_1(4, "ff1 a0, a1", 0x6000001000000000, a0, a1);
   TESTINST_1_1(4, "ff1 a0, a1", 0xf000001000000000, a0, a1);

   //printf("rev rd, rs1\n");
   TESTINST_1_1(4, "rev a0, a1", 0x0011223344556677, a0, a1);
   TESTINST_1_1(4, "rev a0, a1", 0x1122334455667788, a0, a1);
   TESTINST_1_1(4, "rev a0, a1", 0x2233445566778899, a0, a1);
   TESTINST_1_1(4, "rev a0, a1", 0x11223344aabbccdd, a0, a1);

   //printf("revw rd, rs1\n");
   TESTINST_1_1(4, "revw a0, a1", 0x0011223344556677, a0, a1);
   TESTINST_1_1(4, "revw a0, a1", 0x1122334455667788, a0, a1);
   TESTINST_1_1(4, "revw a0, a1", 0x2233445566778899, a0, a1);
   TESTINST_1_1(4, "revw a0, a1", 0x11223344aabbccdd, a0, a1);

   //printf("tstnbz rd, rs1\n");
   TESTINST_1_1(4, "tstnbz a0, a1", 0x0011223344556677, a0, a1);
   TESTINST_1_1(4, "tstnbz a0, a1", 0x0011003344556677, a0, a1);
   TESTINST_1_1(4, "tstnbz a0, a1", 0x0011003300556677, a0, a1);
   TESTINST_1_1(4, "tstnbz a0, a1", 0x0011003300550077, a0, a1);

   printf("Single-bit instructions (XTheadBs)\n");
   //printf("tst rd, rs1, imm6\n");
   TESTINST_1_1(4, "tst a0, a1, 0",  0x0011223344556677, a0, a1);
   TESTINST_1_1(4, "tst a0, a1, 8",  0x0011223344556677, a0, a1);
   TESTINST_1_1(4, "tst a0, a1, 33", 0x0011223344556677, a0, a1);
   TESTINST_1_1(4, "tst a0, a1, 42", 0x0011223344556677, a0, a1);

   printf("Conditional move (XTheadCondMov)\n");
   //printf("mveqz rd, rs1, rs2\n");
   TESTINST_1_2_SETRD(4, "mveqz a0, a1, a2", 0x000000000000abcd, 0x0011223344556677, 0x0000000000000000, a0, a1, a2);
   TESTINST_1_2_SETRD(4, "mveqz a0, a1, a2", 0x000000000000abcd, 0x0011223344556677, 0x0000000000000001, a0, a1, a2);

   //printf("mvnez rd, rs1, rs2\n");
   TESTINST_1_2_SETRD(4, "mvnez a0, a1, a2", 0x0000000000001234, 0xaa11223344556677, 0x0000000000000000, a0, a1, a2);
   TESTINST_1_2_SETRD(4, "mvnez a0, a1, a2", 0x0000000000001234, 0xaa11223344556677, 0x0000000000000001, a0, a1, a2);

   printf("Multiply-accumulate instructions (XTheadMac)\n");
   //printf("mula rd, rs1, rs2\n");
   TESTINST_1_2_SETRD(4, "mula a0, a1, a2", 0x0000000000000001, 0x0000000011111111, 0x0000000000000001, a0, a1, a2);
   TESTINST_1_2_SETRD(4, "mula a0, a1, a2", 0x0000000000000001, 0x00000000ffffffff, 0x0000000000000002, a0, a1, a2);

   //printf("mulah rd, rs1, rs2\n");
   TESTINST_1_2_SETRD(4, "mulah a0, a1, a2", 0x0000000000000001, 0x0000000011111111, 0x0000000000000002, a0, a1, a2);
   TESTINST_1_2_SETRD(4, "mulah a0, a1, a2", 0x0000000000000001, 0x00000000ffffffff, 0x0000000000000002, a0, a1, a2);

   //printf("mulaw rd, rs1, rs2\n");
   TESTINST_1_2_SETRD(4, "mulaw a0, a1, a2", 0x0000000000000001, 0x0000000011111111, 0x0000000000000002, a0, a1, a2);
   TESTINST_1_2_SETRD(4, "mulaw a0, a1, a2", 0x0000000000000001, 0x00000000ffffffff, 0x0000000000000002, a0, a1, a2);

   //printf("muls rd, rs1, rs2\n");
   TESTINST_1_2_SETRD(4, "muls a0, a1, a2", 0xffff000000000001, 0x0000000011111111, 0x0000000000000001, a0, a1, a2);
   TESTINST_1_2_SETRD(4, "muls a0, a1, a2", 0xffff000000000001, 0x00000000ffffffff, 0x0000000000000002, a0, a1, a2);

   //printf("mulsh rd, rs1, rs2\n");
   TESTINST_1_2_SETRD(4, "mulsh a0, a1, a2", 0xffff000000000001, 0x0000000011111111, 0x0000000000000001, a0, a1, a2);
   TESTINST_1_2_SETRD(4, "mulsh a0, a1, a2", 0xffff000000000001, 0x00000000ffffffff, 0x0000000000000002, a0, a1, a2);

   //printf("mulsw rd, rs1, rs2\n");
   TESTINST_1_2_SETRD(4, "mulsw a0, a1, a2", 0xffff000000000001, 0x0000000011111111, 0x0000000000000001, a0, a1, a2);
   TESTINST_1_2_SETRD(4, "mulsw a0, a1, a2", 0xffff000000000001, 0x00000000ffffffff, 0x0000000000000002, a0, a1, a2);
}

int main(void)
{
   test_integer_arithmetic();
   return 0;
}
