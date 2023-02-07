/* Tests for the T-HEAD vendor extension instruction set. */

#include <stdio.h>
#include <malloc.h>  // memalign
#include <string.h>  // memset
#include "tests/malloc.h"
#include <assert.h>

typedef  unsigned char           UChar;
typedef  unsigned short int      UShort;
typedef  unsigned int            UInt;
typedef  signed int              Int;
typedef  unsigned char           UChar;
typedef  signed long long int    Long;
typedef  unsigned long long int  ULong;

typedef  unsigned char           Bool;
#define False ((Bool)0)
#define True  ((Bool)1)

static inline UChar randUChar ( void )
{
   static UInt seed = 80021;
   seed = 1103515245 * seed + 12345;
   return (seed >> 17) & 0xFF;
}

static ULong randULong ( void )
{
   Int i;
   ULong r = 0;
   for (i = 0; i < 8; i++) {
      r = (r << 8) | (ULong)(0xFF & randUChar());
   }
   return r;
}

static void show_block_diff ( UChar* block1, UChar* block2, Int n )
{
   Int i;
   printf("  ");
   for (i = 0; i < n; i++) {
      if (i > 0 && 0 == (i & 15)) printf("\n  ");
      if (0 == (i & 15)) printf("[%3d]  ", i);
      UInt diff = 0xFF & (UInt)(block1[i] - block2[i]);
      if (diff == 0)
         printf(".. ");
      else
         printf("%02x ", block2[i]);
   }
   printf("\n");
}

#define MEM_TEST(INSN, AREG1OFF, AREG2VAL) { \
  int i; \
  const int N = 256; \
  UChar* area = memalign16(N); \
  UChar area2[N]; \
  for (i = 0; i < N; i++) area[i] = area2[i] = randUChar(); \
  ULong block[5]; \
  /* 0:x28      1:x29      2:f28       3:x5      4:x6 */ \
  for (i = 0; i < 5; i++) block[i] = randULong(); \
  block[3] = (ULong)(&area[128]) + (Long)(Int)AREG1OFF; \
  block[4] = (Long)AREG2VAL; \
  ULong block2[5]; \
  for (i = 0; i < 5; i++) block2[i] = block[i]; \
  __asm__ __volatile__( \
  "ld  x28, 0(%0)  ; " \
  "ld  x29, 8(%0)  ; " \
  "fld f28, 16(%0) ; " \
  "ld  x5,  24(%0) ; " \
  "ld  x6,  32(%0) ; " \
  INSN " ; " \
  "sd  x28, 0(%0)  ; " \
  "sd  x29, 8(%0)  ; " \
  "fsd f28, 16(%0) ; " \
  "sd  x5,  24(%0) ; " \
  "sd  x6,  32(%0) ; " \
  : : "r"(&block[0]) : "x5", "x6", "x28", "x29", \
                       "f28", "memory" \
  ); \
  printf("%s  with  x5 = middle_of_block+%lld,  x6=%lld\n", \
         INSN, (Long)AREG1OFF, (Long)AREG2VAL); \
  show_block_diff(&area2[0], area, 256); \
  printf("  %016llx  x28      (xfer intreg #1)\n", block[0]); \
  printf("  %016llx  x29      (xfer intreg #2)\n", block[1]); \
  printf("  %016llx  f28      (xfer freg #2)\n", block[2]); \
  printf("  %16lld   x5       (sub, base reg)\n",block[3] - block2[3]); \
  printf("  %16lld   x6       (index reg)\n",    block[4]); \
  printf("\n"); \
  free(area); \
  }


