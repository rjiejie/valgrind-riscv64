
/*--------------------------------------------------------------------*/
/*--- begin                                  guest_riscv64V_defs.h ---*/
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

#ifndef __VEX_GUEST_RISCV64V_DEFS_H
#define __VEX_GUEST_RISCV64V_DEFS_H

#include "libvex_basictypes.h"
#include "libvex_riscv_common.h"

#define VInsn(insn, op) #insn"."#op
#define VInsnVV(insn)   VInsn(insn, vv)
#define VInsnVX(insn)   VInsn(insn, vx)
#define VInsnVI         VInsnVX
#define VInsnVS(insn)   VInsn(insn, vs)
#define VInsnWV(insn)   VInsn(insn, wv)
#define VInsnWX(insn)   VInsn(insn, wx)

#define VInsnVVM(insn)  VInsn(insn, vvm)
#define VInsnVXM(insn)  VInsn(insn, vxm)
#define VInsnVIM        VInsnVXM

#define VInsnMM(insn)   VInsn(insn, mm)
#define VInsnM(insn)    VInsn(insn, m)
#define VInsnV(insn)    VInsn(insn, v)
#define VInsnX(insn)    VInsn(insn, x)
#define VInsnI          VInsnX
#define VInsnW(insn)    VInsn(insn, w)

#define VInsnVF(insn)   VInsn(insn, vf)
#define VInsnWF(insn)   VInsn(insn, wf)
#define VInsnF(insn)    VInsn(insn, f)

/*---------------------------------------------------------------*/
/*--- Get Name/Address of helper functions                    ---*/
/*---------------------------------------------------------------*/

#define GETN_VOp(insn, type, op, mask) "RVV_"#type"_"#insn#op#mask
#define GETA_VOp(insn, type, op, mask) RVV_##type##_##insn##op##mask

/* Binop */
#define GETN_VBinopVV_M(insn)  GETN_VOp(insn, Binop, vv, _m)
#define GETA_VBinopVV_M(insn)  GETA_VOp(insn, Binop, vv, _m)
#define GETN_VBinopVV(insn)    GETN_VOp(insn, Binop, vv,)
#define GETA_VBinopVV(insn)    GETA_VOp(insn, Binop, vv,)
#define GETN_VBinopVS_M        GETN_VBinopVV_M
#define GETA_VBinopVS_M        GETA_VBinopVV_M
#define GETN_VBinopVS          GETN_VBinopVV
#define GETA_VBinopVS          GETA_VBinopVV
#define GETN_VBinopWV_M        GETN_VBinopVV_M
#define GETA_VBinopWV_M        GETA_VBinopVV_M
#define GETN_VBinopWV          GETN_VBinopVV
#define GETA_VBinopWV          GETA_VBinopVV
#define GETN_VBinopVVM_M       GETN_VBinopVV_M
#define GETA_VBinopVVM_M       GETA_VBinopVV_M
#define GETN_VBinopVVM         GETN_VBinopVV
#define GETA_VBinopVVM         GETA_VBinopVV
#define GETN_VBinopMM_M        GETN_VBinopVV_M
#define GETA_VBinopMM_M        GETA_VBinopVV_M
#define GETN_VBinopMM          GETN_VBinopVV
#define GETA_VBinopMM          GETA_VBinopVV

#define GETN_VBinopVX_M(insn)  GETN_VOp(insn, Binop, vx, _m)
#define GETA_VBinopVX_M(insn)  GETA_VOp(insn, Binop, vx, _m)
#define GETN_VBinopVX(insn)    GETN_VOp(insn, Binop, vx,)
#define GETA_VBinopVX(insn)    GETA_VOp(insn, Binop, vx,)
#define GETN_VBinopWX_M        GETN_VBinopVX_M
#define GETA_VBinopWX_M        GETA_VBinopVX_M
#define GETN_VBinopWX          GETN_VBinopVX
#define GETA_VBinopWX          GETA_VBinopVX
#define GETN_VBinopVXM_M       GETN_VBinopVX_M
#define GETA_VBinopVXM_M       GETA_VBinopVX_M
#define GETN_VBinopVXM         GETN_VBinopVX
#define GETA_VBinopVXM         GETA_VBinopVX

#define GETN_VBinopVI_M(insn)  GETN_VOp(insn, Binop, vi, _m)
#define GETA_VBinopVI_M(insn)  GETA_VOp(insn, Binop, vi, _m)
#define GETN_VBinopVI(insn)    GETN_VOp(insn, Binop, vi,)
#define GETA_VBinopVI(insn)    GETA_VOp(insn, Binop, vi,)
#define GETN_VBinopVIM_M       GETN_VBinopVI_M
#define GETA_VBinopVIM_M       GETA_VBinopVI_M
#define GETN_VBinopVIM         GETN_VBinopVI
#define GETA_VBinopVIM         GETA_VBinopVI

#define GETN_VBinopVF_M(insn)  GETN_VOp(insn, Binop, vf, _m)
#define GETA_VBinopVF_M(insn)  GETA_VOp(insn, Binop, vf, _m)
#define GETN_VBinopVF(insn)    GETN_VOp(insn, Binop, vf,)
#define GETA_VBinopVF(insn)    GETA_VOp(insn, Binop, vf,)
#define GETN_VBinopWF_M        GETN_VBinopVF_M
#define GETA_VBinopWF_M        GETA_VBinopVF_M
#define GETN_VBinopWF          GETN_VBinopVF
#define GETA_VBinopWF          GETA_VBinopVF

