/* Tests for the RV64 Zfh standard half-precision floating-point instruction-set
   extension. */

#include "testinst.h"

static void test_float16_shared(void)
{
   printf("RV64 Zfh half-precision FP instruction set.\n");
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