static __attribute__((noinline)) void test_integer_load_store(void)
{
printf("LRB/LRBU (register, imm2)\n");
MEM_TEST("lrb   x28, x5, x6, 0", 0, 0);
MEM_TEST("lrb   x28, x5, x6, 0", 0, 1);
MEM_TEST("lrb   x28, x5, x6, 1", 0, 2);
MEM_TEST("lrb   x28, x5, x6, 2", 0, 3);
MEM_TEST("lrb   x28, x5, x6, 3", 0, 4);
MEM_TEST("lrbu  x28, x5, x6, 0", 0, 0);
MEM_TEST("lrbu  x28, x5, x6, 0", 0, 1);
MEM_TEST("lrbu  x28, x5, x6, 1", 0, 2);
MEM_TEST("lrbu  x28, x5, x6, 2", 0, 3);
MEM_TEST("lrbu  x28, x5, x6, 3", 0, 4);
printf("SRB (register, imm2)\n");
MEM_TEST("srb   x28, x5, x6, 0", 0, 0);
MEM_TEST("srb   x28, x5, x6, 0", 0, 1);
MEM_TEST("srb   x28, x5, x6, 1", 0, 2);
MEM_TEST("srb   x28, x5, x6, 2", 0, 3);
MEM_TEST("srb   x28, x5, x6, 3", 0, 4);

printf("LURB/LURBU (register, imm2)\n");
MEM_TEST("lurb  x28, x5, x6, 0", 0, 0xFFFFFFFF00000000);
MEM_TEST("lurb  x28, x5, x6, 0", 0, 0xFFFFFFFF00000001);
MEM_TEST("lurb  x28, x5, x6, 1", 0, 0xFFFFFFFF00000002);
MEM_TEST("lurb  x28, x5, x6, 2", 0, 0xFFFFFFFF00000003);
MEM_TEST("lurb  x28, x5, x6, 3", 0, 0xFFFFFFFF00000004);
MEM_TEST("lurbu x28, x5, x6, 0", 0, 0xFFFFFFFF00000000);
MEM_TEST("lurbu x28, x5, x6, 0", 0, 0xFFFFFFFF00000001);
MEM_TEST("lurbu x28, x5, x6, 1", 0, 0xFFFFFFFF00000002);
MEM_TEST("lurbu x28, x5, x6, 2", 0, 0xFFFFFFFF00000003);
MEM_TEST("lurbu x28, x5, x6, 3", 0, 0xFFFFFFFF00000004);
printf("SURB (register, imm2)\n");
MEM_TEST("surb  x28, x5, x6, 0", 0, 0xFFFFFFFF00000000);
MEM_TEST("surb  x28, x5, x6, 0", 0, 0xFFFFFFFF00000001);
MEM_TEST("surb  x28, x5, x6, 1", 0, 0xFFFFFFFF00000002);
MEM_TEST("surb  x28, x5, x6, 2", 0, 0xFFFFFFFF00000003);
MEM_TEST("surb  x28, x5, x6, 3", 0, 0xFFFFFFFF00000004);

printf("LRH/LRHU (register, imm2)\n");
MEM_TEST("lrh   x28, x5, x6, 0", 0, 0);
MEM_TEST("lrh   x28, x5, x6, 1", 0, 1);
MEM_TEST("lrh   x28, x5, x6, 1", 0, 2);
MEM_TEST("lrh   x28, x5, x6, 2", 0, 3);
MEM_TEST("lrh   x28, x5, x6, 3", 0, 4);
MEM_TEST("lrhu  x28, x5, x6, 0", 0, 0);
MEM_TEST("lrhu  x28, x5, x6, 1", 0, 1);
MEM_TEST("lrhu  x28, x5, x6, 1", 0, 2);
MEM_TEST("lrhu  x28, x5, x6, 2", 0, 3);
MEM_TEST("lrhu  x28, x5, x6, 3", 0, 4);
printf("SRH (register, imm2)\n");
MEM_TEST("srh   x28, x5, x6, 0", 0, 0);
MEM_TEST("srh   x28, x5, x6, 1", 0, 1);
MEM_TEST("srh   x28, x5, x6, 1", 0, 2);
MEM_TEST("srh   x28, x5, x6, 2", 0, 3);
MEM_TEST("srh   x28, x5, x6, 3", 0, 4);

printf("LURH/LURHU (register, imm2)\n");
MEM_TEST("lurh  x28, x5, x6, 0", 0, 0xFFFFFFFF00000000);
MEM_TEST("lurh  x28, x5, x6, 1", 0, 0xFFFFFFFF00000001);
MEM_TEST("lurh  x28, x5, x6, 1", 0, 0xFFFFFFFF00000002);
MEM_TEST("lurh  x28, x5, x6, 2", 0, 0xFFFFFFFF00000003);
MEM_TEST("lurh  x28, x5, x6, 3", 0, 0xFFFFFFFF00000004);
MEM_TEST("lurhu x28, x5, x6, 0", 0, 0xFFFFFFFF00000000);
MEM_TEST("lurhu x28, x5, x6, 1", 0, 0xFFFFFFFF00000001);
MEM_TEST("lurhu x28, x5, x6, 1", 0, 0xFFFFFFFF00000002);
MEM_TEST("lurhu x28, x5, x6, 2", 0, 0xFFFFFFFF00000003);
MEM_TEST("lurhu x28, x5, x6, 3", 0, 0xFFFFFFFF00000004);
printf("SURH (register, imm2)\n");
MEM_TEST("surh  x28, x5, x6, 0", 0, 0xFFFFFFFF00000000);
MEM_TEST("surh  x28, x5, x6, 1", 0, 0xFFFFFFFF00000001);
MEM_TEST("surh  x28, x5, x6, 1", 0, 0xFFFFFFFF00000002);
MEM_TEST("surh  x28, x5, x6, 2", 0, 0xFFFFFFFF00000003);
MEM_TEST("surh  x28, x5, x6, 3", 0, 0xFFFFFFFF00000004);

printf("LRW/LRWU (register, imm2)\n");
MEM_TEST("lrw   x28, x5, x6, 0", 0, 0);
MEM_TEST("lrw   x28, x5, x6, 2", 0, 1);
MEM_TEST("lrw   x28, x5, x6, 2", 0, 2);
MEM_TEST("lrw   x28, x5, x6, 2", 0, 3);
MEM_TEST("lrw   x28, x5, x6, 3", 0, 4);
MEM_TEST("lrwu  x28, x5, x6, 0", 0, 0);
MEM_TEST("lrwu  x28, x5, x6, 2", 0, 1);
MEM_TEST("lrwu  x28, x5, x6, 2", 0, 2);
MEM_TEST("lrwu  x28, x5, x6, 2", 0, 3);
MEM_TEST("lrwu  x28, x5, x6, 3", 0, 4);
printf("SRW (register, imm2)\n");
MEM_TEST("srw   x28, x5, x6, 0", 0, 0);
MEM_TEST("srw   x28, x5, x6, 2", 0, 1);
MEM_TEST("srw   x28, x5, x6, 2", 0, 2);
MEM_TEST("srw   x28, x5, x6, 2", 0, 3);
MEM_TEST("srw   x28, x5, x6, 3", 0, 4);

printf("LURW/LURWU (register, imm2)\n");
MEM_TEST("lurw  x28, x5, x6, 0", 0, 0xFFFFFFFF00000000);
MEM_TEST("lurw  x28, x5, x6, 2", 0, 0xFFFFFFFF00000001);
MEM_TEST("lurw  x28, x5, x6, 2", 0, 0xFFFFFFFF00000002);
MEM_TEST("lurw  x28, x5, x6, 2", 0, 0xFFFFFFFF00000003);
MEM_TEST("lurw  x28, x5, x6, 3", 0, 0xFFFFFFFF00000004);
MEM_TEST("lurwu x28, x5, x6, 0", 0, 0xFFFFFFFF00000000);
MEM_TEST("lurwu x28, x5, x6, 2", 0, 0xFFFFFFFF00000001);
MEM_TEST("lurwu x28, x5, x6, 2", 0, 0xFFFFFFFF00000002);
MEM_TEST("lurwu x28, x5, x6, 2", 0, 0xFFFFFFFF00000003);
MEM_TEST("lurwu x28, x5, x6, 3", 0, 0xFFFFFFFF00000004);
printf("SURW (register, imm2)\n");
MEM_TEST("surw  x28, x5, x6, 0", 0, 0xFFFFFFFF00000000);
MEM_TEST("surw  x28, x5, x6, 2", 0, 0xFFFFFFFF00000001);
MEM_TEST("surw  x28, x5, x6, 2", 0, 0xFFFFFFFF00000002);
MEM_TEST("surw  x28, x5, x6, 2", 0, 0xFFFFFFFF00000003);
MEM_TEST("surw  x28, x5, x6, 3", 0, 0xFFFFFFFF00000004);

printf("LRD (register, imm2)\n");
MEM_TEST("lrd   x28, x5, x6, 0", 0, 0);
MEM_TEST("lrd   x28, x5, x6, 3", 0, 1);
MEM_TEST("lrd   x28, x5, x6, 3", 0, 2);
MEM_TEST("lrd   x28, x5, x6, 3", 0, 3);
MEM_TEST("lrd   x28, x5, x6, 3", 0, 4);
printf("SRD (register, imm2)\n");
MEM_TEST("srd   x28, x5, x6, 0", 0, 0);
MEM_TEST("srd   x28, x5, x6, 3", 0, 1);
MEM_TEST("srd   x28, x5, x6, 3", 0, 2);
MEM_TEST("srd   x28, x5, x6, 3", 0, 3);
MEM_TEST("srd   x28, x5, x6, 3", 0, 4);

printf("LURD (register, imm2)\n");
MEM_TEST("lurd  x28, x5, x6, 0", 0, 0xFFFFFFFF00000000);
MEM_TEST("lurd  x28, x5, x6, 3", 0, 0xFFFFFFFF00000001);
MEM_TEST("lurd  x28, x5, x6, 3", 0, 0xFFFFFFFF00000002);
MEM_TEST("lurd  x28, x5, x6, 3", 0, 0xFFFFFFFF00000003);
MEM_TEST("lurd  x28, x5, x6, 3", 0, 0xFFFFFFFF00000004);
printf("SURD (register, imm2)\n");
MEM_TEST("surd  x28, x5, x6, 0", 0, 0xFFFFFFFF00000000);
MEM_TEST("surd  x28, x5, x6, 3", 0, 0xFFFFFFFF00000001);
MEM_TEST("surd  x28, x5, x6, 3", 0, 0xFFFFFFFF00000002);
MEM_TEST("surd  x28, x5, x6, 3", 0, 0xFFFFFFFF00000003);
MEM_TEST("surd  x28, x5, x6, 3", 0, 0xFFFFFFFF00000004);

////////////////////////////////////////////////////////////////
printf("LBIA/LBIB (imm5, imm2)\n");
MEM_TEST("lbia   x28, (x5), 0, 0", 0, 0);
MEM_TEST("lbia   x28, (x5), 1, 0", 0, 0);
MEM_TEST("lbia   x28, (x5), 2, 1", 0, 0);
MEM_TEST("lbia   x28, (x5), 3, 2", 0, 0);
MEM_TEST("lbia   x28, (x5), 4, 3", 0, 0);
MEM_TEST("lbib   x28, (x5), 0, 0", 0, 0);
MEM_TEST("lbib   x28, (x5), 1, 0", 0, 0);
MEM_TEST("lbib   x28, (x5), 2, 1", 0, 0);
MEM_TEST("lbib   x28, (x5), 3, 2", 0, 0);
MEM_TEST("lbib   x28, (x5), 4, 3", 0, 0);

MEM_TEST("lbuia   x28, (x5), 0, 0", 0, 0);
MEM_TEST("lbuia   x28, (x5), 1, 0", 0, 0);
MEM_TEST("lbuia   x28, (x5), 2, 1", 0, 0);
MEM_TEST("lbuia   x28, (x5), 3, 2", 0, 0);
MEM_TEST("lbuia   x28, (x5), 4, 3", 0, 0);
MEM_TEST("lbuib   x28, (x5), 0, 0", 0, 0);
MEM_TEST("lbuib   x28, (x5), 1, 0", 0, 0);
MEM_TEST("lbuib   x28, (x5), 2, 1", 0, 0);
MEM_TEST("lbuib   x28, (x5), 3, 2", 0, 0);
MEM_TEST("lbuib   x28, (x5), 4, 3", 0, 0);
printf("SBIA/SBIB (imm5, imm2)\n");
MEM_TEST("sbia   x28, (x5), 0, 0", 0, 0);
MEM_TEST("sbia   x28, (x5), 1, 0", 0, 0);
MEM_TEST("sbia   x28, (x5), 2, 1", 0, 0);
MEM_TEST("sbia   x28, (x5), 3, 2", 0, 0);
MEM_TEST("sbia   x28, (x5), 4, 3", 0, 0);
MEM_TEST("sbib   x28, (x5), 0, 0", 0, 0);
MEM_TEST("sbib   x28, (x5), 1, 0", 0, 0);
MEM_TEST("sbib   x28, (x5), 2, 1", 0, 0);
MEM_TEST("sbib   x28, (x5), 3, 2", 0, 0);
MEM_TEST("sbib   x28, (x5), 4, 3", 0, 0);

printf("LHIA/LHIB (imm5, imm2)\n");
MEM_TEST("lhia   x28, (x5), 0, 0", 0, 0);
MEM_TEST("lhia   x28, (x5), 1, 1", 0, 0);
MEM_TEST("lhia   x28, (x5), 2, 1", 0, 0);
MEM_TEST("lhia   x28, (x5), 3, 2", 0, 0);
MEM_TEST("lhia   x28, (x5), 4, 3", 0, 0);
MEM_TEST("lhib   x28, (x5), 0, 0", 0, 0);
MEM_TEST("lhib   x28, (x5), 1, 1", 0, 0);
MEM_TEST("lhib   x28, (x5), 2, 1", 0, 0);
MEM_TEST("lhib   x28, (x5), 3, 2", 0, 0);
MEM_TEST("lhib   x28, (x5), 4, 3", 0, 0);

MEM_TEST("lhuia   x28, (x5), 0, 0", 0, 0);
MEM_TEST("lhuia   x28, (x5), 1, 1", 0, 0);
MEM_TEST("lhuia   x28, (x5), 2, 1", 0, 0);
MEM_TEST("lhuia   x28, (x5), 3, 2", 0, 0);
MEM_TEST("lhuia   x28, (x5), 4, 3", 0, 0);
MEM_TEST("lhuib   x28, (x5), 0, 0", 0, 0);
MEM_TEST("lhuib   x28, (x5), 1, 1", 0, 0);
MEM_TEST("lhuib   x28, (x5), 2, 1", 0, 0);
MEM_TEST("lhuib   x28, (x5), 3, 2", 0, 0);
MEM_TEST("lhuib   x28, (x5), 4, 3", 0, 0);
printf("SHIA/SHIB (imm5, imm2)\n");
MEM_TEST("shia   x28, (x5), 0, 0", 0, 0);
MEM_TEST("shia   x28, (x5), 1, 1", 0, 0);
MEM_TEST("shia   x28, (x5), 2, 1", 0, 0);
MEM_TEST("shia   x28, (x5), 3, 2", 0, 0);
MEM_TEST("shia   x28, (x5), 4, 3", 0, 0);
MEM_TEST("shib   x28, (x5), 0, 0", 0, 0);
MEM_TEST("shib   x28, (x5), 1, 1", 0, 0);
MEM_TEST("shib   x28, (x5), 2, 1", 0, 0);
MEM_TEST("shib   x28, (x5), 3, 2", 0, 0);
MEM_TEST("shib   x28, (x5), 4, 3", 0, 0);

printf("LWIA/LWIB (imm5, imm2)\n");
MEM_TEST("lwia   x28, (x5), 0, 0", 0, 0);
MEM_TEST("lwia   x28, (x5), 1, 2", 0, 0);
MEM_TEST("lwia   x28, (x5), 2, 2", 0, 0);
MEM_TEST("lwia   x28, (x5), 3, 2", 0, 0);
MEM_TEST("lwia   x28, (x5), 4, 3", 0, 0);
MEM_TEST("lwib   x28, (x5), 0, 0", 0, 0);
MEM_TEST("lwib   x28, (x5), 1, 2", 0, 0);
MEM_TEST("lwib   x28, (x5), 2, 2", 0, 0);
MEM_TEST("lwib   x28, (x5), 3, 2", 0, 0);
MEM_TEST("lwib   x28, (x5), 4, 3", 0, 0);

MEM_TEST("lwuia   x28, (x5), 0, 0", 0, 0);
MEM_TEST("lwuia   x28, (x5), 1, 2", 0, 0);
MEM_TEST("lwuia   x28, (x5), 2, 2", 0, 0);
MEM_TEST("lwuia   x28, (x5), 3, 2", 0, 0);
MEM_TEST("lwuia   x28, (x5), 4, 3", 0, 0);
MEM_TEST("lwuib   x28, (x5), 0, 0", 0, 0);
MEM_TEST("lwuib   x28, (x5), 1, 2", 0, 0);
MEM_TEST("lwuib   x28, (x5), 2, 2", 0, 0);
MEM_TEST("lwuib   x28, (x5), 3, 2", 0, 0);
MEM_TEST("lwuib   x28, (x5), 4, 3", 0, 0);
printf("SWIA/SWIB (imm5, imm2)\n");
MEM_TEST("swia   x28, (x5), 0, 0", 0, 0);
MEM_TEST("swia   x28, (x5), 1, 2", 0, 0);
MEM_TEST("swia   x28, (x5), 2, 2", 0, 0);
MEM_TEST("swia   x28, (x5), 3, 2", 0, 0);
MEM_TEST("swia   x28, (x5), 4, 3", 0, 0);
MEM_TEST("swib   x28, (x5), 0, 0", 0, 0);
MEM_TEST("swib   x28, (x5), 1, 2", 0, 0);
MEM_TEST("swib   x28, (x5), 2, 2", 0, 0);
MEM_TEST("swib   x28, (x5), 3, 2", 0, 0);
MEM_TEST("swib   x28, (x5), 4, 3", 0, 0);

printf("LDIA/LDIB (imm5, imm2)\n");
MEM_TEST("ldia   x28, (x5), 0, 0", 0, 0);
MEM_TEST("ldia   x28, (x5), 1, 3", 0, 0);
MEM_TEST("ldia   x28, (x5), 2, 3", 0, 0);
MEM_TEST("ldia   x28, (x5), 3, 3", 0, 0);
MEM_TEST("ldia   x28, (x5), 4, 3", 0, 0);
MEM_TEST("ldib   x28, (x5), 0, 0", 0, 0);
MEM_TEST("ldib   x28, (x5), 1, 3", 0, 0);
MEM_TEST("ldib   x28, (x5), 2, 3", 0, 0);
MEM_TEST("ldib   x28, (x5), 3, 3", 0, 0);
MEM_TEST("ldib   x28, (x5), 4, 3", 0, 0);
printf("SDIA/SDIB (imm5, imm2)\n");
MEM_TEST("sdia   x28, (x5), 0, 0", 0, 0);
MEM_TEST("sdia   x28, (x5), 1, 3", 0, 0);
MEM_TEST("sdia   x28, (x5), 2, 3", 0, 0);
MEM_TEST("sdia   x28, (x5), 3, 3", 0, 0);
MEM_TEST("sdia   x28, (x5), 4, 3", 0, 0);
MEM_TEST("sdib   x28, (x5), 0, 0", 0, 0);
MEM_TEST("sdib   x28, (x5), 1, 3", 0, 0);
MEM_TEST("sdib   x28, (x5), 2, 3", 0, 0);
MEM_TEST("sdib   x28, (x5), 3, 3", 0, 0);
MEM_TEST("sdib   x28, (x5), 4, 3", 0, 0);

////////////////////////////////////////////////////////////////
printf("LWD/LWUD (imm2)\n");
MEM_TEST("lwd   x28,x29, (x5), 0, 3", 0, 0);
MEM_TEST("lwd   x28,x29, (x5), 1, 3", 0, 0);
MEM_TEST("lwd   x28,x29, (x5), 2, 3", 0, 0);
MEM_TEST("lwd   x28,x29, (x5), 3, 3", 0, 0);
MEM_TEST("lwud  x28,x29, (x5), 0, 3", 0, 0);
MEM_TEST("lwud  x28,x29, (x5), 1, 3", 0, 0);
MEM_TEST("lwud  x28,x29, (x5), 2, 3", 0, 0);
MEM_TEST("lwud  x28,x29, (x5), 3, 3", 0, 0);
printf("SWD (imm2)\n");
MEM_TEST("swd   x28,x29, (x5), 0, 3", 0, 0);
MEM_TEST("swd   x28,x29, (x5), 1, 3", 0, 0);
MEM_TEST("swd   x28,x29, (x5), 2, 3", 0, 0);
MEM_TEST("swd   x28,x29, (x5), 3, 3", 0, 0);

printf("LDD (imm2)\n");
MEM_TEST("ldd   x28,x29, (x5), 0, 4", 0, 0);
MEM_TEST("ldd   x28,x29, (x5), 1, 4", 0, 0);
MEM_TEST("ldd   x28,x29, (x5), 2, 4", 0, 0);
MEM_TEST("ldd   x28,x29, (x5), 3, 4", 0, 0);
printf("SDD (imm2)\n");
MEM_TEST("sdd   x28,x29, (x5), 0, 4", 0, 0);
MEM_TEST("sdd   x28,x29, (x5), 1, 4", 0, 0);
MEM_TEST("sdd   x28,x29, (x5), 2, 4", 0, 0);
MEM_TEST("sdd   x28,x29, (x5), 3, 4", 0, 0);
}