#define GETN_VBinopVNIL_M(insn)  NULL
#define GETA_VBinopVNIL_M(insn)  NULL
#define GETN_VBinopVNIL(insn)    NULL
#define GETA_VBinopVNIL(insn)    NULL

/* Unop */
#define GETN_VUnopV_M(insn)    GETN_VOp(insn, Unop, v, _m)
#define GETA_VUnopV_M(insn)    GETA_VOp(insn, Unop, v, _m)
#define GETN_VUnopV(insn)      GETN_VOp(insn, Unop, v,)
#define GETA_VUnopV(insn)      GETA_VOp(insn, Unop, v,)
#define GETN_VUnopM_M          GETN_VUnopV_M
#define GETA_VUnopM_M          GETA_VUnopV_M
#define GETN_VUnopM            GETN_VUnopV
#define GETA_VUnopM            GETA_VUnopV
#define GETN_VUnopW_M          GETN_VUnopV_M
#define GETA_VUnopW_M          GETA_VUnopV_M
#define GETN_VUnopW            GETN_VUnopV
#define GETA_VUnopW            GETA_VUnopV

#define GETN_VUnopX_M(insn)    GETN_VOp(insn, Unop, x, _m)
#define GETA_VUnopX_M(insn)    GETA_VOp(insn, Unop, x, _m)
#define GETN_VUnopX(insn)      GETN_VOp(insn, Unop, x,)
#define GETA_VUnopX(insn)      GETA_VOp(insn, Unop, x,)

#define GETN_VUnopI_M(insn)    GETN_VOp(insn, Unop, i, _m)
#define GETA_VUnopI_M(insn)    GETA_VOp(insn, Unop, i, _m)
#define GETN_VUnopI(insn)      GETN_VOp(insn, Unop, i,)
#define GETA_VUnopI(insn)      GETA_VOp(insn, Unop, i,)

#define GETN_VUnopF_M(insn)    GETN_VOp(insn, Unop, f, _m)
#define GETA_VUnopF_M(insn)    GETA_VOp(insn, Unop, f, _m)
#define GETN_VUnopF(insn)      GETN_VOp(insn, Unop, f,)
#define GETA_VUnopF(insn)      GETA_VOp(insn, Unop, f,)

#define GETN_VUnopNIL_M(insn)  NULL
#define GETA_VUnopNIL_M(insn)  NULL
#define GETN_VUnopNIL(insn)    NULL
#define GETA_VUnopNIL(insn)    NULL

/*---------------------------------------------------------------*/
/*--- Special instruction templates                           ---*/
/*---------------------------------------------------------------*/

#define RVV_Config()                                                           \
   UShort vstart = st->guest_vstart;                                           \
   do {                                                                        \
      UShort vl    = st->guest_vl;                                             \
      UShort vtype = st->guest_vtype;                                          \
      __asm__ __volatile__("vsetvl\tx0,%0,%1\n\t" ::"r"(vl), "r"(vtype) :);    \
   } while (0)

#define RVV_ConfigVstart()                                                     \
   do {                                                                        \
      if (vstart)                                                              \
         __asm__ __volatile__("csrw\tvstart,%0\n\t" ::"r"(vstart) :);          \
   } while (0)

#define RVV_PushCfg()
#define RVV_PopCfg()
#define RVV_PushWCfg()
// Get mask
#define RVV_Mask()                  \
   __asm__ __volatile__(            \
      "vl1r.v\tv0,(%0)\n\t"         \
      :                             \
      : "r"(mask)                   \
      : );

// Get X/I/F register
#define RVV_VX(vs) \
   vs += (ULong)st;\
   __asm__ __volatile__("ld\tt0,0(%0)\n\t"::"r"(vs):"t0");
#define RVV_VI(vs) \
   __asm__ __volatile__("mv\tt0,%0\n\t"::"r"(vs):"t0");
#define RVV_VF(vs) \
   vs += (ULong)st;\
   __asm__ __volatile__("fld\tft0,0(%0)\n\t"::"r"(vs):"ft0");

// Push fcsr
// Set frm
#define RVV_PushFCSR()              \
   __asm__ __volatile__(            \
      "csrr\tt1,fcsr\n\t"           \
      "csrw\tfrm,%0\n\t"            \
      "csrw\tfflags,x0\n\t"         \
      :                             \
      : "r"(frm)                    \
      : "t1");
// Get fflags
// Pop fcsr
#define RVV_PopFCSR()               \
   __asm__ __volatile__(            \
      "csrr\t%0,fflags\n\t"         \
      "csrw\tfcsr,t1\n\t"           \
      : "=r"(ret)                   \
      :                             \
      :);

// Push fcsr
// Set xrm
#define RVV_PushXSAT()              \
   __asm__ __volatile__(            \
      "csrr\tt1,fcsr\n\t"           \
      "csrw\tvxrm,%0\n\t"           \
      "csrw\tvxsat,x0\n\t"          \
      :                             \
      : "r"(xrm)                    \
      : "t1");
