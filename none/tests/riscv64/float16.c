/* Tests for the RV64 Zfh standard half-precision floating-point instruction-set
   extension. */

#include "testinst.h"

static void test_float16_shared(void)
{
   printf("RV64 Zfh half-precision FP instruction set.\n");

   /* --------------- flh rd, imm[11:0](rs1) ---------------- */
   TESTINST_1_1_FLOAD(4, "flh fa0, 0(a1)", fa0, a1);
   TESTINST_1_1_FLOAD(4, "flh fa0, 4(a1)", fa0, a1);
   TESTINST_1_1_FLOAD(4, "flh fa0, 8(a1)", fa0, a1);
   TESTINST_1_1_FLOAD(4, "flh fa0, 16(a1)", fa0, a1);
   TESTINST_1_1_FLOAD(4, "flh fa0, 32(a1)", fa0, a1);
   TESTINST_1_1_FLOAD(4, "flh fa0, 64(a1)", fa0, a1);
   TESTINST_1_1_FLOAD(4, "flh fa0, 128(a1)", fa0, a1);
   TESTINST_1_1_FLOAD(4, "flh fa0, 256(a1)", fa0, a1);
   TESTINST_1_1_FLOAD(4, "flh fa0, 512(a1)", fa0, a1);
   TESTINST_1_1_FLOAD(4, "flh fa0, 1024(a1)", fa0, a1);
   TESTINST_1_1_FLOAD(4, "flh fa0, 2040(a1)", fa0, a1);
   TESTINST_1_1_FLOAD(4, "flh fa0, -4(a1)", fa0, a1);
   TESTINST_1_1_FLOAD(4, "flh fa0, -2048(a1)", fa0, a1);

   TESTINST_1_1_FLOAD(4, "flh fa4, 0(a5)", fa4, a5);

   /* --------------- fsh rs2, imm[11:0](rs1) --------------- */
   TESTINST_0_2_FSTORE(4, "fsh fa0, 0(a1)", 0xabcdef0123456789, fa0, a1);
   TESTINST_0_2_FSTORE(4, "fsh fa0, 4(a1)", 0xabcdef0123456789, fa0, a1);
   TESTINST_0_2_FSTORE(4, "fsh fa0, 8(a1)", 0xabcdef0123456789, fa0, a1);
   TESTINST_0_2_FSTORE(4, "fsh fa0, 16(a1)", 0xabcdef0123456789, fa0, a1);
   TESTINST_0_2_FSTORE(4, "fsh fa0, 32(a1)", 0xabcdef0123456789, fa0, a1);
   TESTINST_0_2_FSTORE(4, "fsh fa0, 64(a1)", 0xabcdef0123456789, fa0, a1);
   TESTINST_0_2_FSTORE(4, "fsh fa0, 128(a1)", 0xabcdef0123456789, fa0, a1);
   TESTINST_0_2_FSTORE(4, "fsh fa0, 256(a1)", 0xabcdef0123456789, fa0, a1);
   TESTINST_0_2_FSTORE(4, "fsh fa0, 512(a1)", 0xabcdef0123456789, fa0, a1);
   TESTINST_0_2_FSTORE(4, "fsh fa0, 1024(a1)", 0xabcdef0123456789, fa0, a1);
   TESTINST_0_2_FSTORE(4, "fsh fa0, 2040(a1)", 0xabcdef0123456789, fa0, a1);
   TESTINST_0_2_FSTORE(4, "fsh fa0, -4(a1)", 0xabcdef0123456789, fa0, a1);
   TESTINST_0_2_FSTORE(4, "fsh fa0, -2048(a1)", 0xabcdef0123456789, fa0, a1);

   TESTINST_0_2_FSTORE(4, "fsh fa4, 0(a5)", 0xabcdef0123456789, fa4, a5);

   /* ------------ fmadd.h rd, rs1, rs2, rs3, rm ------------ */
   /* 3.0 * 2.0 + 1.0 -> 7.0 */
   TESTINST_1_3_F(4, "fmadd.h fa0, fa1, fa2, fa3", 0xffffffffffff4200,
                  0xffffffffffff4000, 0xffffffffffff3c00, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * 1.0 + -1.0 -> 0.0 */
   TESTINST_1_3_F(4, "fmadd.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffffbc00, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 2.0 * HFLT_TRUE_MIN + -HFLT_TRUE_MIN -> HFLT_TRUE_MIN (no UF because exact)
    */
   TESTINST_1_3_F(4, "fmadd.h fa0, fa1, fa2, fa3", 0xffffffffffff4000,
                  0xffffffffffff0001, 0xffffffffffff8001, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 2.0 * FLT_MAX + -FLT_MAX -> FLT_MAX */
   TESTINST_1_3_F(4, "fmadd.h fa0, fa1, fa2, fa3", 0xffffffffffff4000,
                  0xffffffffffff7bff, 0xfffffffffffffbff, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 2.0 * FLT_MAX + 0.0 -> INFINITY (OF, NX) */
   TESTINST_1_3_F(4, "fmadd.h fa0, fa1, fa2, fa3", 0xffffffffffff4000,
                  0xffffffffffff7bff, 0xffffffffffff0000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 2.0 * INFINITY + -INFINITY -> qNAN (NV) */
   TESTINST_1_3_F(4, "fmadd.h fa0, fa1, fa2, fa3", 0xffffffffffff4000,
                  0xffffffffffff7c00, 0xfffffffffffffc00, 0x00, fa0, fa1, fa2,
                  fa3);

   /* 1.0 * 1.0 + HFLT_EPSILON/2 (RNE) -> 1.0 (NX) */
   TESTINST_1_3_F(4, "fmadd.h fa0, fa1, fa2, fa3, rne", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * nextafterf(1.0) + HFLT_EPSILON/2 (RNE) -> 2nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fmadd.h fa0, fa1, fa2, fa3, rne", 0xffffffffffff3c00,
                  0xffffffffffff3bff, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * 1.0 + HFLT_EPSILON/2 (RTZ) -> 1.0 (NX) */
   TESTINST_1_3_F(4, "fmadd.h fa0, fa1, fa2, fa3, rtz", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * -1.0 + -HFLT_EPSILON/2 (RTZ) -> -1.0 (NX) */
   TESTINST_1_3_F(4, "fmadd.h fa0, fa1, fa2, fa3, rtz", 0xffffffffffff3c00,
                  0xffffffffffffbc00, 0xffffffffffff9000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * 1.0 + HFLT_EPSILON/2 (RDN) -> 1.0 (NX) */
   TESTINST_1_3_F(4, "fmadd.h fa0, fa1, fa2, fa3, rdn", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * -1.0 + -HFLT_EPSILON/2 (RDN) -> -nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fmadd.h fa0, fa1, fa2, fa3, rdn", 0xffffffffffff3c00,
                  0xffffffffffffbc00, 0xffffffffffff9000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * 1.0 + HFLT_EPSILON/2 (RUP) -> nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fmadd.h fa0, fa1, fa2, fa3, rup", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * -1.0 + -HFLT_EPSILON/2 (RUP) -> -1.0 (NX) */
   TESTINST_1_3_F(4, "fmadd.h fa0, fa1, fa2, fa3, rup", 0xffffffffffff3c00,
                  0xffffffffffffbc00, 0xffffffffffff9000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * 1.0 + HFLT_EPSILON/2 (RMM) -> nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fmadd.h fa0, fa1, fa2, fa3, rmm", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * -1.0 + -HFLT_EPSILON/2 (RMM) -> -nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fmadd.h fa0, fa1, fa2, fa3, rmm", 0xffffffffffff3c00,
                  0xffffffffffffbc00, 0xffffffffffff9000, 0x00, fa0, fa1, fa2,
                  fa3);

   /* 1.0 * 1.0 + HFLT_EPSILON/2 (DYN-RNE) -> 1.0 (NX) */
   TESTINST_1_3_F(4, "fmadd.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * nextafterf(1.0) + HFLT_EPSILON/2 (DYN-RNE) -> 2nextafterf(1.0) (NX)
    */
   TESTINST_1_3_F(4, "fmadd.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffff3bff, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * 1.0 + HFLT_EPSILON/2 (DYN-RTZ) -> 1.0 (NX) */
   TESTINST_1_3_F(4, "fmadd.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x20, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * -1.0 + -HFLT_EPSILON/2 (DYN-RTZ) -> -1.0 (NX) */
   TESTINST_1_3_F(4, "fmadd.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffffbc00, 0xffffffffffff9000, 0x20, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * 1.0 + HFLT_EPSILON/2 (DYN-RDN) -> 1.0 (NX) */
   TESTINST_1_3_F(4, "fmadd.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x40, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * -1.0 + -HFLT_EPSILON/2 (DYN-RDN) -> -nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fmadd.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffffbc00, 0xffffffffffff9000, 0x40, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * 1.0 + HFLT_EPSILON/2 (DYN-RUP) -> nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fmadd.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x60, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * -1.0 + -HFLT_EPSILON/2 (DYN-RUP) -> -1.0 (NX) */
   TESTINST_1_3_F(4, "fmadd.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffffbc00, 0xffffffffffff9000, 0x60, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * 1.0 + HFLT_EPSILON/2 (DYN-RMM) -> nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fmadd.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x80, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * -1.0 + -HFLT_EPSILON/2 (DYN-RMM) -> -nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fmadd.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffffbc00, 0xffffffffffff9000, 0x80, fa0, fa1, fa2,
                  fa3);

   /* ------------ fmsub.h rd, rs1, rs2, rs3, rm ------------ */
   /* 3.0 * 2.0 - 1.0 -> 5.0 */
   TESTINST_1_3_F(4, "fmsub.h fa0, fa1, fa2, fa3", 0xffffffffffff4200,
                  0xffffffffffff4000, 0xffffffffffff3c00, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * 1.0 - 1.0 -> 0.0 */
   TESTINST_1_3_F(4, "fmsub.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff3c00, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 2.0 * HFLT_TRUE_MIN - HFLT_TRUE_MIN -> HFLT_TRUE_MIN (no UF because exact)
    */
   TESTINST_1_3_F(4, "fmsub.h fa0, fa1, fa2, fa3", 0xffffffffffff4000,
                  0xffffffffffff0001, 0xffffffffffff0001, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 2.0 * FLT_MAX - FLT_MAX -> FLT_MAX */
   TESTINST_1_3_F(4, "fmsub.h fa0, fa1, fa2, fa3", 0xffffffffffff4000,
                  0xffffffffffff7bff, 0xffffffffffff7bff, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 2.0 * FLT_MAX - 0.0 -> INFINITY (OF, NX) */
   TESTINST_1_3_F(4, "fmsub.h fa0, fa1, fa2, fa3", 0xffffffffffff4000,
                  0xffffffffffff7bff, 0xffffffffffff0000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 2.0 * INFINITY - INFINITY -> qNAN (NV) */
   TESTINST_1_3_F(4, "fmsub.h fa0, fa1, fa2, fa3", 0xffffffffffff4000,
                  0xffffffffffff7c00, 0xffffffffffff7c00, 0x00, fa0, fa1, fa2,
                  fa3);

   /* 1.0 * nextafterf(1.0) - HFLT_EPSILON/2 (RNE) -> 1.0 (NX) */
   TESTINST_1_3_F(4, "fmsub.h fa0, fa1, fa2, fa3, rne", 0xffffffffffff3c00,
                  0xffffffffffff3bff, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * 2nextafterf(1.0) - HFLT_EPSILON/2 (RNE) -> 2nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fmsub.h fa0, fa1, fa2, fa3, rne", 0xffffffffffff3c00,
                  0xffffffffffff3fff, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * nextafterf(1.0) - HFLT_EPSILON/2 (RTZ) -> 1.0 (NX) */
   TESTINST_1_3_F(4, "fmsub.h fa0, fa1, fa2, fa3, rtz", 0xffffffffffff3c00,
                  0xffffffffffff3bff, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * -1.0 - HFLT_EPSILON/2 (RTZ) -> -1.0 (NX) */
   TESTINST_1_3_F(4, "fmsub.h fa0, fa1, fa2, fa3, rtz", 0xffffffffffff3c00,
                  0xffffffffffffbc00, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * nextafterf(1.0) - HFLT_EPSILON/2 (RDN) -> 1.0 (NX) */
   TESTINST_1_3_F(4, "fmsub.h fa0, fa1, fa2, fa3, rdn", 0xffffffffffff3c00,
                  0xffffffffffff3bff, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * -1.0 - HFLT_EPSILON/2 (RDN) -> -nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fmsub.h fa0, fa1, fa2, fa3, rdn", 0xffffffffffff3c00,
                  0xffffffffffffbc00, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * nextafterf(1.0) - HFLT_EPSILON/2 (RUP) -> nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fmsub.h fa0, fa1, fa2, fa3, rup", 0xffffffffffff3c00,
                  0xffffffffffff3bff, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * -1.0 - HFLT_EPSILON/2 (RUP) -> -1.0 (NX) */
   TESTINST_1_3_F(4, "fmsub.h fa0, fa1, fa2, fa3, rup", 0xffffffffffff3c00,
                  0xffffffffffffbc00, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * nextafterf(1.0) - HFLT_EPSILON/2 (RMM) -> nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fmsub.h fa0, fa1, fa2, fa3, rmm", 0xffffffffffff3c00,
                  0xffffffffffff3bff, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * -1.0 - HFLT_EPSILON/2 (RMM) -> -nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fmsub.h fa0, fa1, fa2, fa3, rmm", 0xffffffffffff3c00,
                  0xffffffffffffbc00, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);

   /* 1.0 * nextafterf(1.0) - HFLT_EPSILON/2 (DYN-RNE) -> 1.0 (NX) */
   TESTINST_1_3_F(4, "fmsub.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffff3bff, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * 2nextafterf(1.0) - HFLT_EPSILON/2 (DYN-RNE) -> 2nextafterf(1.0) (NX)
    */
   TESTINST_1_3_F(4, "fmsub.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffff3fff, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * nextafterf(1.0) - HFLT_EPSILON/2 (DYN-RTZ) -> 1.0 (NX) */
   TESTINST_1_3_F(4, "fmsub.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffff3bff, 0xffffffffffff1000, 0x20, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * -1.0 - HFLT_EPSILON/2 (DYN-RTZ) -> -1.0 (NX) */
   TESTINST_1_3_F(4, "fmsub.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffffbc00, 0xffffffffffff1000, 0x20, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * nextafterf(1.0) - HFLT_EPSILON/2 (DYN-RDN) -> 1.0 (NX) */
   TESTINST_1_3_F(4, "fmsub.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffff3bff, 0xffffffffffff1000, 0x40, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * -1.0 - HFLT_EPSILON/2 (DYN-RDN) -> -nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fmsub.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffffbc00, 0xffffffffffff1000, 0x40, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * nextafterf(1.0) - HFLT_EPSILON/2 (DYN-RUP) -> nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fmsub.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffff3bff, 0xffffffffffff1000, 0x60, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * -1.0 - HFLT_EPSILON/2 (DYN-RUP) -> -1.0 (NX) */
   TESTINST_1_3_F(4, "fmsub.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffffbc00, 0xffffffffffff1000, 0x60, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * nextafterf(1.0) - HFLT_EPSILON/2 (DYN-RMM) -> nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fmsub.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffff3bff, 0xffffffffffff1000, 0x80, fa0, fa1, fa2,
                  fa3);
   /* 1.0 * -1.0 - HFLT_EPSILON/2 (DYN-RMM) -> -nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fmsub.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffffbc00, 0xffffffffffff1000, 0x80, fa0, fa1, fa2,
                  fa3);

   /* ----------- fnmsub.h rd, rs1, rs2, rs3, rm ------------ */
   /* -(3.0 * 2.0) + 1.0 -> -5.0 */
   TESTINST_1_3_F(4, "fnmsub.h fa0, fa1, fa2, fa3", 0xffffffffffff4200,
                  0xffffffffffff4000, 0xffffffffffff3c00, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(1.0 * 1.0) + 1.0 -> 0.0 */
   TESTINST_1_3_F(4, "fnmsub.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff3c00, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(2.0 * HFLT_TRUE_MIN) + HFLT_TRUE_MIN -> -HFLT_TRUE_MIN (no UF because
      exact) */
   TESTINST_1_3_F(4, "fnmsub.h fa0, fa1, fa2, fa3", 0xffffffffffff4000,
                  0xffffffffffff0001, 0xffffffffffff0001, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(2.0 * FLT_MAX) + FLT_MAX -> -FLT_MAX */
   TESTINST_1_3_F(4, "fnmsub.h fa0, fa1, fa2, fa3", 0xffffffffffff4000,
                  0xffffffffffff7bff, 0xffffffffffff7bff, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(2.0 * FLT_MAX) + 0.0 -> -INFINITY (OF, NX) */
   TESTINST_1_3_F(4, "fnmsub.h fa0, fa1, fa2, fa3", 0xffffffffffff4000,
                  0xffffffffffff7bff, 0xffffffffffff0000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(2.0 * INFINITY) + INFINITY -> qNAN (NV) */
   TESTINST_1_3_F(4, "fnmsub.h fa0, fa1, fa2, fa3", 0xffffffffffff4000,
                  0xffffffffffff7c00, 0xffffffffffff7c00, 0x00, fa0, fa1, fa2,
                  fa3);

   /* -(-1.0 * 1.0) + HFLT_EPSILON/2 (RNE) -> 1.0 (NX) */
   TESTINST_1_3_F(4, "fnmsub.h fa0, fa1, fa2, fa3, rne", 0xffffffffffffbc00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(-1.0 * nextafterf(1.0)) + HFLT_EPSILON/2 (RNE) -> 2nextafterf(1.0) (NX)
    */
   TESTINST_1_3_F(4, "fnmsub.h fa0, fa1, fa2, fa3, rne", 0xffffffffffffbc00,
                  0xffffffffffff3bff, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(-1.0 * 1.0) + HFLT_EPSILON/2 (RTZ) -> 1.0 (NX) */
   TESTINST_1_3_F(4, "fnmsub.h fa0, fa1, fa2, fa3, rtz", 0xffffffffffffbc00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(1.0 * 1.0) + -HFLT_EPSILON/2 (RTZ) -> -1.0 (NX) */
   TESTINST_1_3_F(4, "fnmsub.h fa0, fa1, fa2, fa3, rtz", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff9000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(-1.0 * 1.0) + HFLT_EPSILON/2 (RDN) -> 1.0 (NX) */
   TESTINST_1_3_F(4, "fnmsub.h fa0, fa1, fa2, fa3, rdn", 0xffffffffffffbc00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(1.0 * 1.0) + -HFLT_EPSILON/2 (RDN) -> -nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fnmsub.h fa0, fa1, fa2, fa3, rdn", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff9000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(-1.0 * 1.0) + HFLT_EPSILON/2 (RUP) -> nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fnmsub.h fa0, fa1, fa2, fa3, rup", 0xffffffffffffbc00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(1.0 * 1.0) + -HFLT_EPSILON/2 (RUP) -> -1.0 (NX) */
   TESTINST_1_3_F(4, "fnmsub.h fa0, fa1, fa2, fa3, rup", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff9000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(-1.0 * 1.0) + HFLT_EPSILON/2 (RMM) -> nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fnmsub.h fa0, fa1, fa2, fa3, rmm", 0xffffffffffffbc00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(1.0 * 1.0) + -HFLT_EPSILON/2 (RMM) -> -nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fnmsub.h fa0, fa1, fa2, fa3, rmm", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff9000, 0x00, fa0, fa1, fa2,
                  fa3);

   /* -(-1.0 * 1.0) + HFLT_EPSILON/2 (DYN-RNE) -> 1.0 (NX) */
   TESTINST_1_3_F(4, "fnmsub.h fa0, fa1, fa2, fa3", 0xffffffffffffbc00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(-1.0 * nextafterf(1.0)) + HFLT_EPSILON/2 (DYN-RNE) ->
      2nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fnmsub.h fa0, fa1, fa2, fa3", 0xffffffffffffbc00,
                  0xffffffffffff3bff, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(-1.0 * 1.0) + HFLT_EPSILON/2 (DYN-RTZ) -> 1.0 (NX) */
   TESTINST_1_3_F(4, "fnmsub.h fa0, fa1, fa2, fa3", 0xffffffffffffbc00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x20, fa0, fa1, fa2,
                  fa3);
   /* -(1.0 * 1.0) + -HFLT_EPSILON/2 (DYN-RTZ) -> -1.0 (NX) */
   TESTINST_1_3_F(4, "fnmsub.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff9000, 0x20, fa0, fa1, fa2,
                  fa3);
   /* -(-1.0 * 1.0) + HFLT_EPSILON/2 (DYN-RDN) -> 1.0 (NX) */
   TESTINST_1_3_F(4, "fnmsub.h fa0, fa1, fa2, fa3", 0xffffffffffffbc00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x40, fa0, fa1, fa2,
                  fa3);
   /* -(1.0 * 1.0) + -HFLT_EPSILON/2 (DYN-RDN) -> -nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fnmsub.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff9000, 0x40, fa0, fa1, fa2,
                  fa3);
   /* -(-1.0 * 1.0) + HFLT_EPSILON/2 (DYN-RUP) -> nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fnmsub.h fa0, fa1, fa2, fa3", 0xffffffffffffbc00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x60, fa0, fa1, fa2,
                  fa3);
   /* -(1.0 * 1.0) + -HFLT_EPSILON/2 (DYN-RUP) -> -1.0 (NX) */
   TESTINST_1_3_F(4, "fnmsub.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff9000, 0x60, fa0, fa1, fa2,
                  fa3);
   /* -(-1.0 * 1.0) + HFLT_EPSILON/2 (DYN-RMM) -> nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fnmsub.h fa0, fa1, fa2, fa3", 0xffffffffffffbc00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x80, fa0, fa1, fa2,
                  fa3);
   /* -(1.0 * 1.0) + -HFLT_EPSILON/2 (DYN-RMM) -> -nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fnmsub.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff9000, 0x80, fa0, fa1, fa2,
                  fa3);

   /* ----------- fnmadd.h rd, rs1, rs2, rs3, rm ------------ */
   /* -(3.0 * 2.0) - 1.0 -> -7.0 */
   TESTINST_1_3_F(4, "fnmadd.h fa0, fa1, fa2, fa3", 0xffffffffffff4200,
                  0xffffffffffff4000, 0xffffffffffff3c00, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(1.0 * 1.0) - -1.0 -> 0.0 */
   TESTINST_1_3_F(4, "fnmadd.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffffbc00, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(2.0 * HFLT_TRUE_MIN) - -HFLT_TRUE_MIN -> -HFLT_TRUE_MIN (no UF because
      exact) */
   TESTINST_1_3_F(4, "fnmadd.h fa0, fa1, fa2, fa3", 0xffffffffffff4000,
                  0xffffffffffff0001, 0xffffffffffff8001, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(2.0 * FLT_MAX) - -FLT_MAX -> -FLT_MAX */
   TESTINST_1_3_F(4, "fnmadd.h fa0, fa1, fa2, fa3", 0xffffffffffff4000,
                  0xffffffffffff7bff, 0xfffffffffffffbff, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(2.0 * FLT_MAX) - 0.0 -> -INFINITY (OF, NX) */
   TESTINST_1_3_F(4, "fnmadd.h fa0, fa1, fa2, fa3", 0xffffffffffff4000,
                  0xffffffffffff7bff, 0xffffffffffff0000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(2.0 * INFINITY) - -INFINITY -> qNAN (NV) */
   TESTINST_1_3_F(4, "fnmadd.h fa0, fa1, fa2, fa3", 0xffffffffffff4000,
                  0xffffffffffff7c00, 0xfffffffffffffc00, 0x00, fa0, fa1, fa2,
                  fa3);

   /* -(-1.0 * nextafterf(1.0)) - HFLT_EPSILON/2 (RNE) -> 1.0 (NX) */
   TESTINST_1_3_F(4, "fnmadd.h fa0, fa1, fa2, fa3, rne", 0xffffffffffffbc00,
                  0xffffffffffff3bff, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(-1.0 * 2nextafterf(1.0)) - HFLT_EPSILON/2 (RNE) -> 2nextafterf(1.0) (NX)
    */
   TESTINST_1_3_F(4, "fnmadd.h fa0, fa1, fa2, fa3, rne", 0xffffffffffffbc00,
                  0xffffffffffff3fff, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(-1.0 * nextafterf(1.0)) - HFLT_EPSILON/2 (RTZ) -> 1.0 (NX) */
   TESTINST_1_3_F(4, "fnmadd.h fa0, fa1, fa2, fa3, rtz", 0xffffffffffffbc00,
                  0xffffffffffff3bff, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(1.0 * 1.0) - HFLT_EPSILON/2 (RTZ) -> -1.0 (NX) */
   TESTINST_1_3_F(4, "fnmadd.h fa0, fa1, fa2, fa3, rtz", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(-1.0 * nextafterf(1.0)) - HFLT_EPSILON/2 (RDN) -> 1.0 (NX) */
   TESTINST_1_3_F(4, "fnmadd.h fa0, fa1, fa2, fa3, rdn", 0xffffffffffffbc00,
                  0xffffffffffff3bff, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(1.0 * 1.0) - HFLT_EPSILON/2 (RDN) -> -nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fnmadd.h fa0, fa1, fa2, fa3, rdn", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(-1.0 * nextafterf(1.0)) - HFLT_EPSILON/2 (RUP) -> nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fnmadd.h fa0, fa1, fa2, fa3, rup", 0xffffffffffffbc00,
                  0xffffffffffff3bff, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(1.0 * 1.0) - HFLT_EPSILON/2 (RUP) -> -1.0 (NX) */
   TESTINST_1_3_F(4, "fnmadd.h fa0, fa1, fa2, fa3, rup", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(-1.0 * nextafterf(1.0)) - HFLT_EPSILON/2 (RMM) -> nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fnmadd.h fa0, fa1, fa2, fa3, rmm", 0xffffffffffffbc00,
                  0xffffffffffff3bff, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(1.0 * 1.0) - HFLT_EPSILON/2 (RMM) -> -nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fnmadd.h fa0, fa1, fa2, fa3, rmm", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);

   /* -(-1.0 * nextafterf(1.0)) - HFLT_EPSILON/2 (DYN-RNE) -> 1.0 (NX) */
   TESTINST_1_3_F(4, "fnmadd.h fa0, fa1, fa2, fa3", 0xffffffffffffbc00,
                  0xffffffffffff3bff, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(-1.0 * 2nextafterf(1.0)) - HFLT_EPSILON/2 (DYN-RNE) -> 2nextafterf(1.0)
      (NX) */
   TESTINST_1_3_F(4, "fnmadd.h fa0, fa1, fa2, fa3", 0xffffffffffffbc00,
                  0xffffffffffff3fff, 0xffffffffffff1000, 0x00, fa0, fa1, fa2,
                  fa3);
   /* -(-1.0 * nextafterf(1.0)) - HFLT_EPSILON/2 (DYN-RTZ) -> 1.0 (NX) */
   TESTINST_1_3_F(4, "fnmadd.h fa0, fa1, fa2, fa3", 0xffffffffffffbc00,
                  0xffffffffffff3bff, 0xffffffffffff1000, 0x20, fa0, fa1, fa2,
                  fa3);
   /* -(1.0 * 1.0) - HFLT_EPSILON/2 (DYN-RTZ) -> -1.0 (NX) */
   TESTINST_1_3_F(4, "fnmadd.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x20, fa0, fa1, fa2,
                  fa3);
   /* -(-1.0 * nextafterf(1.0)) - HFLT_EPSILON/2 (DYN-RDN) -> 1.0 (NX) */
   TESTINST_1_3_F(4, "fnmadd.h fa0, fa1, fa2, fa3", 0xffffffffffffbc00,
                  0xffffffffffff3bff, 0xffffffffffff1000, 0x40, fa0, fa1, fa2,
                  fa3);
   /* -(1.0 * 1.0) - HFLT_EPSILON/2 (DYN-RDN) -> -nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fnmadd.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x40, fa0, fa1, fa2,
                  fa3);
   /* -(-1.0 * nextafterf(1.0)) - HFLT_EPSILON/2 (DYN-RUP) ->
      nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fnmadd.h fa0, fa1, fa2, fa3", 0xffffffffffffbc00,
                  0xffffffffffff3bff, 0xffffffffffff1000, 0x60, fa0, fa1, fa2,
                  fa3);
   /* -(1.0 * 1.0) - HFLT_EPSILON/2 (DYN-RUP) -> -1.0 (NX) */
   TESTINST_1_3_F(4, "fnmadd.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x60, fa0, fa1, fa2,
                  fa3);
   /* -(-1.0 * nextafterf(1.0)) - HFLT_EPSILON/2 (DYN-RMM) ->
      nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fnmadd.h fa0, fa1, fa2, fa3", 0xffffffffffffbc00,
                  0xffffffffffff3bff, 0xffffffffffff1000, 0x80, fa0, fa1, fa2,
                  fa3);
   /* -(1.0 * 1.0) - HFLT_EPSILON/2 (DYN-RMM) -> -nextafterf(1.0) (NX) */
   TESTINST_1_3_F(4, "fnmadd.h fa0, fa1, fa2, fa3", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0xffffffffffff1000, 0x80, fa0, fa1, fa2,
                  fa3);

   /* --------------- fadd.h rd, rs1, rs2, rm --------------- */
   /* 2.0 + 1.0 -> 3.0 */
   TESTINST_1_2_F(4, "fadd.h fa0, fa1, fa2", 0xffffffffffff4000,
                  0xffffffffffff3c00, 0x00, fa0, fa1, fa2);
   /* 1.0 + -1.0 -> 0.0 */
   TESTINST_1_2_F(4, "fadd.h fa0, fa1, fa2", 0xffffffffffff3c00,
                  0xffffffffffffbc00, 0x00, fa0, fa1, fa2);
   /* HFLT_TRUE_MIN + HFLT_TRUE_MIN -> 2*HFLT_TRUE_MIN (no UF because exact) */
   TESTINST_1_2_F(4, "fadd.h fa0, fa1, fa2", 0xffffffffffff0001,
                  0xffffffffffff0001, 0x00, fa0, fa1, fa2);
   /* FLT_MAX + FLT_MAX -> INFINITY (OF, NX) */
   TESTINST_1_2_F(4, "fadd.h fa0, fa1, fa2", 0xffffffffffff7bff,
                  0xffffffffffff7bff, 0x00, fa0, fa1, fa2);
   /* -FLT_MAX + -FLT_MAX -> -INFINITY (OF, NX) */
   TESTINST_1_2_F(4, "fadd.h fa0, fa1, fa2", 0xfffffffffffffbff,
                  0xfffffffffffffbff, 0x00, fa0, fa1, fa2);
   /* nextafterf(FLT_MIN) + -FLT_MIN -> HFLT_TRUE_MIN (no UF because exact) */
   TESTINST_1_2_F(4, "fadd.h fa0, fa1, fa2", 0xffffffffffff0401,
                  0xffffffffffff8400, 0x00, fa0, fa1, fa2);
   /* INFINITY + -INFINITY -> qNAN (NV) */
   TESTINST_1_2_F(4, "fadd.h fa0, fa1, fa2", 0xffffffffffff7c00,
                  0xfffffffffffffc00, 0x00, fa0, fa1, fa2);

   /* 1.0 + HFLT_EPSILON/2 (RNE) -> 1.0 (NX) */
   TESTINST_1_2_F(4, "fadd.h fa0, fa1, fa2, rne", 0xffffffffffff3c00,
                  0xffffffffffff1000, 0x00, fa0, fa1, fa2);
   /* nextafterf(1.0) + HFLT_EPSILON/2 (RNE) -> 2nextafterf(1.0) (NX) */
   TESTINST_1_2_F(4, "fadd.h fa0, fa1, fa2, rne", 0xffffffffffff3bff,
                  0xffffffffffff1000, 0x00, fa0, fa1, fa2);
   /* 1.0 + HFLT_EPSILON/2 (RTZ) -> 1.0 (NX) */
   TESTINST_1_2_F(4, "fadd.h fa0, fa1, fa2, rtz", 0xffffffffffff3c00,
                  0xffffffffffff1000, 0x00, fa0, fa1, fa2);
   /* -1.0 + -HFLT_EPSILON/2 (RTZ) -> -1.0 (NX) */
   TESTINST_1_2_F(4, "fadd.h fa0, fa1, fa2, rtz", 0xffffffffffffbc00,
                  0xffffffffffff9000, 0x00, fa0, fa1, fa2);
   /* 1.0 + HFLT_EPSILON/2 (RDN) -> 1.0 (NX) */
   TESTINST_1_2_F(4, "fadd.h fa0, fa1, fa2, rdn", 0xffffffffffff3c00,
                  0xffffffffffff1000, 0x00, fa0, fa1, fa2);
   /* -1.0 + -HFLT_EPSILON/2 (RDN) -> -nextafterf(1.0) (NX) */
   TESTINST_1_2_F(4, "fadd.h fa0, fa1, fa2, rdn", 0xffffffffffffbc00,
                  0xffffffffffff9000, 0x00, fa0, fa1, fa2);
   /* 1.0 + HFLT_EPSILON/2 (RUP) -> nextafterf(1.0) (NX) */
   TESTINST_1_2_F(4, "fadd.h fa0, fa1, fa2, rup", 0xffffffffffff3c00,
                  0xffffffffffff1000, 0x00, fa0, fa1, fa2);
   /* -1.0 + -HFLT_EPSILON/2 (RUP) -> -1.0 (NX) */
   TESTINST_1_2_F(4, "fadd.h fa0, fa1, fa2, rup", 0xffffffffffffbc00,
                  0xffffffffffff9000, 0x00, fa0, fa1, fa2);
   /* 1.0 + HFLT_EPSILON/2 (RMM) -> nextafterf(1.0) (NX) */
   TESTINST_1_2_F(4, "fadd.h fa0, fa1, fa2, rmm", 0xffffffffffff3c00,
                  0xffffffffffff1000, 0x00, fa0, fa1, fa2);
   /* -1.0 + -HFLT_EPSILON/2 (RMM) -> -nextafterf(1.0) (NX) */
   TESTINST_1_2_F(4, "fadd.h fa0, fa1, fa2, rmm", 0xffffffffffffbc00,
                  0xffffffffffff9000, 0x00, fa0, fa1, fa2);

   /* 1.0 + HFLT_EPSILON/2 (DYN-RNE) -> 1.0 (NX) */
   TESTINST_1_2_F(4, "fadd.h fa0, fa1, fa2", 0xffffffffffff3c00,
                  0xffffffffffff1000, 0x00, fa0, fa1, fa2);
   /* nextafterf(1.0) + HFLT_EPSILON/2 (DYN-RNE) -> 2nextafterf(1.0) (NX) */
   TESTINST_1_2_F(4, "fadd.h fa0, fa1, fa2", 0xffffffffffff3bff,
                  0xffffffffffff1000, 0x00, fa0, fa1, fa2);
   /* 1.0 + HFLT_EPSILON/2 (DYN-RTZ) -> 1.0 (NX) */
   TESTINST_1_2_F(4, "fadd.h fa0, fa1, fa2", 0xffffffffffff3c00,
                  0xffffffffffff1000, 0x20, fa0, fa1, fa2);
   /* -1.0 + -HFLT_EPSILON/2 (DYN-RTZ) -> -1.0 (NX) */
   TESTINST_1_2_F(4, "fadd.h fa0, fa1, fa2", 0xffffffffffffbc00,
                  0xffffffffffff9000, 0x20, fa0, fa1, fa2);
   /* 1.0 + HFLT_EPSILON/2 (DYN-RDN) -> 1.0 (NX) */
   TESTINST_1_2_F(4, "fadd.h fa0, fa1, fa2", 0xffffffffffff3c00,
                  0xffffffffffff1000, 0x40, fa0, fa1, fa2);
   /* -1.0 + -HFLT_EPSILON/2 (DYN-RDN) -> -nextafterf(1.0) (NX) */
   TESTINST_1_2_F(4, "fadd.h fa0, fa1, fa2", 0xffffffffffffbc00,
                  0xffffffffffff9000, 0x40, fa0, fa1, fa2);
   /* 1.0 + HFLT_EPSILON/2 (DYN-RUP) -> nextafterf(1.0) (NX) */
   TESTINST_1_2_F(4, "fadd.h fa0, fa1, fa2", 0xffffffffffff3c00,
                  0xffffffffffff1000, 0x60, fa0, fa1, fa2);
   /* -1.0 + -HFLT_EPSILON/2 (DYN-RUP) -> -1.0 (NX) */
   TESTINST_1_2_F(4, "fadd.h fa0, fa1, fa2", 0xffffffffffffbc00,
                  0xffffffffffff9000, 0x60, fa0, fa1, fa2);
   /* 1.0 + HFLT_EPSILON/2 (DYN-RMM) -> nextafterf(1.0) (NX) */
   TESTINST_1_2_F(4, "fadd.h fa0, fa1, fa2", 0xffffffffffff3c00,
                  0xffffffffffff1000, 0x80, fa0, fa1, fa2);
   /* -1.0 + -HFLT_EPSILON/2 (DYN-RMM) -> -nextafterf(1.0) (NX) */
   TESTINST_1_2_F(4, "fadd.h fa0, fa1, fa2", 0xffffffffffffbc00,
                  0xffffffffffff9000, 0x80, fa0, fa1, fa2);

   /* --------------- fsub.h rd, rs1, rs2, rm --------------- */
   /* 2.0 - 1.0 -> 1.0 */
   TESTINST_1_2_F(4, "fsub.h fa0, fa1, fa2", 0xffffffffffff4000,
                  0xffffffffffff3c00, 0x00, fa0, fa1, fa2);
   /* 1.0 - 1.0 -> 0.0 */
   TESTINST_1_2_F(4, "fsub.h fa0, fa1, fa2", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0x00, fa0, fa1, fa2);
   /* HFLT_TRUE_MIN - -HFLT_TRUE_MIN -> 2*HFLT_TRUE_MIN (no UF because exact) */
   TESTINST_1_2_F(4, "fsub.h fa0, fa1, fa2", 0xffffffffffff0001,
                  0xffffffffffff8001, 0x00, fa0, fa1, fa2);
   /* FLT_MAX - -FLT_MAX -> INFINITY (OF, NX) */
   TESTINST_1_2_F(4, "fsub.h fa0, fa1, fa2", 0xffffffffffff7bff,
                  0xfffffffffffffbff, 0x00, fa0, fa1, fa2);
   /* -FLT_MAX - FLT_MAX -> -INFINITY (OF, NX) */
   TESTINST_1_2_F(4, "fsub.h fa0, fa1, fa2", 0xfffffffffffffbff,
                  0xffffffffffff7bff, 0x00, fa0, fa1, fa2);
   /* nextafterf(FLT_MIN) - FLT_MIN -> HFLT_TRUE_MIN (no UF because exact) */
   TESTINST_1_2_F(4, "fsub.h fa0, fa1, fa2", 0xffffffffffff0401,
                  0xffffffffffff0400, 0x00, fa0, fa1, fa2);
   /* INFINITY - INFINITY -> qNAN (NV) */
   TESTINST_1_2_F(4, "fsub.h fa0, fa1, fa2", 0xffffffffffff7c00,
                  0xffffffffffff7c00, 0x00, fa0, fa1, fa2);

   /* nextafterf(1.0) - HFLT_EPSILON/2 (RNE) -> 1.0 (NX) */
   TESTINST_1_2_F(4, "fsub.h fa0, fa1, fa2, rne", 0xffffffffffff3bff,
                  0xffffffffffff1000, 0x00, fa0, fa1, fa2);
   /* 2nextafterf(1.0) - HFLT_EPSILON/2 (RNE) -> 2nextafterf(1.0) (NX) */
   TESTINST_1_2_F(4, "fsub.h fa0, fa1, fa2, rne", 0xffffffffffff3fff,
                  0xffffffffffff1000, 0x00, fa0, fa1, fa2);
   /* nextafterf(1.0) - HFLT_EPSILON/2 (RTZ) -> 1.0 (NX) */
   TESTINST_1_2_F(4, "fsub.h fa0, fa1, fa2, rtz", 0xffffffffffff3bff,
                  0xffffffffffff1000, 0x00, fa0, fa1, fa2);
   /* -1.0 - HFLT_EPSILON/2 (RTZ) -> -1.0 (NX) */
   TESTINST_1_2_F(4, "fsub.h fa0, fa1, fa2, rtz", 0xffffffffffffbc00,
                  0xffffffffffff1000, 0x00, fa0, fa1, fa2);
   /* nextafterf(1.0) - HFLT_EPSILON/2 (RDN) -> 1.0 (NX) */
   TESTINST_1_2_F(4, "fsub.h fa0, fa1, fa2, rdn", 0xffffffffffff3bff,
                  0xffffffffffff1000, 0x00, fa0, fa1, fa2);
   /* -1.0 - HFLT_EPSILON/2 (RDN) -> -nextafterf(1.0) (NX) */
   TESTINST_1_2_F(4, "fsub.h fa0, fa1, fa2, rdn", 0xffffffffffffbc00,
                  0xffffffffffff1000, 0x00, fa0, fa1, fa2);
   /* nextafterf(1.0) - HFLT_EPSILON/2 (RUP) -> nextafterf(1.0) (NX) */
   TESTINST_1_2_F(4, "fsub.h fa0, fa1, fa2, rup", 0xffffffffffff3bff,
                  0xffffffffffff1000, 0x00, fa0, fa1, fa2);
   /* -1.0 - HFLT_EPSILON/2 (RUP) -> -1.0 (NX) */
   TESTINST_1_2_F(4, "fsub.h fa0, fa1, fa2, rup", 0xffffffffffffbc00,
                  0xffffffffffff1000, 0x00, fa0, fa1, fa2);
   /* nextafterf(1.0) - HFLT_EPSILON/2 (RMM) -> nextafterf(1.0) (NX) */
   TESTINST_1_2_F(4, "fsub.h fa0, fa1, fa2, rmm", 0xffffffffffff3bff,
                  0xffffffffffff1000, 0x00, fa0, fa1, fa2);
   /* -1.0 - HFLT_EPSILON/2 (RMM) -> -nextafterf(1.0) (NX) */
   TESTINST_1_2_F(4, "fsub.h fa0, fa1, fa2, rmm", 0xffffffffffffbc00,
                  0xffffffffffff1000, 0x00, fa0, fa1, fa2);

   /* nextafterf(1.0) - HFLT_EPSILON/2 (DYN-RNE) -> 1.0 (NX) */
   TESTINST_1_2_F(4, "fsub.h fa0, fa1, fa2", 0xffffffffffff3bff,
                  0xffffffffffff1000, 0x00, fa0, fa1, fa2);
   /* 2nextafterf(1.0) - HFLT_EPSILON/2 (DYN-RNE) -> 2nextafterf(1.0) (NX) */
   TESTINST_1_2_F(4, "fsub.h fa0, fa1, fa2", 0xffffffffffff3fff,
                  0xffffffffffff1000, 0x00, fa0, fa1, fa2);
   /* nextafterf(1.0) - HFLT_EPSILON/2 (DYN-RTZ) -> 1.0 (NX) */
   TESTINST_1_2_F(4, "fsub.h fa0, fa1, fa2", 0xffffffffffff3bff,
                  0xffffffffffff1000, 0x20, fa0, fa1, fa2);
   /* -1.0 - HFLT_EPSILON/2 (DYN-RTZ) -> -1.0 (NX) */
   TESTINST_1_2_F(4, "fsub.h fa0, fa1, fa2", 0xffffffffffffbc00,
                  0xffffffffffff1000, 0x20, fa0, fa1, fa2);
   /* nextafterf(1.0) - HFLT_EPSILON/2 (DYN-RDN) -> 1.0 (NX) */
   TESTINST_1_2_F(4, "fsub.h fa0, fa1, fa2", 0xffffffffffff3bff,
                  0xffffffffffff1000, 0x40, fa0, fa1, fa2);
   /* -1.0 - HFLT_EPSILON/2 (DYN-RDN) -> -nextafterf(1.0) (NX) */
   TESTINST_1_2_F(4, "fsub.h fa0, fa1, fa2", 0xffffffffffffbc00,
                  0xffffffffffff1000, 0x40, fa0, fa1, fa2);
   /* nextafterf(1.0) - HFLT_EPSILON/2 (DYN-RUP) -> nextafterf(1.0) (NX) */
   TESTINST_1_2_F(4, "fsub.h fa0, fa1, fa2", 0xffffffffffff3bff,
                  0xffffffffffff1000, 0x60, fa0, fa1, fa2);
   /* -1.0 - HFLT_EPSILON/2 (DYN-RUP) -> -1.0 (NX) */
   TESTINST_1_2_F(4, "fsub.h fa0, fa1, fa2", 0xffffffffffffbc00,
                  0xffffffffffff1000, 0x60, fa0, fa1, fa2);
   /* nextafterf(1.0) - HFLT_EPSILON/2 (DYN-RMM) -> nextafterf(1.0) (NX) */
   TESTINST_1_2_F(4, "fsub.h fa0, fa1, fa2", 0xffffffffffff3bff,
                  0xffffffffffff1000, 0x80, fa0, fa1, fa2);
   /* -1.0 - HFLT_EPSILON/2 (DYN-RMM) -> -nextafterf(1.0) (NX) */
   TESTINST_1_2_F(4, "fsub.h fa0, fa1, fa2", 0xffffffffffffbc00,
                  0xffffffffffff1000, 0x80, fa0, fa1, fa2);

   /* --------------- fmul.h rd, rs1, rs2, rm --------------- */
   /* 2.0 * 1.0 -> 2.0 */
   TESTINST_1_2_F(4, "fmul.h fa0, fa1, fa2", 0xffffffffffff4000,
                  0xffffffffffff3c00, 0x00, fa0, fa1, fa2);
   /* 1.0 * 0.0 -> 0.0 */
   TESTINST_1_2_F(4, "fmul.h fa0, fa1, fa2", 0xffffffffffff3c00,
                  0xffffffffffff0000, 0x00, fa0, fa1, fa2);
   /* 2**-12 * 2**-12 -> 2**-149 aka HFLT_TRUE_MIN (no UF because exact) */
   TESTINST_1_2_F(4, "fmul.h fa0, fa1, fa2", 0xffffffffffff0c00,
                  0xffffffffffff0c00, 0x00, fa0, fa1, fa2);
   /* FLT_MAX * FLT_MAX -> INFINITY (OF, NX) */
   TESTINST_1_2_F(4, "fmul.h fa0, fa1, fa2", 0xffffffffffff7bff,
                  0xffffffffffff7bff, 0x00, fa0, fa1, fa2);
   /* FLT_MAX * -FLT_MAX -> -INFINITY (OF, NX) */
   TESTINST_1_2_F(4, "fmul.h fa0, fa1, fa2", 0xffffffffffff7bff,
                  0xfffffffffffffbff, 0x00, fa0, fa1, fa2);
   /* 1.0 * INFINITY -> INFINITY */
   TESTINST_1_2_F(4, "fmul.h fa0, fa1, fa2", 0xffffffffffff3c00,
                  0xffffffffffff7c00, 0x00, fa0, fa1, fa2);
   /* 0.0 * INFINITY -> qNAN (NV) */
   TESTINST_1_2_F(4, "fmul.h fa0, fa1, fa2", 0xffffffffffff0000,
                  0xffffffffffff7c00, 0x00, fa0, fa1, fa2);

   /* HFLT_TRUE_MIN * 0.5 (RNE) -> 0.0 (UF, NX) */
   TESTINST_1_2_F(4, "fmul.h fa0, fa1, fa2, rne", 0xffffffffffff0001,
                  0xffffffffffff3800, 0x00, fa0, fa1, fa2);
   /* 3*HFLT_TRUE_MIN * 0.5 (RNE) -> 2*HFLT_TRUE_MIN (UF, NX) */
   TESTINST_1_2_F(4, "fmul.h fa0, fa1, fa2, rne", 0xffffffffffff0003,
                  0xffffffffffff3800, 0x00, fa0, fa1, fa2);
   /* HFLT_TRUE_MIN * 0.5 (RTZ) -> 0.0 (UF, NX) */
   TESTINST_1_2_F(4, "fmul.h fa0, fa1, fa2, rtz", 0xffffffffffff0001,
                  0xffffffffffff3800, 0x00, fa0, fa1, fa2);
   /* -HFLT_TRUE_MIN * 0.5 (RTZ) -> -0.0 (UF, NX) */
   TESTINST_1_2_F(4, "fmul.h fa0, fa1, fa2, rtz", 0xffffffffffff8001,
                  0xffffffffffff3800, 0x00, fa0, fa1, fa2);
   /* HFLT_TRUE_MIN * 0.5 (RDN) -> 0.0 (UF, NX) */
   TESTINST_1_2_F(4, "fmul.h fa0, fa1, fa2, rdn", 0xffffffffffff0001,
                  0xffffffffffff3800, 0x00, fa0, fa1, fa2);
   /* -HFLT_TRUE_MIN * 0.5 (RDN) -> -HFLT_TRUE_MIN (UF, NX) */
   TESTINST_1_2_F(4, "fmul.h fa0, fa1, fa2, rdn", 0xffffffffffff8001,
                  0xffffffffffff3800, 0x00, fa0, fa1, fa2);
   /* HFLT_TRUE_MIN * 0.5 (RUP) -> HFLT_TRUE_MIN (UF, NX) */
   TESTINST_1_2_F(4, "fmul.h fa0, fa1, fa2, rup", 0xffffffffffff0001,
                  0xffffffffffff3800, 0x00, fa0, fa1, fa2);
   /* -HFLT_TRUE_MIN * 0.5 (RUP) -> -0.0 (UF, NX) */
   TESTINST_1_2_F(4, "fmul.h fa0, fa1, fa2, rup", 0xffffffffffff8001,
                  0xffffffffffff3800, 0x00, fa0, fa1, fa2);
   /* HFLT_TRUE_MIN * 0.5 (RMM) -> HFLT_TRUE_MIN (UF, NX) */
   TESTINST_1_2_F(4, "fmul.h fa0, fa1, fa2, rmm", 0xffffffffffff0001,
                  0xffffffffffff3800, 0x00, fa0, fa1, fa2);
   /* -HFLT_TRUE_MIN * 0.5 (RMM) -> -HFLT_TRUE_MIN (UF, NX) */
   TESTINST_1_2_F(4, "fmul.h fa0, fa1, fa2, rmm", 0xffffffffffff8001,
                  0xffffffffffff3800, 0x00, fa0, fa1, fa2);

   /* HFLT_TRUE_MIN * 0.5 (DYN-RNE) -> 0.0 (UF, NX) */
   TESTINST_1_2_F(4, "fmul.h fa0, fa1, fa2", 0xffffffffffff0001,
                  0xffffffffffff3800, 0x00, fa0, fa1, fa2);
   /* 3*HFLT_TRUE_MIN * 0.5 (DYN-RNE) -> 2*HFLT_TRUE_MIN (UF, NX) */
   TESTINST_1_2_F(4, "fmul.h fa0, fa1, fa2", 0xffffffffffff0003,
                  0xffffffffffff3800, 0x00, fa0, fa1, fa2);
   /* HFLT_TRUE_MIN * 0.5 (DYN-RTZ) -> 0.0 (UF, NX) */
   TESTINST_1_2_F(4, "fmul.h fa0, fa1, fa2", 0xffffffffffff0001,
                  0xffffffffffff3800, 0x20, fa0, fa1, fa2);
   /* -HFLT_TRUE_MIN * 0.5 (DYN-RTZ) -> -0.0 (UF, NX) */
   TESTINST_1_2_F(4, "fmul.h fa0, fa1, fa2", 0xffffffffffff8001,
                  0xffffffffffff3800, 0x20, fa0, fa1, fa2);
   /* HFLT_TRUE_MIN * 0.5 (DYN-RDN) -> 0.0 (UF, NX) */
   TESTINST_1_2_F(4, "fmul.h fa0, fa1, fa2", 0xffffffffffff0001,
                  0xffffffffffff3800, 0x40, fa0, fa1, fa2);
   /* -HFLT_TRUE_MIN * 0.5 (DYN-RDN) -> -HFLT_TRUE_MIN (UF, NX) */
   TESTINST_1_2_F(4, "fmul.h fa0, fa1, fa2", 0xffffffffffff8001,
                  0xffffffffffff3800, 0x40, fa0, fa1, fa2);
   /* HFLT_TRUE_MIN * 0.5 (DYN-RUP) -> HFLT_TRUE_MIN (UF, NX) */
   TESTINST_1_2_F(4, "fmul.h fa0, fa1, fa2", 0xffffffffffff0001,
                  0xffffffffffff3800, 0x60, fa0, fa1, fa2);
   /* -HFLT_TRUE_MIN * 0.5 (DYN-RUP) -> -0.0 (UF, NX) */
   TESTINST_1_2_F(4, "fmul.h fa0, fa1, fa2", 0xffffffffffff8001,
                  0xffffffffffff3800, 0x60, fa0, fa1, fa2);
   /* HFLT_TRUE_MIN * 0.5 (DYN-RMM) -> HFLT_TRUE_MIN (UF, NX) */
   TESTINST_1_2_F(4, "fmul.h fa0, fa1, fa2", 0xffffffffffff0001,
                  0xffffffffffff3800, 0x80, fa0, fa1, fa2);
   /* -HFLT_TRUE_MIN * 0.5 (DYN-RMM) -> -HFLT_TRUE_MIN (UF, NX) */
   TESTINST_1_2_F(4, "fmul.h fa0, fa1, fa2", 0xffffffffffff8001,
                  0xffffffffffff3800, 0x80, fa0, fa1, fa2);

   /* --------------- fdiv.h rd, rs1, rs2, rm --------------- */
   /* 2.0 / 1.0 -> 2.0 */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2", 0xffffffffffff4000,
                  0xffffffffffff3c00, 0x00, fa0, fa1, fa2);
   /* 0.0 / 1.0 -> 0.0 */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2", 0xffffffffffff0000,
                  0xffffffffffff3c00, 0x00, fa0, fa1, fa2);
   /* 1.0 / 2**15 -> 2**-15 (no UF because exact) */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2", 0xffffffffffff3c00,
                  0xffffffffffff7800, 0x00, fa0, fa1, fa2);
   /* FLT_MAX / 0.5 -> INFINITY (OF, NX) */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2", 0xffffffffffff7bff,
                  0xffffffffffff3800, 0x00, fa0, fa1, fa2);
   /* FLT_MAX / -0.5 -> -INFINITY (OF, NX) */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2", 0xffffffffffff7bff,
                  0xffffffffffffb800, 0x00, fa0, fa1, fa2);
   /* 1.0 / INFINITY -> 0.0 */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2", 0xffffffffffff3c00,
                  0xffffffffffff7c00, 0x00, fa0, fa1, fa2);
   /* 1.0 / 0.0 -> INFINITY (DZ) */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2", 0xffffffffffff3c00,
                  0xffffffffffff0000, 0x00, fa0, fa1, fa2);
   /* 0.0 / 0.0 -> qNAN (NV) */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2", 0xffffffffffff0000,
                  0xffffffffffff0000, 0x00, fa0, fa1, fa2);

   /* HFLT_TRUE_MIN / 2.0 (RNE) -> 0.0 (UF, NX) */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2, rne", 0xffffffffffff0001,
                  0xffffffffffff4000, 0x00, fa0, fa1, fa2);
   /* 3*HFLT_TRUE_MIN / 2.0 (RNE) -> 2*HFLT_TRUE_MIN (UF, NX) */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2, rne", 0xffffffffffff0003,
                  0xffffffffffff4000, 0x00, fa0, fa1, fa2);
   /* HFLT_TRUE_MIN / 2.0 (RTZ) -> 0.0 (UF, NX) */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2, rtz", 0xffffffffffff0001,
                  0xffffffffffff4000, 0x00, fa0, fa1, fa2);
   /* -HFLT_TRUE_MIN / 2.0 (RTZ) -> -0.0 (UF, NX) */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2, rtz", 0xffffffffffff8001,
                  0xffffffffffff4000, 0x00, fa0, fa1, fa2);
   /* HFLT_TRUE_MIN / 2.0 (RDN) -> 0.0 (UF, NX) */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2, rdn", 0xffffffffffff0001,
                  0xffffffffffff4000, 0x00, fa0, fa1, fa2);
   /* -HFLT_TRUE_MIN / 2.0 (RDN) -> -HFLT_TRUE_MIN (UF, NX) */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2, rdn", 0xffffffffffff8001,
                  0xffffffffffff4000, 0x00, fa0, fa1, fa2);
   /* HFLT_TRUE_MIN / 2.0 (RUP) -> HFLT_TRUE_MIN (UF, NX) */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2, rup", 0xffffffffffff0001,
                  0xffffffffffff4000, 0x00, fa0, fa1, fa2);
   /* -HFLT_TRUE_MIN / 2.0 (RUP) -> -0.0 (UF, NX) */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2, rup", 0xffffffffffff8001,
                  0xffffffffffff4000, 0x00, fa0, fa1, fa2);
   /* HFLT_TRUE_MIN / 2.0 (RMM) -> HFLT_TRUE_MIN (UF, NX) */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2, rmm", 0xffffffffffff0001,
                  0xffffffffffff4000, 0x00, fa0, fa1, fa2);
   /* -HFLT_TRUE_MIN / 2.0 (RMM) -> -HFLT_TRUE_MIN (UF, NX) */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2, rmm", 0xffffffffffff8001,
                  0xffffffffffff4000, 0x00, fa0, fa1, fa2);

   /* HFLT_TRUE_MIN / 2.0 (DYN-RNE) -> 0.0 (UF, NX) */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2", 0xffffffffffff0001,
                  0xffffffffffff4000, 0x00, fa0, fa1, fa2);
   /* 3*HFLT_TRUE_MIN / 2.0 (DYN-RNE) -> 2*HFLT_TRUE_MIN (UF, NX) */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2", 0xffffffffffff0003,
                  0xffffffffffff4000, 0x00, fa0, fa1, fa2);
   /* HFLT_TRUE_MIN / 2.0 (DYN-RTZ) -> 0.0 (UF, NX) */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2", 0xffffffffffff0001,
                  0xffffffffffff4000, 0x20, fa0, fa1, fa2);
   /* -HFLT_TRUE_MIN / 2.0 (DYN-RTZ) -> -0.0 (UF, NX) */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2", 0xffffffffffff8001,
                  0xffffffffffff4000, 0x20, fa0, fa1, fa2);
   /* HFLT_TRUE_MIN / 2.0 (DYN-RDN) -> 0.0 (UF, NX) */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2", 0xffffffffffff0001,
                  0xffffffffffff4000, 0x40, fa0, fa1, fa2);
   /* -HFLT_TRUE_MIN / 2.0 (DYN-RDN) -> -HFLT_TRUE_MIN (UF, NX) */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2", 0xffffffffffff8001,
                  0xffffffffffff4000, 0x40, fa0, fa1, fa2);
   /* HFLT_TRUE_MIN / 2.0 (DYN-RUP) -> HFLT_TRUE_MIN (UF, NX) */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2", 0xffffffffffff0001,
                  0xffffffffffff4000, 0x60, fa0, fa1, fa2);
   /* -HFLT_TRUE_MIN / 2.0 (DYN-RUP) -> -0.0 (UF, NX) */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2", 0xffffffffffff8001,
                  0xffffffffffff4000, 0x60, fa0, fa1, fa2);
   /* HFLT_TRUE_MIN / 2.0 (DYN-RMM) -> HFLT_TRUE_MIN (UF, NX) */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2", 0xffffffffffff0001,
                  0xffffffffffff4000, 0x80, fa0, fa1, fa2);
   /* -HFLT_TRUE_MIN / 2.0 (DYN-RMM) -> -HFLT_TRUE_MIN (UF, NX) */
   TESTINST_1_2_F(4, "fdiv.h fa0, fa1, fa2", 0xffffffffffff8001,
                  0xffffffffffff4000, 0x80, fa0, fa1, fa2);

   /* ----------------- fsqrt.h rd, rs1, rm ----------------- */
   /* sqrt(0.0) -> 0.0 */
   TESTINST_1_1_F(4, "fsqrt.h fa0, fa1", 0xffffffffffff0000, 0x00, fa0, fa1);
   /* sqrt(INFINITY) -> INFINITY */
   TESTINST_1_1_F(4, "fsqrt.h fa0, fa1", 0xffffffffffff7c00, 0x00, fa0, fa1);
   /* sqrt(2*HFLT_TRUE_MIN) -> 2**-74 */
   TESTINST_1_1_F(4, "fsqrt.h fa0, fa1", 0xffffffffffff0002, 0x00, fa0, fa1);
   /* sqrt(qNAN) -> qNAN */
   TESTINST_1_1_F(4, "fsqrt.h fa0, fa1", 0xffffffffffff7e00, 0x00, fa0, fa1);
   /* sqrt(-1.0) -> qNAN (NV) */
   TESTINST_1_1_F(4, "fsqrt.h fa0, fa1", 0xffffffffffffbc00, 0x00, fa0, fa1);

   /* sqrt(nextafterf(1.0)) (RNE) -> 1.0 (NX) */
   TESTINST_1_1_F(4, "fsqrt.h fa0, fa1, rne", 0xffffffffffff3bff, 0x00, fa0,
                  fa1);
   /* sqrt(2nextafterf(1.0)) (RNE) -> nextafterf(1.0) (NX) */
   TESTINST_1_1_F(4, "fsqrt.h fa0, fa1, rne", 0xffffffffffff3fff, 0x00, fa0,
                  fa1);
   /* sqrt(nextafterf(1.0)) (RTZ) -> 1.0 (NX) */
   TESTINST_1_1_F(4, "fsqrt.h fa0, fa1, rtz", 0xffffffffffff3bff, 0x00, fa0,
                  fa1);
   /* sqrt(2nextafterf(1.0)) (RTZ) -> 1.0 (NX) */
   TESTINST_1_1_F(4, "fsqrt.h fa0, fa1, rtz", 0xffffffffffff3fff, 0x00, fa0,
                  fa1);
   /* sqrt(nextafterf(1.0)) (RDN) -> 1.0 (NX) */
   TESTINST_1_1_F(4, "fsqrt.h fa0, fa1, rdn", 0xffffffffffff3bff, 0x00, fa0,
                  fa1);
   /* sqrt(2nextafterf(1.0)) (RDN) -> 1.0 (NX) */
   TESTINST_1_1_F(4, "fsqrt.h fa0, fa1, rdn", 0xffffffffffff3fff, 0x00, fa0,
                  fa1);
   /* sqrt(nextafterf(1.0)) (RUP) -> nextafterf(1.0) (NX) */
   TESTINST_1_1_F(4, "fsqrt.h fa0, fa1, rup", 0xffffffffffff3bff, 0x00, fa0,
                  fa1);
   /* sqrt(2nextafterf(1.0)) (RUP) -> nextafterf(1.0) (NX) */
   TESTINST_1_1_F(4, "fsqrt.h fa0, fa1, rup", 0xffffffffffff3fff, 0x00, fa0,
                  fa1);
   /* sqrt(nextafterf(1.0)) (RMM) -> 1.0 (NX) */
   TESTINST_1_1_F(4, "fsqrt.h fa0, fa1, rmm", 0xffffffffffff3bff, 0x00, fa0,
                  fa1);
   /* sqrt(2nextafterf(1.0)) (RMM) -> nextafterf(1.0) (NX) */
   TESTINST_1_1_F(4, "fsqrt.h fa0, fa1, rmm", 0xffffffffffff3fff, 0x00, fa0,
                  fa1);

   /* sqrt(nextafterf(1.0)) (DYN-RNE) -> 1.0 (NX) */
   TESTINST_1_1_F(4, "fsqrt.h fa0, fa1", 0xffffffffffff3bff, 0x00, fa0, fa1);
   /* sqrt(2nextafterf(1.0)) (DYN-RNE) -> nextafterf(1.0) (NX) */
   TESTINST_1_1_F(4, "fsqrt.h fa0, fa1", 0xffffffffffff3fff, 0x00, fa0, fa1);
   /* sqrt(nextafterf(1.0)) (DYN-RTZ) -> 1.0 (NX) */
   TESTINST_1_1_F(4, "fsqrt.h fa0, fa1", 0xffffffffffff3bff, 0x20, fa0, fa1);
   /* sqrt(2nextafterf(1.0)) (DYN-RTZ) -> 1.0 (NX) */
   TESTINST_1_1_F(4, "fsqrt.h fa0, fa1", 0xffffffffffff3fff, 0x20, fa0, fa1);
   /* sqrt(nextafterf(1.0)) (DYN-RDN) -> 1.0 (NX) */
   TESTINST_1_1_F(4, "fsqrt.h fa0, fa1", 0xffffffffffff3bff, 0x40, fa0, fa1);
   /* sqrt(2nextafterf(1.0)) (DYN-RDN) -> 1.0 (NX) */
   TESTINST_1_1_F(4, "fsqrt.h fa0, fa1", 0xffffffffffff3fff, 0x40, fa0, fa1);
   /* sqrt(nextafterf(1.0)) (DYN-RUP) -> nextafterf(1.0) (NX) */
   TESTINST_1_1_F(4, "fsqrt.h fa0, fa1", 0xffffffffffff3bff, 0x60, fa0, fa1);
   /* sqrt(2nextafterf(1.0)) (DYN-RUP) -> nextafterf(1.0) (NX) */
   TESTINST_1_1_F(4, "fsqrt.h fa0, fa1", 0xffffffffffff3fff, 0x60, fa0, fa1);
   /* sqrt(nextafterf(1.0)) (DYN-RMM) -> 1.0 (NX) */
   TESTINST_1_1_F(4, "fsqrt.h fa0, fa1", 0xffffffffffff3bff, 0x80, fa0, fa1);
   /* sqrt(2nextafterf(1.0)) (DYN-RMM) -> nextafterf(1.0) (NX) */
   TESTINST_1_1_F(4, "fsqrt.h fa0, fa1", 0xffffffffffff3fff, 0x80, fa0, fa1);

   /* ---------------- fsgnj.h rd, rs1, rs2 ----------------- */
   /* fmv.s rd, rs1 */
   TESTINST_1_2_F(4, "fsgnj.h fa0, fa1, fa1", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0x00, fa0, fa1, fa1);
   TESTINST_1_2_F(4, "fsgnj.h fa0, fa1, fa1", 0xffffffffffffbc00,
                  0xffffffffffffbc00, 0x00, fa0, fa1, fa1);

   /* fsgnj(1.0, +) -> 1.0 */
   TESTINST_1_2_F(4, "fsgnj.h fa0, fa1, fa2", 0xffffffffffff3c00,
                  0xffffffffffff7fff, 0x00, fa0, fa1, fa2);
   /* fsgnj(1.0, -) -> -1.0 */
   TESTINST_1_2_F(4, "fsgnj.h fa0, fa1, fa2", 0xffffffffffff3c00,
                  0xffffffffffff8000, 0x00, fa0, fa1, fa2);
   /* fsgnj(-1.0, +) -> 1.0 */
   TESTINST_1_2_F(4, "fsgnj.h fa0, fa1, fa2", 0xffffffffffffbc00,
                  0xffffffffffff7fff, 0x00, fa0, fa1, fa2);
   /* fsgnj(-1.0, -) -> -1.0 */
   TESTINST_1_2_F(4, "fsgnj.h fa0, fa1, fa2", 0xffffffffffffbc00,
                  0xffffffffffff8000, 0x00, fa0, fa1, fa2);

   /* ---------------- fsgnjn.h rd, rs1, rs2 ---------------- */
   /* fneg.s rd, rs1 */
   TESTINST_1_2_F(4, "fsgnjn.h fa0, fa1, fa1", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0x00, fa0, fa1, fa1);
   TESTINST_1_2_F(4, "fsgnjn.h fa0, fa1, fa1", 0xffffffffffffbc00,
                  0xffffffffffffbc00, 0x00, fa0, fa1, fa1);

   /* fsgnjn(1.0, +) -> -1.0 */
   TESTINST_1_2_F(4, "fsgnjn.h fa0, fa1, fa2", 0xffffffffffff3c00,
                  0xffffffffffff7fff, 0x00, fa0, fa1, fa2);
   /* fsgnjn(1.0, -) -> 1.0 */
   TESTINST_1_2_F(4, "fsgnjn.h fa0, fa1, fa2", 0xffffffffffff3c00,
                  0xffffffffffff8000, 0x00, fa0, fa1, fa2);
   /* fsgnjn(-1.0, +) -> -1.0 */
   TESTINST_1_2_F(4, "fsgnjn.h fa0, fa1, fa2", 0xffffffffffffbc00,
                  0xffffffffffff7fff, 0x00, fa0, fa1, fa2);
   /* fsgnjn(-1.0, -) -> 1.0 */
   TESTINST_1_2_F(4, "fsgnjn.h fa0, fa1, fa2", 0xffffffffffffbc00,
                  0xffffffffffff8000, 0x00, fa0, fa1, fa2);

   /* ---------------- fsgnjx.h rd, rs1, rs2 ---------------- */
   /* fabs.s rd, rs1 */
   TESTINST_1_2_F(4, "fsgnjx.h fa0, fa1, fa1", 0xffffffffffff3c00,
                  0xffffffffffff3c00, 0x00, fa0, fa1, fa1);
   TESTINST_1_2_F(4, "fsgnjx.h fa0, fa1, fa1", 0xffffffffffffbc00,
                  0xffffffffffffbc00, 0x00, fa0, fa1, fa1);

   /* fsgnjx(1.0, +) -> 1.0 */
   TESTINST_1_2_F(4, "fsgnjx.h fa0, fa1, fa2", 0xffffffffffff3c00,
                  0xffffffffffff7fff, 0x00, fa0, fa1, fa2);
   /* fsgnjx(1.0, -) -> -1.0 */
   TESTINST_1_2_F(4, "fsgnjx.h fa0, fa1, fa2", 0xffffffffffff3c00,
                  0xffffffffffff8000, 0x00, fa0, fa1, fa2);
   /* fsgnjx(-1.0, +) -> -1.0 */
   TESTINST_1_2_F(4, "fsgnjx.h fa0, fa1, fa2", 0xffffffffffffbc00,
                  0xffffffffffff7fff, 0x00, fa0, fa1, fa2);
   /* fsgnjx(-1.0, -) -> 1.0 */
   TESTINST_1_2_F(4, "fsgnjx.h fa0, fa1, fa2", 0xffffffffffffbc00,
                  0xffffffffffff8000, 0x00, fa0, fa1, fa2);

   /* ----------------- fmin.h rd, rs1, rs2 ----------------- */
   /* min(0.0, 1.0) -> 0.0 */
   TESTINST_1_2_F(4, "fmin.h fa0, fa1, fa2", 0xffffffffffff0000,
                  0xffffffffffff3c00, 0x00, fa0, fa1, fa2);
   /* min(0.0, -0.0) -> -0.0 */
   TESTINST_1_2_F(4, "fmin.h fa0, fa1, fa2", 0xffffffffffff0000,
                  0xffffffffffff8000, 0x00, fa0, fa1, fa2);
   /* min(-0.0, 0.0) -> -0.0 */
   TESTINST_1_2_F(4, "fmin.h fa0, fa1, fa2", 0xffffffffffff8000,
                  0xffffffffffff0000, 0x00, fa0, fa1, fa2);
   /* min(INFINITY, INFINITY) -> INFINITY */
   TESTINST_1_2_F(4, "fmin.h fa0, fa1, fa2", 0xffffffffffff7c00,
                  0xffffffffffff7c00, 0x00, fa0, fa1, fa2);
   /* min(0.0, qNAN) -> 0.0 */
   TESTINST_1_2_F(4, "fmin.h fa0, fa1, fa2", 0xffffffffffff0000,
                  0xffffffffffff7e00, 0x00, fa0, fa1, fa2);
   /* min(0.0, sNAN) -> 0.0 (NV) */
   TESTINST_1_2_F(4, "fmin.h fa0, fa1, fa2", 0xffffffffffff0000,
                  0xffffffffffff7d00, 0x00, fa0, fa1, fa2);

   /* ----------------- fmax.h rd, rs1, rs2 ----------------- */
   /* max(0.0, 1.0) -> 1.0 */
   TESTINST_1_2_F(4, "fmax.h fa0, fa1, fa2", 0xffffffffffff0000,
                  0xffffffffffff3c00, 0x00, fa0, fa1, fa2);
   /* max(0.0, -0.0) -> 0.0 */
   TESTINST_1_2_F(4, "fmax.h fa0, fa1, fa2", 0xffffffffffff0000,
                  0xffffffffffff8000, 0x00, fa0, fa1, fa2);
   /* max(-0.0, 0.0) -> 0.0 */
   TESTINST_1_2_F(4, "fmax.h fa0, fa1, fa2", 0xffffffffffff8000,
                  0xffffffffffff0000, 0x00, fa0, fa1, fa2);
   /* max(INFINITY, INFINITY) -> INFINITY */
   TESTINST_1_2_F(4, "fmax.h fa0, fa1, fa2", 0xffffffffffff7c00,
                  0xffffffffffff7c00, 0x00, fa0, fa1, fa2);
   /* max(0.0, qNAN) -> 0.0 */
   TESTINST_1_2_F(4, "fmax.h fa0, fa1, fa2", 0xffffffffffff0000,
                  0xffffffffffff7e00, 0x00, fa0, fa1, fa2);
   /* max(0.0, sNAN) -> 0.0 (NV) */
   TESTINST_1_2_F(4, "fmax.h fa0, fa1, fa2", 0xffffffffffff0000,
                  0xffffffffffff7d00, 0x00, fa0, fa1, fa2);

   /* ---------------- fcvt.w.h rd, rs1, rm ----------------- */
   /* 0.0 -> 0 */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0", 0xffffffffffff0000, 0x00, a0, fa0);
   /* HFLT_TRUE_MIN -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0", 0xffffffffffff0001, 0x00, a0, fa0);
   /* INFINITY -> 2**15-1 aka INT_MAX (NV)  */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0", 0xffffffffffff7c00, 0x00, a0, fa0);
   /* qNAN -> 2**15-1 aka INT_MAX (NV) */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0", 0xffffffffffff7e00, 0x00, a0, fa0);
   /* nextafterf(2**15, 0.0) -> 2**15-(2**4) */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0", 0xffffffffffff77ff, 0x00, a0, fa0);
   /* -2**15 -> -2**15 */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0", 0xfffffffffffff800, 0x00, a0, fa0);
   /* 2**15 -> 2**15 */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0", 0xffffffffffff7800, 0x00, a0, fa0);
   /* -nextafterf(2**15) -> -2**15 + (2**4) */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0", 0xfffffffffffff7ff, 0x00, a0, fa0);

   /* 1.0 (rd=zero) -> 0 */
   TESTINST_1_1_IF(4, "fcvt.w.h zero, fa0", 0xffffffffffff3c00, 0x00, zero,
                   fa0);

   /* 0.5 (RNE) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0, rne", 0xffffffffffff3800, 0x00, a0,
                   fa0);
   /* 1.5 (RNE) -> 2 (NX) */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0, rne", 0xffffffffffff3e00, 0x00, a0,
                   fa0);
   /* 0.5 (RTZ) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0, rtz", 0xffffffffffff3800, 0x00, a0,
                   fa0);
   /* -0.5 (RTZ) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0, rtz", 0xffffffffffffb800, 0x00, a0,
                   fa0);
   /* 0.5 (RDN) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0, rdn", 0xffffffffffff3800, 0x00, a0,
                   fa0);
   /* -0.5 (RDN) -> -1 (NX) */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0, rdn", 0xffffffffffffb800, 0x00, a0,
                   fa0);
   /* 0.5 (RUP) -> 1 (NX) */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0, rup", 0xffffffffffff3800, 0x00, a0,
                   fa0);
   /* -0.5 (RUP) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0, rup", 0xffffffffffffb800, 0x00, a0,
                   fa0);
   /* 0.5 (RMM) -> 1 (NX) */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0, rmm", 0xffffffffffff3800, 0x00, a0,
                   fa0);
   /* -0.5 (RMM) -> -1 (NX) */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0, rmm", 0xffffffffffffb800, 0x00, a0,
                   fa0);

   /* 0.5 (DYN-RNE) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0", 0xffffffffffff3800, 0x00, a0, fa0);
   /* 1.5 (DYN-RNE) -> 2 (NX) */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0", 0xffffffffffff3e00, 0x00, a0, fa0);
   /* 0.5 (DYN-RTZ) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0", 0xffffffffffff3800, 0x20, a0, fa0);
   /* -0.5 (DYN-RTZ) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0", 0xffffffffffffb800, 0x20, a0, fa0);
   /* 0.5 (DYN-RDN) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0", 0xffffffffffff3800, 0x40, a0, fa0);
   /* -0.5 (DYN-RDN) -> -1 (NX) */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0", 0xffffffffffffb800, 0x40, a0, fa0);
   /* 0.5 (DYN-RUP) -> 1 (NX) */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0", 0xffffffffffff3800, 0x60, a0, fa0);
   /* -0.5 (DYN-RUP) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0", 0xffffffffffffb800, 0x60, a0, fa0);
   /* 0.5 (DYN-RMM) -> 1 (NX) */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0", 0xffffffffffff3800, 0x80, a0, fa0);
   /* -0.5 (DYN-RMM) -> -1 (NX) */
   TESTINST_1_1_IF(4, "fcvt.w.h a0, fa0", 0xffffffffffffb800, 0x80, a0, fa0);

   /* ---------------- fcvt.wu.h rd, rs1, rm ---------------- */
   /* 0.0 -> 0 */
   TESTINST_1_1_IF(4, "fcvt.wu.h a0, fa0", 0xffffffffffff0000, 0x00, a0, fa0);
   /* HFLT_TRUE_MIN -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.wu.h a0, fa0", 0xffffffffffff0001, 0x00, a0, fa0);
   /* INFINITY -> 2**32-1 aka UINT_MAX (NV) */
   TESTINST_1_1_IF(4, "fcvt.wu.h a0, fa0", 0xffffffffffff7c00, 0x00, a0, fa0);
   /* qNAN -> 2**32-1 aka UINT_MAX (NV) */
   TESTINST_1_1_IF(4, "fcvt.wu.h a0, fa0", 0xffffffffffff7e00, 0x00, a0, fa0);
   /* nextafterf(2**15, 0.0) -> 2**15-(2**4) */
   TESTINST_1_1_IF(4, "fcvt.wu.h a0, fa0", 0xffffffffffff77ff, 0x00, a0, fa0);

   /* -1.0 -> 0 (NV) */
   TESTINST_1_1_IF(4, "fcvt.wu.h a0, fa0", 0xffffffffffffbc00, 0x00, a0, fa0);

   /* 1.0 (rd=zero) -> 0 */
   TESTINST_1_1_IF(4, "fcvt.wu.h zero, fa0", 0xffffffffffff3c00, 0x00, zero,
                   fa0);

   /* 0.5 (RNE) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.wu.h a0, fa0, rne", 0xffffffffffff3800, 0x00, a0,
                   fa0);
   /* 1.5 (RNE) -> 2 (NX) */
   TESTINST_1_1_IF(4, "fcvt.wu.h a0, fa0, rne", 0xffffffffffff3e00, 0x00, a0,
                   fa0);
   /* 0.5 (RTZ) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.wu.h a0, fa0, rtz", 0xffffffffffff3800, 0x00, a0,
                   fa0);
   /* 0.5 (RDN) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.wu.h a0, fa0, rdn", 0xffffffffffff3800, 0x00, a0,
                   fa0);
   /* 0.5 (RUP) -> 1 (NX) */
   TESTINST_1_1_IF(4, "fcvt.wu.h a0, fa0, rup", 0xffffffffffff3800, 0x00, a0,
                   fa0);
   /* 0.5 (RMM) -> 1 (NX) */
   TESTINST_1_1_IF(4, "fcvt.wu.h a0, fa0, rmm", 0xffffffffffff3800, 0x00, a0,
                   fa0);

   /* 0.5 (DYN-RNE) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.wu.h a0, fa0", 0xffffffffffff3800, 0x00, a0, fa0);
   /* 1.5 (DYN-RNE) -> 2 (NX) */
   TESTINST_1_1_IF(4, "fcvt.wu.h a0, fa0", 0xffffffffffff3e00, 0x00, a0, fa0);
   /* 0.5 (DYN-RTZ) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.wu.h a0, fa0", 0xffffffffffff3800, 0x20, a0, fa0);
   /* 0.5 (DYN-RDN) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.wu.h a0, fa0", 0xffffffffffff3800, 0x40, a0, fa0);
   /* 0.5 (DYN-RUP) -> 1 (NX) */
   TESTINST_1_1_IF(4, "fcvt.wu.h a0, fa0", 0xffffffffffff3800, 0x60, a0, fa0);
   /* 0.5 (DYN-RMM) -> 1 (NX) */
   TESTINST_1_1_IF(4, "fcvt.wu.h a0, fa0", 0xffffffffffff3800, 0x80, a0, fa0);

   /* ----------------- feq.h rd, rs1, rs2 ------------------ */
   /* 0.0 == 1.0 -> 0 */
   TESTINST_1_2_FCMP(4, "feq.h a0, fa0, fa1", 0xffffffffffff0000,
                     0xffffffffffff3c00, 0x00, a0, fa0, fa1);
   /* 0.0 == 0.0 -> 1 */
   TESTINST_1_2_FCMP(4, "feq.h a0, fa0, fa1", 0xffffffffffff0000,
                     0xffffffffffff0000, 0x00, a0, fa0, fa1);
   /* 0.0 == -0.0 -> 1 */
   TESTINST_1_2_FCMP(4, "feq.h a0, fa0, fa1", 0xffffffffffff0000,
                     0xffffffffffff8000, 0x00, a0, fa0, fa1);
   /* -0.0 == 0.0 -> 1 */
   TESTINST_1_2_FCMP(4, "feq.h a0, fa0, fa1", 0xffffffffffff8000,
                     0xffffffffffff0000, 0x00, a0, fa0, fa1);
   /* INFINITY == INFINITY -> 1 */
   TESTINST_1_2_FCMP(4, "feq.h a0, fa0, fa1", 0xffffffffffff7c00,
                     0xffffffffffff7c00, 0x00, a0, fa0, fa1);
   /* 0.0 == qNAN -> 0 */
   TESTINST_1_2_FCMP(4, "feq.h a0, fa0, fa1", 0xffffffffffff0000,
                     0xffffffffffff7e00, 0x00, a0, fa0, fa1);
   /* 0.0 == sNAN -> 0 (NV) */
   TESTINST_1_2_FCMP(4, "feq.h a0, fa0, fa1", 0xffffffffffff0000,
                     0xffffffffffff7d00, 0x00, a0, fa0, fa1);

   /* sNAN == sNAN (rd=zero) -> 0 (NV) */
   TESTINST_1_2_FCMP(4, "feq.h zero, fa0, fa1", 0xffffffffffff7d00,
                     0xffffffffffff7d00, 0x00, zero, fa0, fa1);

   /* ----------------- flt.h rd, rs1, rs2 ------------------ */
   /* 0.0 < 0.0 -> 0 */
   TESTINST_1_2_FCMP(4, "flt.h a0, fa0, fa1", 0xffffffffffff0000,
                     0xffffffffffff0000, 0x00, a0, fa0, fa1);
   /* 0.0 < 1.0 -> 1 */
   TESTINST_1_2_FCMP(4, "flt.h a0, fa0, fa1", 0xffffffffffff0000,
                     0xffffffffffff3c00, 0x00, a0, fa0, fa1);
   /* 0.0 < -0.0 -> 0 */
   TESTINST_1_2_FCMP(4, "flt.h a0, fa0, fa1", 0xffffffffffff0000,
                     0xffffffffffff8000, 0x00, a0, fa0, fa1);
   /* -0.0 < 0.0 -> 0 */
   TESTINST_1_2_FCMP(4, "flt.h a0, fa0, fa1", 0xffffffffffff8000,
                     0xffffffffffff0000, 0x00, a0, fa0, fa1);
   /* INFINITY < INFINITY -> 0 */
   TESTINST_1_2_FCMP(4, "flt.h a0, fa0, fa1", 0xffffffffffff7c00,
                     0xffffffffffff7c00, 0x00, a0, fa0, fa1);
   /* 0.0 < qNAN -> 0 (NV) */
   TESTINST_1_2_FCMP(4, "flt.h a0, fa0, fa1", 0xffffffffffff0000,
                     0xffffffffffff7e00, 0x00, a0, fa0, fa1);
   /* 0.0 < sNAN -> 0 (NV) */
   TESTINST_1_2_FCMP(4, "flt.h a0, fa0, fa1", 0xffffffffffff0000,
                     0xffffffffffff7d00, 0x00, a0, fa0, fa1);

   /* sNAN < sNAN (rd=zero) -> 0 (NV) */
   TESTINST_1_2_FCMP(4, "flt.h zero, fa0, fa1", 0xffffffffffff7d00,
                     0xffffffffffff7d00, 0x00, zero, fa0, fa1);

   /* ----------------- fle.h rd, rs1, rs2 ------------------ */
   /* 1.0 < 0.0 -> 0 */
   TESTINST_1_2_FCMP(4, "fle.h a0, fa0, fa1", 0xffffffffffff3c00,
                     0xffffffffffff0000, 0x00, a0, fa0, fa1);
   /* 0.0 <= 0.0 -> 1 */
   TESTINST_1_2_FCMP(4, "fle.h a0, fa0, fa1", 0xffffffffffff0000,
                     0xffffffffffff0000, 0x00, a0, fa0, fa1);
   /* 0.0 <= 1.0 -> 1 */
   TESTINST_1_2_FCMP(4, "fle.h a0, fa0, fa1", 0xffffffffffff0000,
                     0xffffffffffff3c00, 0x00, a0, fa0, fa1);
   /* 0.0 <= -0.0 -> 1 */
   TESTINST_1_2_FCMP(4, "fle.h a0, fa0, fa1", 0xffffffffffff0000,
                     0xffffffffffff8000, 0x00, a0, fa0, fa1);
   /* -0.0 <= 0.0 -> 1 */
   TESTINST_1_2_FCMP(4, "fle.h a0, fa0, fa1", 0xffffffffffff8000,
                     0xffffffffffff0000, 0x00, a0, fa0, fa1);
   /* INFINITY <= INFINITY -> 1 */
   TESTINST_1_2_FCMP(4, "fle.h a0, fa0, fa1", 0xffffffffffff7c00,
                     0xffffffffffff7c00, 0x00, a0, fa0, fa1);
   /* 0.0 <= qNAN -> 0 (NV) */
   TESTINST_1_2_FCMP(4, "fle.h a0, fa0, fa1", 0xffffffffffff0000,
                     0xffffffffffff7e00, 0x00, a0, fa0, fa1);
   /* 0.0 <= sNAN -> 0 (NV) */
   TESTINST_1_2_FCMP(4, "fle.h a0, fa0, fa1", 0xffffffffffff0000,
                     0xffffffffffff7d00, 0x00, a0, fa0, fa1);

   /* sNAN <= sNAN (rd=zero) -> 0 (NV) */
   TESTINST_1_2_FCMP(4, "fle.h zero, fa0, fa1", 0xffffffffffff7d00,
                     0xffffffffffff7d00, 0x00, zero, fa0, fa1);

   /* ---------------- fcvt.h.w rd, rs1, rm ----------------- */
   /* 0 -> 0.0 */
   TESTINST_1_1_FI(4, "fcvt.h.w fa0, a0", 0x0000000000000000, 0x00, fa0, a0);
   /* 2**15-(2**4) -> nextafterf(2**15, 0.0) */
   TESTINST_1_1_FI(4, "fcvt.h.w fa0, a0", 0x0000000000007ff0, 0x00, fa0, a0);
   /* 2**15-1 aka INT16_MAX -> 2**15 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.w fa0, a0", 0x0000000000007fff, 0x00, fa0, a0);
   /* -2**15 aka INT16_MIN -> -2**15 */
   TESTINST_1_1_FI(4, "fcvt.h.w fa0, a0", 0x00000000ffff8000, 0x00, fa0, a0);

   /* 2**11+1 (RNE) -> 2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.w fa0, a0, rne", 0x0000000000000801, 0x00, fa0,
                   a0);
   /* 2**11+3 (RNE) -> 2**11+4 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.w fa0, a0, rne", 0x0000000000000803, 0x00, fa0,
                   a0);
   /* 2**11+1 (RTZ) -> 2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.w fa0, a0, rtz", 0x0000000000000801, 0x00, fa0,
                   a0);
   /* -2**11-1 (RTZ) -> -2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.w fa0, a0, rtz", 0xfffffffffffff7ff, 0x00, fa0,
                   a0);
   /* 2**11+1 (RDN) -> 2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.w fa0, a0, rdn", 0x0000000000000801, 0x00, fa0,
                   a0);
   /* -2**11-1 (RDN) -> -2**11-2 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.w fa0, a0, rdn", 0xfffffffffffff7ff, 0x00, fa0,
                   a0);
   /* 2**11+1 (RUP) -> 2**11+2 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.w fa0, a0, rup", 0x0000000000000801, 0x00, fa0,
                   a0);
   /* -2**11-1 (RUP) -> -2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.w fa0, a0, rup", 0xfffffffffffff7ff, 0x00, fa0,
                   a0);
   /* 2**11+1 (RMM) -> 2**11+2 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.w fa0, a0, rmm", 0x0000000000000801, 0x00, fa0,
                   a0);
   /* -2**11-1 (RMM) -> -2**11-2 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.w fa0, a0, rmm", 0xfffffffffffff7ff, 0x00, fa0,
                   a0);

   /* 2**11+1 (DYN-RNE) -> 2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.w fa0, a0", 0x0000000000000801, 0x00, fa0, a0);
   /* 2**11+3 (DYN-RNE) -> 2**11+4 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.w fa0, a0", 0x0000000000000803, 0x00, fa0, a0);
   /* 2**11+1 (DYN-RTZ) -> 2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.w fa0, a0", 0x0000000000000801, 0x20, fa0, a0);
   /* -2**11-1 (DYN-RTZ) -> -2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.w fa0, a0", 0xfffffffffffff7ff, 0x20, fa0, a0);
   /* 2**11+1 (DYN-RDN) -> 2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.w fa0, a0", 0x0000000000000801, 0x40, fa0, a0);
   /* -2**11-1 (DYN-RDN) -> -2**11-2 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.w fa0, a0", 0xfffffffffffff7ff, 0x40, fa0, a0);
   /* 2**11+1 (DYN-RUP) -> 2**11+2 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.w fa0, a0", 0x0000000000000801, 0x60, fa0, a0);
   /* -2**11-1 (DYN-RUP) -> -2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.w fa0, a0", 0xfffffffffffff7ff, 0x60, fa0, a0);
   /* 2**11+1 (DYN-RMM) -> 2**11+2 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.w fa0, a0", 0x0000000000000801, 0x80, fa0, a0);
   /* -2**11-1 (DYN-RMM) -> -2**11-2 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.w fa0, a0", 0xfffffffffffff7ff, 0x80, fa0, a0);

   /* ---------------- fcvt.h.wu rd, rs1, rm ---------------- */
   /* 0 -> 0.0 */
   TESTINST_1_1_FI(4, "fcvt.h.wu fa0, a0", 0x0000000000000000, 0x00, fa0, a0);
   /* 2**16-(2**5) -> nextafterf(2**16, 0.0) */
   TESTINST_1_1_FI(4, "fcvt.h.wu fa0, a0", 0x000000000000ffe0, 0x00, fa0, a0);

   /* 2**11+1 (RNE) -> 2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.wu fa0, a0, rne", 0x0000000000000801, 0x00, fa0,
                   a0);
   /* 2**11+3 (RNE) -> 2**11+4 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.wu fa0, a0, rne", 0x0000000000000803, 0x00, fa0,
                   a0);
   /* 2**11+1 (RTZ) -> 2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.wu fa0, a0, rtz", 0x0000000000000801, 0x00, fa0,
                   a0);
   /* 2**11+1 (RDN) -> 2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.wu fa0, a0, rdn", 0x0000000000000801, 0x00, fa0,
                   a0);
   /* 2**11+1 (RUP) -> 2**11+2 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.wu fa0, a0, rup", 0x0000000000000801, 0x00, fa0,
                   a0);
   /* 2**11+1 (RMM) -> 2**11+2 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.wu fa0, a0, rmm", 0x0000000000000801, 0x00, fa0,
                   a0);

   /* 2**11+1 (DYN-RNE) -> 2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.wu fa0, a0", 0x0000000000000801, 0x00, fa0, a0);
   /* 2**11+3 (DYN-RNE) -> 2**11+4 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.wu fa0, a0", 0x0000000000000803, 0x00, fa0, a0);
   /* 2**11+1 (DYN-RTZ) -> 2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.wu fa0, a0", 0x0000000000000801, 0x20, fa0, a0);
   /* 2**11+1 (DYN-RDN) -> 2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.wu fa0, a0", 0x0000000000000801, 0x40, fa0, a0);
   /* 2**11+1 (DYN-RUP) -> 2**11+2 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.wu fa0, a0", 0x0000000000000801, 0x60, fa0, a0);
   /* 2**11+1 (DYN-RMM) -> 2**11+2 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.wu fa0, a0", 0x0000000000000801, 0x80, fa0, a0);

   /* ------------------- fmv.x.h rd, rs1 ------------------- */
   TESTINST_1_1_IF(4, "fmv.x.h a0, fa0", 0xabcdef0123456789, 0x00, a0, fa0);

   /* "0xffffffffffff7fff" -> "0x0000000000007fff" */
   TESTINST_1_1_IF(4, "fmv.x.h a0, fa0", 0xffffffffffff7fff, 0x00, a0, fa0);
   /* "0x0000000000008000" -> "0xffffffffffff8000" */
   TESTINST_1_1_IF(4, "fmv.x.h a0, fa0", 0x0000000000008000, 0x00, a0, fa0);

   /* 1.0 (rd=zero) -> 0 */
   TESTINST_1_1_IF(4, "fmv.x.h zero, fa0", 0xffffffffffff3c00, 0x00, zero, fa0);

   /* ------------------ fclass.h rd, rs1 ------------------- */
   /* fclass(-INFINITY) -> 0x001 */
   TESTINST_1_1_IF(4, "fclass.h a0, fa0", 0xfffffffffffffc00, 0x00, a0, fa0);
   /* fclass(-1.0) -> 0x002 */
   TESTINST_1_1_IF(4, "fclass.h a0, fa0", 0xffffffffffffbc00, 0x00, a0, fa0);
   /* fclass(-FLT_TRUE_MIN) -> 0x004 */
   TESTINST_1_1_IF(4, "fclass.h a0, fa0", 0xffffffffffff8001, 0x00, a0, fa0);
   /* fclass(-0.0) -> 0x008 */
   TESTINST_1_1_IF(4, "fclass.h a0, fa0", 0xffffffffffff8000, 0x00, a0, fa0);
   /* fclass(0.0) -> 0x010 */
   TESTINST_1_1_IF(4, "fclass.h a0, fa0", 0xffffffffffff0000, 0x00, a0, fa0);
   /* fclass(FLT_TRUE_MIN) -> 0x020 */
   TESTINST_1_1_IF(4, "fclass.h a0, fa0", 0xffffffffffff0001, 0x00, a0, fa0);
   /* fclass(1.0) -> 0x040 */
   TESTINST_1_1_IF(4, "fclass.h a0, fa0", 0xffffffffffff3c00, 0x00, a0, fa0);
   /* fclass(INFINITY) -> 0x080 */
   TESTINST_1_1_IF(4, "fclass.h a0, fa0", 0xffffffffffff7c00, 0x00, a0, fa0);
   /* fclass(sNAN) -> 0x100 */
   TESTINST_1_1_IF(4, "fclass.h a0, fa0", 0xffffffffffff7d00, 0x00, a0, fa0);
   /* fclass(qNAN) -> 0x200 */
   TESTINST_1_1_IF(4, "fclass.h a0, fa0", 0xffffffffffff7e00, 0x00, a0, fa0);

   /* fclass(-INFINITY) (rd=zero) -> 0x000 */
   TESTINST_1_1_IF(4, "fclass.h zero, fa0", 0xfffffffffffffc00, 0x00, zero,
                   fa0);

   /* ------------------- fmv.h.x rd, rs1 ------------------- */
   TESTINST_1_1_FI(4, "fmv.h.x fa0, a0", 0xabcdef0123456789, 0x00, fa0, a0);

   printf("\n");
}

static void test_float16_additions(void)
{
   printf("RV64 Zfh half-precision FP instruction set, additions\n");

   /* ---------------- fcvt.l.h rd, rs1, rm ----------------- */
   /* 0.0 -> 0 */
   TESTINST_1_1_IF(4, "fcvt.l.h a0, fa0", 0xffffffffffff0000, 0x00, a0, fa0);
   /* HFLT_TRUE_MIN -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.l.h a0, fa0", 0xffffffffffff0001, 0x00, a0, fa0);
   /* INFINITY -> 2**63-1 aka LONG_MAX (NV)  */
   TESTINST_1_1_IF(4, "fcvt.l.h a0, fa0", 0xffffffffffff7c00, 0x00, a0, fa0);
   /* qNAN -> 2**63-1 aka LONG_MAX (NV) */
   TESTINST_1_1_IF(4, "fcvt.l.h a0, fa0", 0xffffffffffff7e00, 0x00, a0, fa0);

   /* 1.0 (rd=zero) -> 0 */
   TESTINST_1_1_IF(4, "fcvt.l.h zero, fa0", 0xffffffffffff3c00, 0x00, zero,
                   fa0);

   /* 0.5 (RNE) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.l.h a0, fa0, rne", 0xffffffffffff3800, 0x00, a0,
                   fa0);
   /* 1.5 (RNE) -> 2 (NX) */
   TESTINST_1_1_IF(4, "fcvt.l.h a0, fa0, rne", 0xffffffffffff3e00, 0x00, a0,
                   fa0);
   /* 0.5 (RTZ) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.l.h a0, fa0, rtz", 0xffffffffffff3800, 0x00, a0,
                   fa0);
   /* -0.5 (RTZ) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.l.h a0, fa0, rtz", 0xffffffffffffb800, 0x00, a0,
                   fa0);
   /* 0.5 (RDN) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.l.h a0, fa0, rdn", 0xffffffffffff3800, 0x00, a0,
                   fa0);
   /* -0.5 (RDN) -> -1 (NX) */
   TESTINST_1_1_IF(4, "fcvt.l.h a0, fa0, rdn", 0xffffffffffffb800, 0x00, a0,
                   fa0);
   /* 0.5 (RUP) -> 1 (NX) */
   TESTINST_1_1_IF(4, "fcvt.l.h a0, fa0, rup", 0xffffffffffff3800, 0x00, a0,
                   fa0);
   /* -0.5 (RUP) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.l.h a0, fa0, rup", 0xffffffffffffb800, 0x00, a0,
                   fa0);
   /* 0.5 (RMM) -> 1 (NX) */
   TESTINST_1_1_IF(4, "fcvt.l.h a0, fa0, rmm", 0xffffffffffff3800, 0x00, a0,
                   fa0);
   /* -0.5 (RMM) -> -1 (NX) */
   TESTINST_1_1_IF(4, "fcvt.l.h a0, fa0, rmm", 0xffffffffffffb800, 0x00, a0,
                   fa0);

   /* 0.5 (DYN-RNE) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.l.h a0, fa0", 0xffffffffffff3800, 0x00, a0, fa0);
   /* 1.5 (DYN-RNE) -> 2 (NX) */
   TESTINST_1_1_IF(4, "fcvt.l.h a0, fa0", 0xffffffffffff3e00, 0x00, a0, fa0);
   /* 0.5 (DYN-RTZ) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.l.h a0, fa0", 0xffffffffffff3800, 0x20, a0, fa0);
   /* -0.5 (DYN-RTZ) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.l.h a0, fa0", 0xffffffffffffb800, 0x20, a0, fa0);
   /* 0.5 (DYN-RDN) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.l.h a0, fa0", 0xffffffffffff3800, 0x40, a0, fa0);
   /* -0.5 (DYN-RDN) -> -1 (NX) */
   TESTINST_1_1_IF(4, "fcvt.l.h a0, fa0", 0xffffffffffffb800, 0x40, a0, fa0);
   /* 0.5 (DYN-RUP) -> 1 (NX) */
   TESTINST_1_1_IF(4, "fcvt.l.h a0, fa0", 0xffffffffffff3800, 0x60, a0, fa0);
   /* -0.5 (DYN-RUP) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.l.h a0, fa0", 0xffffffffffffb800, 0x60, a0, fa0);
   /* 0.5 (DYN-RMM) -> 1 (NX) */
   TESTINST_1_1_IF(4, "fcvt.l.h a0, fa0", 0xffffffffffff3800, 0x80, a0, fa0);
   /* -0.5 (DYN-RMM) -> -1 (NX) */
   TESTINST_1_1_IF(4, "fcvt.l.h a0, fa0", 0xffffffffffffb800, 0x80, a0, fa0);

   /* ---------------- fcvt.lu.h rd, rs1, rm ---------------- */
   /* 0.0 -> 0 */
   TESTINST_1_1_IF(4, "fcvt.lu.h a0, fa0", 0xffffffffffff0000, 0x00, a0, fa0);
   /* HFLT_TRUE_MIN -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.lu.h a0, fa0", 0xffffffffffff0001, 0x00, a0, fa0);
   /* INFINITY -> 2**64-1 aka ULONG_MAX (NV) */
   TESTINST_1_1_IF(4, "fcvt.lu.h a0, fa0", 0xffffffffffff7c00, 0x00, a0, fa0);
   /* qNAN -> 2**64-1 aka ULONG_MAX (NV) */
   TESTINST_1_1_IF(4, "fcvt.lu.h a0, fa0", 0xffffffffffff7e00, 0x00, a0, fa0);
   /* -1.0 -> 0 (NV) */
   TESTINST_1_1_IF(4, "fcvt.lu.h a0, fa0", 0xffffffffffffbc00, 0x00, a0, fa0);

   /* 1.0 (rd=zero) -> 0 */
   TESTINST_1_1_IF(4, "fcvt.lu.h zero, fa0", 0xffffffffffff3c00, 0x00, zero,
                   fa0);

   /* 0.5 (RNE) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.lu.h a0, fa0, rne", 0xffffffffffff3800, 0x00, a0,
                   fa0);
   /* 1.5 (RNE) -> 2 (NX) */
   TESTINST_1_1_IF(4, "fcvt.lu.h a0, fa0, rne", 0xffffffffffff3e00, 0x00, a0,
                   fa0);
   /* 0.5 (RTZ) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.lu.h a0, fa0, rtz", 0xffffffffffff3800, 0x00, a0,
                   fa0);
   /* 0.5 (RDN) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.lu.h a0, fa0, rdn", 0xffffffffffff3800, 0x00, a0,
                   fa0);
   /* 0.5 (RUP) -> 1 (NX) */
   TESTINST_1_1_IF(4, "fcvt.lu.h a0, fa0, rup", 0xffffffffffff3800, 0x00, a0,
                   fa0);
   /* 0.5 (RMM) -> 1 (NX) */
   TESTINST_1_1_IF(4, "fcvt.lu.h a0, fa0, rmm", 0xffffffffffff3800, 0x00, a0,
                   fa0);

   /* 0.5 (DYN-RNE) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.lu.h a0, fa0", 0xffffffffffff3800, 0x00, a0, fa0);
   /* 1.5 (DYN-RNE) -> 2 (NX) */
   TESTINST_1_1_IF(4, "fcvt.lu.h a0, fa0", 0xffffffffffff3e00, 0x00, a0, fa0);
   /* 0.5 (DYN-RTZ) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.lu.h a0, fa0", 0xffffffffffff3800, 0x20, a0, fa0);
   /* 0.5 (DYN-RDN) -> 0 (NX) */
   TESTINST_1_1_IF(4, "fcvt.lu.h a0, fa0", 0xffffffffffff3800, 0x40, a0, fa0);
   /* 0.5 (DYN-RUP) -> 1 (NX) */
   TESTINST_1_1_IF(4, "fcvt.lu.h a0, fa0", 0xffffffffffff3800, 0x60, a0, fa0);
   /* 0.5 (DYN-RMM) -> 1 (NX) */
   TESTINST_1_1_IF(4, "fcvt.lu.h a0, fa0", 0xffffffffffff3800, 0x80, a0, fa0);

   /* ---------------- fcvt.h.l rd, rs1, rm ----------------- */
   /* 0 -> 0.0 */
   TESTINST_1_1_FI(4, "fcvt.h.l fa0, a0", 0x0000000000000000, 0x00, fa0, a0);

   /* 2**11+1 (RNE) -> 2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.l fa0, a0, rne", 0x0000000000000801, 0x00, fa0,
                   a0);
   /* 2**11+3 (RNE) -> 2**11+4 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.l fa0, a0, rne", 0x0000000000000803, 0x00, fa0,
                   a0);
   /* 2**11+1 (RTZ) -> 2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.l fa0, a0, rtz", 0x0000000000000801, 0x00, fa0,
                   a0);
   /* -2**11-1 (RTZ) -> -2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.l fa0, a0, rtz", 0xfffffffffffff7ff, 0x00, fa0,
                   a0);
   /* 2**11+1 (RDN) -> 2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.l fa0, a0, rdn", 0x0000000000000801, 0x00, fa0,
                   a0);
   /* -2**11-1 (RDN) -> -2**11-2 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.l fa0, a0, rdn", 0xfffffffffffff7ff, 0x00, fa0,
                   a0);
   /* 2**11+1 (RUP) -> 2**11+2 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.l fa0, a0, rup", 0x0000000000000801, 0x00, fa0,
                   a0);
   /* -2**11-1 (RUP) -> -2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.l fa0, a0, rup", 0xfffffffffffff7ff, 0x00, fa0,
                   a0);
   /* 2**11+1 (RMM) -> 2**11+2 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.l fa0, a0, rmm", 0x0000000000000801, 0x00, fa0,
                   a0);
   /* -2**11-1 (RMM) -> -2**11-2 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.l fa0, a0, rmm", 0xfffffffffffff7ff, 0x00, fa0,
                   a0);

   /* 2**11+1 (DYN-RNE) -> 2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.l fa0, a0", 0x0000000000000801, 0x00, fa0, a0);
   /* 2**11+3 (DYN-RNE) -> 2**11+4 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.l fa0, a0", 0x0000000000000803, 0x00, fa0, a0);
   /* 2**11+1 (DYN-RTZ) -> 2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.l fa0, a0", 0x0000000000000801, 0x20, fa0, a0);
   /* -2**11-1 (DYN-RTZ) -> -2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.l fa0, a0", 0xfffffffffffff7ff, 0x20, fa0, a0);
   /* 2**11+1 (DYN-RDN) -> 2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.l fa0, a0", 0x0000000000000801, 0x40, fa0, a0);
   /* -2**11-1 (DYN-RDN) -> -2**11-2 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.l fa0, a0", 0xfffffffffffff7ff, 0x40, fa0, a0);
   /* 2**11+1 (DYN-RUP) -> 2**11+2 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.l fa0, a0", 0x0000000000000801, 0x60, fa0, a0);
   /* -2**11-1 (DYN-RUP) -> -2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.l fa0, a0", 0xfffffffffffff7ff, 0x60, fa0, a0);
   /* 2**11+1 (DYN-RMM) -> 2**11+2 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.l fa0, a0", 0x0000000000000801, 0x80, fa0, a0);
   /* -2**11-1 (DYN-RMM) -> -2**11-2 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.l fa0, a0", 0xfffffffffffff7ff, 0x80, fa0, a0);

   /* ---------------- fcvt.h.lu rd, rs1, rm ---------------- */
   /* 0 -> 0.0 */
   TESTINST_1_1_FI(4, "fcvt.h.lu fa0, a0", 0x0000000000000000, 0x00, fa0, a0);

   /* 2**11+1 (RNE) -> 2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.lu fa0, a0, rne", 0x0000000000000801, 0x00, fa0,
                   a0);
   /* 2**11+3 (RNE) -> 2**11+4 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.lu fa0, a0, rne", 0x0000000000000803, 0x00, fa0,
                   a0);
   /* 2**11+1 (RTZ) -> 2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.lu fa0, a0, rtz", 0x0000000000000801, 0x00, fa0,
                   a0);
   /* 2**11+1 (RDN) -> 2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.lu fa0, a0, rdn", 0x0000000000000801, 0x00, fa0,
                   a0);
   /* 2**11+1 (RUP) -> 2**11+2 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.lu fa0, a0, rup", 0x0000000000000801, 0x00, fa0,
                   a0);
   /* 2**11+1 (RMM) -> 2**11+2 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.lu fa0, a0, rmm", 0x0000000000000801, 0x00, fa0,
                   a0);

   /* 2**11+1 (DYN-RNE) -> 2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.lu fa0, a0", 0x0000000000000801, 0x00, fa0, a0);
   /* 2**11+3 (DYN-RNE) -> 2**11+4 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.lu fa0, a0", 0x0000000000000803, 0x00, fa0, a0);
   /* 2**11+1 (DYN-RTZ) -> 2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.lu fa0, a0", 0x0000000000000801, 0x20, fa0, a0);
   /* 2**11+1 (DYN-RDN) -> 2**11 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.lu fa0, a0", 0x0000000000000801, 0x40, fa0, a0);
   /* 2**11+1 (DYN-RUP) -> 2**11+2 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.lu fa0, a0", 0x0000000000000801, 0x60, fa0, a0);
   /* 2**11+1 (DYN-RMM) -> 2**11+2 (NX) */
   TESTINST_1_1_FI(4, "fcvt.h.lu fa0, a0", 0x0000000000000801, 0x80, fa0, a0);
}

int main(void)
{
   test_float16_shared();
   test_float16_additions();
   return 0;
}
