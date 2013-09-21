#ifndef _ENV_PHYSICAL_SINGLE_CORE_H
#define _ENV_PHYSICAL_SINGLE_CORE_H

#include "../pcr.h"

//-----------------------------------------------------------------------
// Begin Macro
//-----------------------------------------------------------------------

#define RVTEST_RV64U                                                    \
  .macro init;                                                          \
  .endm

#define RVTEST_RV64UF                                                   \
  .macro init;                                                          \
  RVTEST_FP_ENABLE;                                                     \
  .endm

#define RVTEST_RV64UV                                                   \
  .macro init;                                                          \
  RVTEST_FP_ENABLE;                                                     \
  RVTEST_VEC_ENABLE;                                                    \
  .endm

#define RVTEST_RV32U                                                    \
  .macro init;                                                          \
  RVTEST_32_ENABLE;                                                     \
  .endm

#define RVTEST_RV32UF                                                   \
  .macro init;                                                          \
  RVTEST_32_ENABLE;                                                     \
  RVTEST_FP_ENABLE;                                                     \
  .endm

#define RVTEST_RV32UV                                                   \
  .macro init;                                                          \
  RVTEST_32_ENABLE;                                                     \
  RVTEST_FP_ENABLE;                                                     \
  RVTEST_VEC_ENABLE;                                                    \
  .endm

#define RVTEST_RV64S                                                    \
  .macro init;                                                          \
  .endm

#define RVTEST_32_ENABLE                                                \
  clearpcr status, SR_S64                                               \

#define RVTEST_FP_ENABLE                                                \
  setpcr status, SR_EF;                                                 \
  mfpcr a0, status;                                                     \
  and   a0, a0, SR_EF;                                                  \
  bnez  a0, 2f;                                                         \
  RVTEST_PASS;                                                          \
2:fssr x0;                                                              \

#define RVTEST_VEC_ENABLE                                               \
  setpcr status, SR_EV;                                                 \
  mfpcr a0, status;                                                     \
  and   a0, a0, SR_EV;                                                  \
  bnez  a0, 2f;                                                         \
  RVTEST_PASS;                                                          \
2:                                                                      \

#define RISCV_MULTICORE_DISABLE                                         \
        mfpcr a0, hartid; 1: bnez a0, 1b;                               \

#define EXTRA_INIT

#define RVTEST_CODE_BEGIN                                               \
        .text;                                                          \
        .align  4;                                                      \
        .global _start;                                                 \
_start:                                                                 \
        RISCV_MULTICORE_DISABLE;                                        \
        init;                                                           \
        EXTRA_INIT;                                                     \

//-----------------------------------------------------------------------
// End Macro
//-----------------------------------------------------------------------

#define RVTEST_CODE_END                                                 \

//-----------------------------------------------------------------------
// Pass/Fail Macro
//-----------------------------------------------------------------------

#define RVTEST_PASS                                                     \
        fence;                                                          \
        li  x1, 1;                                                      \
        mtpcr x1, tohost;                                               \
1:      b 1b;                                                           \

#define RVTEST_FAIL                                                     \
        fence;                                                          \
        beqz x28, 1f;                                                   \
        sll x28, x28, 1;                                                \
        or x28, x28, 1;                                                 \
        mtpcr x28, tohost;                                              \
1:      b 1b;                                                           \

//-----------------------------------------------------------------------
// Data Section Macro
//-----------------------------------------------------------------------

#define RVTEST_DATA_BEGIN EXTRA_DATA
#define RVTEST_DATA_END

#define EXTRA_DATA

//#define RVTEST_DATA_BEGIN .align 4; .global begin_signature; begin_signature:
//#define RVTEST_DATA_END .align 4; .global end_signature; end_signature:

#endif