// Get xsat
// Pop fcsr
#define RVV_PopXSAT()               \
   __asm__ __volatile__(            \
      "csrr\t%0,vxsat\n\t"          \
      "csrw\tfcsr,t1\n\t"           \
      : "=r"(ret)                   \
      :                             \
      :);

#define RVV_Push()
#define RVV_Pop()
#define RVV_Pre()
#define RVV_Post()

#define RVV_WholeLD0                \
   __asm__ __volatile__(            \
      "vl8r.v\tv8,(%0)\n\t"         \
      :                             \
      : "r"(vd)                     \
      :);

#define RVV_WholeLD1                \
   __asm__ __volatile__(            \
      "vl8r.v\tv8,(%0)\n\t"         \
      "vl8r.v\tv16,(%1)\n\t"        \
      :                             \
      : "r"(vd), "r"(vs2)           \
      :);

#define RVV_WholeLD                 \
   __asm__ __volatile__(            \
      "vl8r.v\tv8,(%0)\n\t"         \
      "vl8r.v\tv16,(%1)\n\t"        \
      "vl8r.v\tv24,(%2)\n\t"        \
      :                             \
      : "r"(vd), "r"(vs2), "r"(vs1) \
      :);

#define RVV_WholeST                 \
   RVV_ConfigVstart();              \
   __asm__ __volatile__(            \
      "vs8r.v\tv8,(%0)\n\t"         \
      :                             \
      : "r"(vd)                     \
      : "memory");

#define RVV_BinopVV_iTpl(insn, mreg) \
   __asm__ __volatile__(insn "\tv8,v16,v24" mreg);
#define RVV_UnopV_iTpl(insn, mreg) \
   __asm__ __volatile__(insn "\tv8,v16" mreg);

/*---------------------------------------------------------------*/
/*--- VV Instruction templates                                ---*/
/*---------------------------------------------------------------*/

// v8-v15, v16-v23, v24-v31
#define RVV_VOpVV_Tpl(insn, vd, vs2, vs1, imask, mreg, itpl)\
   do {                                                     \
      RVV_Config();                                         \
      UInt ret = 0;                                         \
                                                            \
      vd  += (ULong)st;                                     \
      vs2 += (ULong)st;                                     \
      vs1 += (ULong)st;                                     \
      mask += (ULong)st;                                    \
                                                            \
      RVV_WholeLD                                           \
                                                            \
      imask                                                 \
                                                            \
      RVV_Pre()                                             \
      itpl(insn, mreg)                                      \
      RVV_Post()                                            \
                                                            \
      RVV_WholeST                                           \
                                                            \
      return ret;                                           \
   } while (0)

#define RVV_BinopVV_Tpl(insn, vd, vs2, vs1, imask, mreg)    \
   RVV_VOpVV_Tpl(insn, vd, vs2, vs1, imask, mreg, RVV_BinopVV_iTpl)

#define RVV_UnopV_Tpl(insn, vd, vs2, vs1, imask, mreg)      \
   RVV_VOpVV_Tpl(insn, vd, vs2, vs1, imask, mreg, RVV_UnopV_iTpl)

/*---------------------------------------------------------------*/
/*--- OPIVV/OPFVV Instruction templates helper                ---*/
/*---------------------------------------------------------------*/

/* Binop */
#define RVV_BinopVV(insn, vd, vs2, vs1)    RVV_BinopVV_Tpl(insn, vd, vs2, vs1, ,)
#define RVV_BinopVV_M(insn, vd, vs2, vs1)  RVV_BinopVV_Tpl(insn, vd, vs2, vs1, RVV_Mask(), ",v0.t")
#define RVV_BinopVV2(insn, vd, vs2, vs1)   RVV_BinopVV_Tpl(insn, vd, vs1, vs2, ,)
#define RVV_BinopVV2_M(insn, vd, vs2, vs1) RVV_BinopVV_Tpl(insn, vd, vs1, vs2, RVV_Mask(), ",v0.t")

#define RVV_BinopVVM(insn, vd, vs2, vs1)   RVV_BinopVV_Tpl(insn, vd, vs2, vs1, RVV_Mask(), ",v0")
#define RVV_BinopVVM_M  RVV_BinopVVM

/* Unop */
#define RVV_UnopV(insn, vd, vs2, vs1)      RVV_UnopV_Tpl(insn, vd, vs2, vs1, ,)
#define RVV_UnopV_M(insn, vd, vs2, vs1)    RVV_UnopV_Tpl(insn, vd, vs2, vs1, RVV_Mask(), ",v0.t")

#define RVV_UnopV3(insn, vd, vs2, vs1)     RVV_UnopV_Tpl3(insn, vd, vs2, vs1, ,)
#define RVV_UnopV3_M(insn, vd, vs2, vs1)   RVV_UnopV_Tpl3(insn, vd, vs2, vs1, RVV_Mask(), ",v0.t")

/*---------------------------------------------------------------*/
/*--- VX/VI/VF Instruction templates                          ---*/
/*---------------------------------------------------------------*/

// v8-v15, v16-v23, t0/ft0
#define RVV_BinopXIF_iTpl(insn, treg, mreg)\
   __asm__ __volatile__(insn "\tv8,v16," treg mreg);