static __attribute__((noinline)) void test_float_load_store(void)
{
printf("FLRW (register, imm2)\n");
MEM_TEST("flrw   f28, x5, x6, 0", 0, 0);
MEM_TEST("flrw   f28, x5, x6, 2", 0, 1);
MEM_TEST("flrw   f28, x5, x6, 2", 0, 2);
MEM_TEST("flrw   f28, x5, x6, 2", 0, 3);
MEM_TEST("flrw   f28, x5, x6, 3", 0, 4);
printf("FSRW (register, imm2)\n");
MEM_TEST("fsrw   f28, x5, x6, 0", 0, 0);
MEM_TEST("fsrw   f28, x5, x6, 2", 0, 1);
MEM_TEST("fsrw   f28, x5, x6, 2", 0, 2);
MEM_TEST("fsrw   f28, x5, x6, 2", 0, 3);
MEM_TEST("fsrw   f28, x5, x6, 3", 0, 4);

printf("FLURW (register, imm2)\n");
MEM_TEST("flurw  f28, x5, x6, 0", 0, 0xFFFFFFFF00000000);
MEM_TEST("flurw  f28, x5, x6, 2", 0, 0xFFFFFFFF00000001);
MEM_TEST("flurw  f28, x5, x6, 2", 0, 0xFFFFFFFF00000002);
MEM_TEST("flurw  f28, x5, x6, 2", 0, 0xFFFFFFFF00000003);
MEM_TEST("flurw  f28, x5, x6, 3", 0, 0xFFFFFFFF00000004);
printf("FSURW (register, imm2)\n");
MEM_TEST("fsurw  f28, x5, x6, 0", 0, 0xFFFFFFFF00000000);
MEM_TEST("fsurw  f28, x5, x6, 2", 0, 0xFFFFFFFF00000001);
MEM_TEST("fsurw  f28, x5, x6, 2", 0, 0xFFFFFFFF00000002);
MEM_TEST("fsurw  f28, x5, x6, 2", 0, 0xFFFFFFFF00000003);
MEM_TEST("fsurw  f28, x5, x6, 3", 0, 0xFFFFFFFF00000004);

printf("FLRD (register, imm2)\n");
MEM_TEST("flrd   f28, x5, x6, 0", 0, 0);
MEM_TEST("flrd   f28, x5, x6, 3", 0, 1);
MEM_TEST("flrd   f28, x5, x6, 3", 0, 2);
MEM_TEST("flrd   f28, x5, x6, 3", 0, 3);
MEM_TEST("flrd   f28, x5, x6, 3", 0, 4);
printf("FSRD (register, imm2)\n");
MEM_TEST("fsrd   f28, x5, x6, 0", 0, 0);
MEM_TEST("fsrd   f28, x5, x6, 3", 0, 1);
MEM_TEST("fsrd   f28, x5, x6, 3", 0, 2);
MEM_TEST("fsrd   f28, x5, x6, 3", 0, 3);
MEM_TEST("fsrd   f28, x5, x6, 3", 0, 4);

printf("FLURD (register, imm2)\n");
MEM_TEST("flurd  f28, x5, x6, 0", 0, 0xFFFFFFFF00000000);
MEM_TEST("flurd  f28, x5, x6, 3", 0, 0xFFFFFFFF00000001);
MEM_TEST("flurd  f28, x5, x6, 3", 0, 0xFFFFFFFF00000002);
MEM_TEST("flurd  f28, x5, x6, 3", 0, 0xFFFFFFFF00000003);
MEM_TEST("flurd  f28, x5, x6, 3", 0, 0xFFFFFFFF00000004);
printf("FSURD (register, imm2)\n");
MEM_TEST("fsurd  f28, x5, x6, 0", 0, 0xFFFFFFFF00000000);
MEM_TEST("fsurd  f28, x5, x6, 3", 0, 0xFFFFFFFF00000001);
MEM_TEST("fsurd  f28, x5, x6, 3", 0, 0xFFFFFFFF00000002);
MEM_TEST("fsurd  f28, x5, x6, 3", 0, 0xFFFFFFFF00000003);
MEM_TEST("fsurd  f28, x5, x6, 3", 0, 0xFFFFFFFF00000004);
}

int main(void)
{
   test_integer_load_store();
   test_float_load_store();
   return 0;
}