// v8-v15, t0/ft0, v16-v23
#define RVV_BinopXIF_iTpl2(insn, treg, mreg)\
   __asm__ __volatile__(insn "\tv8," treg ",v16" mreg);
// ret, v16-v23, t0/ft0
#define RVV_BinopXIF_iTpl3(insn, treg, mreg)\
   (void)vstart;\
   __asm__ __volatile__(insn "\t%0,v16" treg mreg :"=r"(ret)::);

#define RVV_BinopXIF_Generic(insn, vd, vs2, vs1, imask, mreg, sopc, treg, itpl)\
   do {                                                     \
      RVV_Config();                                         \
      UInt ret = 0;                                         \
                                                            \
      vd  += (ULong)st;                                     \
      vs2 += (ULong)st;                                     \
      mask += (ULong)st;                                    \
                                                            \
      RVV_WholeLD1                                          \
                                                            \
      imask                                                 \
                                                            \
      RVV_Pre()                                             \
      sopc                                                  \
      itpl(insn, treg, mreg)                                \
      RVV_Post()                                            \
                                                            \
      RVV_WholeST                                           \
                                                            \
      return ret;                                           \
   } while (0)

#define RVV_BinopXIF_Tpl(insn, vd, vs2, vs1, imask, mreg, sopc, treg)\
   RVV_BinopXIF_Generic(insn, vd, vs2, vs1, imask, mreg, sopc, treg, RVV_BinopXIF_iTpl)
#define RVV_BinopVX_Tpl(insn, vd, vs2, vs1, imask, mreg)\
   RVV_BinopXIF_Tpl(insn, vd, vs2, vs1, imask, mreg, RVV_VX(vs1), "t0")
#define RVV_BinopVI_Tpl(insn, vd, vs2, vs1, imask, mreg)\
   RVV_BinopXIF_Tpl(insn, vd, vs2, vs1, imask, mreg, RVV_VI(vs1), "t0")
#define RVV_BinopVF_Tpl(insn, vd, vs2, vs1, imask, mreg)\
   RVV_BinopXIF_Tpl(insn, vd, vs2, vs1, imask, mreg, RVV_VF(vs1), "ft0")

#define RVV_BinopXIF_Tpl2(insn, vd, vs2, vs1, imask, mreg, sopc, treg)\
   RVV_BinopXIF_Generic(insn, vd, vs2, vs1, imask, mreg, sopc, treg, RVV_BinopXIF_iTpl2)
#define RVV_BinopVX_Tpl2(insn, vd, vs2, vs1, imask, mreg)\
   RVV_BinopXIF_Tpl2(insn, vd, vs2, vs1, imask, mreg, RVV_VX(vs1), "t0")
#define RVV_BinopVF_Tpl2(insn, vd, vs2, vs1, imask, mreg)\
   RVV_BinopXIF_Tpl2(insn, vd, vs2, vs1, imask, mreg, RVV_VF(vs1), "ft0")

#define RVV_BinopXIF_Tpl3(insn, vd, vs2, vs1, imask, mreg, sopc, treg)\
   RVV_BinopXIF_Generic(insn, vd, vs2, vs1, imask, mreg, sopc, treg, RVV_BinopXIF_iTpl3)
#define RVV_BinopVX_Tpl3(insn, vd, vs2, vs1, imask, mreg)\
   RVV_BinopXIF_Tpl3(insn, vd, vs2, vs1, imask, mreg, RVV_VX(vs1), ",t0")
#define RVV_UnopV_Tpl3(insn, vd, vs2, vs1, imask, mreg)\
   RVV_BinopXIF_Tpl3(insn, vd, vs2, vs1, imask, mreg, , )

// v8-v15, t0/ft0
#define RVV_UnopXIF_Tpl(insn, vd, vs2, vs1, imask, mreg, sopc, treg)\
   do {                                                     \
      RVV_Config();                                         \
      UInt ret = 0;                                         \
                                                            \
      vd  += (ULong)st;                                     \
      mask += (ULong)st;                                    \
                                                            \
      RVV_WholeLD0                                          \
                                                            \
      imask                                                 \
                                                            \
      RVV_Pre()                                             \
      sopc                                                  \
      __asm__ __volatile__(insn "\tv8," treg mreg);         \
      RVV_Post()                                            \
                                                            \
      RVV_WholeST                                           \
                                                            \
      return ret;                                           \
   } while (0)

#define RVV_UnopX_Tpl(insn, vd, vs2, vs1, imask, mreg)\
   RVV_UnopXIF_Tpl(insn, vd, vs2, vs1, imask, mreg, RVV_VX(vs2), "t0")
#define RVV_UnopI_Tpl(insn, vd, vs2, vs1, imask, mreg)\
   RVV_UnopXIF_Tpl(insn, vd, vs2, vs1, imask, mreg, RVV_VI(vs2), "t0")
#define RVV_UnopF_Tpl(insn, vd, vs2, vs1, imask, mreg)\
   RVV_UnopXIF_Tpl(insn, vd, vs2, vs1, imask, mreg, RVV_VF(vs2), "ft0")

/*---------------------------------------------------------------*/
/*--- VX/VI/VF Instruction templates helper                   ---*/
/*---------------------------------------------------------------*/

/* Binop */
// OPI
#define RVV_BinopVX(insn, vd, vs2, vs1)    RVV_BinopVX_Tpl(insn, vd, vs2, vs1, ,)
#define RVV_BinopVX_M(insn, vd, vs2, vs1)  RVV_BinopVX_Tpl(insn, vd, vs2, vs1, RVV_Mask(), ",v0.t")
#define RVV_BinopVI(insn, vd, vs2, vs1)    RVV_BinopVI_Tpl(insn, vd, vs2, vs1, ,)
#define RVV_BinopVI_M(insn, vd, vs2, vs1)  RVV_BinopVI_Tpl(insn, vd, vs2, vs1, RVV_Mask(), ",v0.t")
#define RVV_BinopVX2(insn, vd, vs2, vs1)   RVV_BinopVX_Tpl2(insn, vd, vs2, vs1, ,)
#define RVV_BinopVX2_M(insn, vd, vs2, vs1) RVV_BinopVX_Tpl2(insn, vd, vs2, vs1, RVV_Mask(), ",v0.t")
#define RVV_BinopVX3(insn, vd, vs2, vs1)   RVV_BinopVX_Tpl3(insn, vd, vs2, vs1, ,)
#define RVV_BinopVX3_M  RVV_BinopVX3

#define RVV_BinopVXM(insn, vd, vs2, vs1)   RVV_BinopVX_Tpl(insn, vd, vs2, vs1, RVV_Mask(), ",v0")
#define RVV_BinopVXM_M  RVV_BinopVXM
#define RVV_BinopVIM(insn, vd, vs2, vs1)   RVV_BinopVI_Tpl(insn, vd, vs2, vs1, RVV_Mask(), ",v0")
#define RVV_BinopVIM_M  RVV_BinopVIM

// OPF
#define RVV_BinopVF(insn, vd, vs2, vs1)    RVV_BinopVF_Tpl(insn, vd, vs2, vs1, ,)
#define RVV_BinopVF_M(insn, vd, vs2, vs1)  RVV_BinopVF_Tpl(insn, vd, vs2, vs1, RVV_Mask(), ",v0.t")
#define RVV_BinopVF2(insn, vd, vs2, vs1)   RVV_BinopVF_Tpl2(insn, vd, vs2, vs1, ,)
#define RVV_BinopVF2_M(insn, vd, vs2, vs1) RVV_BinopVF_Tpl2(insn, vd, vs2, vs1, RVV_Mask(), ",v0.t")

#define RVV_BinopVFM(insn, vd, vs2, vs1)   RVV_BinopVF_Tpl(insn, vd, vs2, vs1, RVV_Mask(), ",v0")
#define RVV_BinopVFM_M  RVV_BinopVFM

/* Unop */
// OPI
#define RVV_UnopX(insn, vd, vs2, vs1)      RVV_UnopX_Tpl(insn, vd, vs2, vs1, ,)
#define RVV_UnopX_M                        RVV_UnopX
#define RVV_UnopI(insn, vd, vs2, vs1)      RVV_UnopI_Tpl(insn, vd, vs2, vs1, ,)
#define RVV_UnopI_M                        RVV_UnopI

// OPF
#define RVV_UnopF(insn, vd, vs2, vs1)      RVV_UnopF_Tpl(insn, vd, vs2, vs1, ,)
#define RVV_UnopF_M                        RVV_UnopF

/*---------------------------------------------------------------*/
/*--- OPI function templates                                  ---*/
/*---------------------------------------------------------------*/

#define RVV_NIL_FT(insn)

#define RVV_FT_DEF(insn, vv, vx, vi)   \
   RVV_##vv##_FT(insn)                 \
   RVV_##vx##_FT(insn)                 \
   RVV_##vi##_FT(insn)

#define RVV_FT_VAR_T(type, name, insn, op, body, args...)                      \
   UInt GETA_V##type##op##_M(name)(VexGuestRISCV64State * st, ##args)          \
   {                                                                           \
      RVV_##type##body##_M(VInsn##op(insn), vd, vs2, vs1);                     \
   }                                                                           \
   UInt GETA_V##type##op(name)(VexGuestRISCV64State * st, ##args)              \
   {                                                                           \
      RVV_##type##body(VInsn##op(insn), vd, vs2, vs1);                         \
   }

#define RVV_BinopOPI_FT_VAR_T(name, insn, op, body)                            \
   RVV_FT_VAR_T(Binop, name, insn, op, body,                                   \
                ULong vd, ULong vs2, ULong vs1, ULong mask)
#define RVV_BinopOPI_FT_VAR(insn, op, body)  RVV_BinopOPI_FT_VAR_T(insn, insn, op, body)

#define RVV_BinopSAT_FT_VAR(insn, op, body)                                    \
   RVV_FT_VAR_T(Binop, insn, insn, op, body,                                   \
                ULong vd, ULong vs2, ULong vs1, ULong mask, UInt xrm)

#define RVV_UnopOPI_FT_VAR_T(name, insn, op, body)                             \
   RVV_FT_VAR_T(Unop, name, insn, op, body,                                    \
                ULong vd, ULong vs2, ULong vs1, ULong mask)
#define RVV_UnopOPI_FT_VAR(insn, op, body) RVV_UnopOPI_FT_VAR_T(insn, insn, op, body)

/* Binop */
#define RVV_BinopOPIVV_FT(insn)     RVV_BinopOPI_FT_VAR(insn, VV, VV)
#define RVV_BinopOPIVX_FT(insn)     RVV_BinopOPI_FT_VAR(insn, VX, VX)
#define RVV_BinopOPIVI_FT(insn)     RVV_BinopOPI_FT_VAR(insn, VI, VI)
#define RVV_BinopOPIVV2_FT(insn)    RVV_BinopOPI_FT_VAR(insn, VV, VV2)
#define RVV_BinopOPIVX2_FT(insn)    RVV_BinopOPI_FT_VAR(insn, VX, VX2)
#define RVV_BinopOPIVS_FT(insn)     RVV_BinopOPI_FT_VAR(insn, VS, VV)

#define RVV_BinopOPIWV_FT(insn)     RVV_BinopOPI_FT_VAR_T(insn##w, insn, WV, VV)
#define RVV_BinopOPIWX_FT(insn)     RVV_BinopOPI_FT_VAR_T(insn##w, insn, WX, VX)

#define RVV_BinopOPIVVM_FT(insn)    RVV_BinopOPI_FT_VAR(insn, VVM, VVM)
#define RVV_BinopOPIVXM_FT(insn)    RVV_BinopOPI_FT_VAR(insn, VXM, VXM)
#define RVV_BinopOPIVIM_FT(insn)    RVV_BinopOPI_FT_VAR(insn, VIM, VIM)

#define RVV_BinopOPIMM_FT(insn)     RVV_BinopOPI_FT_VAR(insn, MM, VV)

#define RVV_BinopSATVV_FT(insn)     RVV_BinopSAT_FT_VAR(insn, VV, VV)
#define RVV_BinopSATVX_FT(insn)     RVV_BinopSAT_FT_VAR(insn, VX, VX)
#define RVV_BinopSATVI_FT(insn)     RVV_BinopSAT_FT_VAR(insn, VI, VI)
#define RVV_BinopSATVV2_FT(insn)    RVV_BinopSAT_FT_VAR(insn, VV, VV2)
#define RVV_BinopSATVX2_FT(insn)    RVV_BinopSAT_FT_VAR(insn, VX, VX2)

/* Unop */
#define RVV_UnopOPIM_FT(insn)       RVV_UnopOPI_FT_VAR(insn, M, V)

/*---------------------------------------------------------------*/
/*--- OPF function templates                                  ---*/
/*---------------------------------------------------------------*/

#define RVV_BinopOPF_FT_VAR_T(name, insn, op, body)                            \
   RVV_FT_VAR_T(Binop, name, insn, op, body,                                   \
                ULong vd, ULong vs2, ULong vs1, ULong mask, UInt frm)
#define RVV_BinopOPF_FT_VAR(insn, op, body)  RVV_BinopOPF_FT_VAR_T(insn, insn, op, body)

#define RVV_UnopOPF_FT_VAR_T(name, insn, op, body)                             \
   RVV_FT_VAR_T(Unop, name, insn, op, body,                                    \
                ULong vd, ULong vs2, ULong vs1, ULong mask, UInt frm)

/* Binop */
#define RVV_BinopOPFVV_FT(insn)     RVV_BinopOPF_FT_VAR(insn, VV, VV)
#define RVV_BinopOPFVF_FT(insn)     RVV_BinopOPF_FT_VAR(insn, VF, VF)
#define RVV_BinopOPFVV2_FT(insn)    RVV_BinopOPF_FT_VAR(insn, VV, VV2)
#define RVV_BinopOPFVF2_FT(insn)    RVV_BinopOPF_FT_VAR(insn, VF, VF2)
#define RVV_BinopOPFVS_FT(insn)     RVV_BinopOPF_FT_VAR(insn, VS, VV)

#define RVV_BinopOPFWV_FT(insn)     RVV_BinopOPF_FT_VAR_T(insn##w, insn, WV, VV)
#define RVV_BinopOPFWF_FT(insn)     RVV_BinopOPF_FT_VAR_T(insn##w, insn, WF, VF)

/* Unop */
#define RVV_UnopOPFV_FT(name, insn)  RVV_UnopOPF_FT_VAR_T(name, insn, V, V)
#define RVV_UnopOPFW_FT(name, insn)  RVV_UnopOPF_FT_VAR_T(name, insn, W, V)

/*---------------------------------------------------------------*/
/*--- Get function prototypes                                 ---*/
/*---------------------------------------------------------------*/

#pragma push_macro("RVV_FT_VAR_T")

#undef  RVV_FT_VAR_T
#define RVV_FT_VAR_T(type, name, insn, op, body, args...) \
    UInt GETA_V##type##op##_M(name)(VexGuestRISCV64State * st, ##args);  \
    UInt GETA_V##type##op(name)(VexGuestRISCV64State * st, ##args);      \

#include "guest_riscv64V_helpers.def"

#pragma push_macro("GETN_VOp")
#pragma push_macro("GETA_VOp")
#undef  GETN_VOp
#undef  GETA_VOp
#define GETN_VOp(insn, type, op, mask) "RVV0p7_"#type"_"#insn#op#mask
#define GETA_VOp(insn, type, op, mask) RVV0p7_##type##_##insn##op##mask
#include "guest_riscv64V0p7_helpers.def"
RVV_UnopOPI_FT_VAR(vid, V, V)
#pragma pop_macro("GETN_VOp")
#pragma pop_macro("GETA_VOp")

#pragma pop_macro("RVV_FT_VAR_T")

/*---------------------------------------------------------------*/
/*--- Get variant type of helper functions                    ---*/
/*---------------------------------------------------------------*/

#define GETV_VopUnknow  0
#define GETV_VopAccD    (1 << 0) /* Indicate VD  is readed   by OP, e.g. VFMACC  */
#define GETV_VopWidenD  (1 << 1) /* Indicate VD  is widened  by OP, e.g. VFWADD  */
#define GETV_VopWidenS2 (1 << 2) /* Indicate VS2 is widened  by OP, e.g. VFWADDW/VFNCVT */
#define GETV_VopM1D     (1 << 3) /* Indicate VD  is 1 LMUL   by OP, e.g. VMFEQ   */
#define GETV_VopM1S     (1 << 4) /* Indicate VS* is 1 LMUL   by OP, e.g. VMAND   */
#define GETV_VopMask    (1 << 5) /* Indicate V0  is readed   by OP, e.g. VADC    */

/*---------------------------------------------------------------*/
/*--- Get call of helper functions                            ---*/
/*---------------------------------------------------------------*/

#define isVOpVV(type) (type == RV64_SOPC_OPIVV || type == RV64_SOPC_OPMVV || type == RV64_SOPC_OPFVV)
#define isVOpVXorVF(type) (type == RV64_SOPC_OPIVX || type == RV64_SOPC_OPMVX || type == RV64_SOPC_OPFVF)
#define isVOpVI(type) (type == RV64_SOPC_OPIVI)

#define RVV_REGO offsetIReg64
#define RVV_REGN nameIReg

/* Binop */
#define GETC_VBinopOP_T(insn, V, X, I, ARGS, VARIANT, LMUL)                    \
   do {                                                                        \
      if (isVOpVV(GET_FUNCT3())) {                                             \
         fName = mask ? GETN_VBinopV##V(insn) : GETN_VBinopV##V##_M(insn);     \
         fAddr = mask ? GETA_VBinopV##V(insn) : GETA_VBinopV##V##_M(insn);     \
         temp  = offsetVReg(rs1);                                              \
      } else if (isVOpVXorVF(GET_FUNCT3())) {                                  \
         fName = mask ? GETN_VBinopV##X(insn) : GETN_VBinopV##X##_M(insn);     \
         fAddr = mask ? GETA_VBinopV##X(insn) : GETA_VBinopV##X##_M(insn);     \
         temp  = RVV_REGO(rs1);                                                \
      } else {                                                                 \
         fName = mask ? GETN_VBinopV##I(insn) : GETN_VBinopV##I##_M(insn);     \
         fAddr = mask ? GETA_VBinopV##I(insn) : GETA_VBinopV##I##_M(insn);     \
         temp  = ((Long) rs1 << 59) >> 59;                                     \
      }                                                                        \
      vassert(fAddr != NULL);                                                  \
      ARGS()                                                                   \
      d = unsafeIRDirty_1_N(ret, 0, fName, fAddr, args);                       \
      d = GETD_VBinop(d, rd, rs2, rs1, mask, GET_FUNCT3(), VARIANT, LMUL);     \
      stmt(irsb, IRStmt_Dirty(d));                                             \
                                                                               \
      if (isVOpVI(GET_FUNCT3()))                                               \
         DIP("%s(%s, %s, %u)\n", fName, nameVReg(rd), nameVReg(rs2), rs1);     \
      else                                                                     \
         DIP("%s(%s, %s, %s)\n", fName, nameVReg(rd), nameVReg(rs2),           \
             isVOpVV(GET_FUNCT3()) ? nameVReg(rs1) : RVV_REGN(rs1));           \
   } while (0)

// OPI
#define GETR_VBinopOPI()                                                       \
   args = mkIRExprVec_5(IRExpr_GSPTR(), mkU64(offsetVReg(rd)),                 \
                        mkU64(offsetVReg(rs2)), mkU64(temp),                   \
                        mkU64(offsetVReg(0)));

#define GETC_VBinopOPI_VAR(insn, v, x, i, var)                                 \
   GETC_VBinopOP_T(insn, v, x, i, GETR_VBinopOPI, var, lmul)
#define GETC_VBinopOPI(insn, v, x, i)                                          \
   GETC_VBinopOPI_VAR(insn, v, x, i, GETV_VopUnknow)

// SAT
#define GETR_VBinopSAT()                                                       \
   assign(irsb, xrm,                                                           \
          binop(Iop_And32, binop(Iop_Shr32, getFCSR(), mkU8(9)), mkU32(3)));   \
   args = mkIRExprVec_6(IRExpr_GSPTR(), mkU64(offsetVReg(rd)),                 \
                        mkU64(offsetVReg(rs2)), mkU64(temp),                   \
                        mkU64(offsetVReg(0)), mkexpr(xrm));

#define GETC_VBinopSAT_VAR(insn, v, x, i, var)                                 \
   GETC_VBinopOP_T(insn, v, x, i, GETR_VBinopSAT, var, lmul);                  \
   putVxsat0p7(irsb, mkexpr(ret));
#define GETC_VBinopSAT(insn, v, x, i)                                          \
   GETC_VBinopSAT_VAR(insn, v, x, i, GETV_VopUnknow)

// OPF
#define GETR_VBinopOPF()                                                       \
   assign(irsb, frm,                                                           \
          binop(Iop_And32, binop(Iop_Shr32, getFCSR(), mkU8(5)), mkU32(7)));   \
   args = mkIRExprVec_6(IRExpr_GSPTR(), mkU64(offsetVReg(rd)),                 \
                        mkU64(offsetVReg(rs2)), mkU64(temp),                   \
                        mkU64(offsetVReg(0)), mkexpr(frm));

#define GETC_VBinopOPF_VAR(insn, v, x, var)                                    \
   GETC_VBinopOP_T(insn, v, x, NIL, GETR_VBinopOPF, var, lmul);                \
   accumulateFFLAGS(irsb, mkexpr(ret));
#define GETC_VBinopOPF(insn, v, x)                                             \
   GETC_VBinopOPF_VAR(insn, v, x, GETV_VopUnknow)

/* Unop */
#define GETC_VUnopOP_T(insn, V, X, I, ARGS, VARIANT, LMUL)                     \
   do {                                                                        \
      if (isVOpVV(GET_FUNCT3())) {                                             \
         fName = mask ? GETN_VUnop##V(insn) : GETN_VUnop##V##_M(insn);         \
         fAddr = mask ? GETA_VUnop##V(insn) : GETA_VUnop##V##_M(insn);         \
         temp  = offsetVReg(rs2);                                              \
      } else if (isVOpVXorVF(GET_FUNCT3())) {                                  \
         fName = mask ? GETN_VUnop##X(insn) : GETN_VUnop##X##_M(insn);         \
         fAddr = mask ? GETA_VUnop##X(insn) : GETA_VUnop##X##_M(insn);         \
         temp  = RVV_REGO(rs1);                                                \
      } else {                                                                 \
         fName = mask ? GETN_VUnop##I(insn) : GETN_VUnop##I##_M(insn);         \
         fAddr = mask ? GETA_VUnop##I(insn) : GETA_VUnop##I##_M(insn);         \
         temp  = ((Long) rs1 << 59) >> 59;                                     \
      }                                                                        \
      vassert(fAddr != NULL);                                                  \
      ARGS()                                                                   \
      d = unsafeIRDirty_1_N(ret, 0, fName, fAddr, args);                       \
      d = GETD_VUnop(d, rd, isVOpVV(GET_FUNCT3()) ? rs2 : rs1, mask,           \
                     GET_FUNCT3(), VARIANT, LMUL);                             \
      stmt(irsb, IRStmt_Dirty(d));                                             \
                                                                               \
      if (isVOpVI(GET_FUNCT3()))                                               \
         DIP("%s(%s, %u)\n", fName, nameVReg(rd), rs1);                        \
      else                                                                     \
         DIP("%s(%s, %s)\n", fName, nameVReg(rd),                              \
             isVOpVV(GET_FUNCT3()) ? nameVReg(rs2) : RVV_REGN(rs1));           \
   } while (0)

// OPI
#define GETR_VUnopOPI()                                                        \
   args = mkIRExprVec_5(IRExpr_GSPTR(), mkU64(offsetVReg(rd)), mkU64(temp),    \
                        mkU64(offsetVReg(0)), mkU64(offsetVReg(0)));

#define GETC_VUnopOPI_VAR(insn, v, x, i, var)                                 \
   GETC_VUnopOP_T(insn, v, x, i, GETR_VUnopOPI, var, lmul)
#define GETC_VUnopOPI(insn, v, x, i)                                          \
   GETC_VUnopOPI_VAR(insn, v, x, i, GETV_VopUnknow)

// OPF
#define GETR_VUnopOPF()                                                        \
   assign(irsb, frm,                                                           \
          binop(Iop_And32, binop(Iop_Shr32, getFCSR(), mkU8(5)), mkU32(7)));   \
   args = mkIRExprVec_6(IRExpr_GSPTR(), mkU64(offsetVReg(rd)), mkU64(temp),    \
                        mkU64(offsetVReg(0)), mkU64(offsetVReg(0)), mkexpr(frm));

#define GETC_VUnopOPF_VAR(insn, v, x, var)                                    \
   GETC_VUnopOP_T(insn, v, x, NIL, GETR_VUnopOPF, var, lmul);                 \
   accumulateFFLAGS(irsb, mkexpr(ret));
#define GETC_VUnopOPF(insn, v, x)                                             \
   GETC_VUnopOPF_VAR(insn, v, x, GETV_VopUnknow)

#endif /* ndef __VEX_GUEST_RISCV64V_DEFS_H */

/*--------------------------------------------------------------------*/
/*--- end                                    guest_riscv64V_defs.h ---*/
/*--------------------------------------------------------------------*/
