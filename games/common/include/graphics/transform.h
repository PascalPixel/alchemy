#ifndef ALCHEMY_TRANSFORM_H
#define ALCHEMY_TRANSFORM_H

#include "edition.h"
#include "types.h"

extern s32 gTransform[12];

extern s32 gTransformStackDepth;
extern u8 *gTransformStackTop;

void SceneTransform_RewindStack(void);
#if GAME_TLA
void Transform_Push(void);
void Transform_Store(void *destination);
void Transform_Load(const void *source);
void Transform_BuildLookAt(s32 *eye, s32 *target, s32 *out);
void Transform_SetLookAt(s32 *eye, s32 *target);
void Transform_ApplyLookAt(s32 *eye, s32 *target);
#endif

static __inline__ void Transform_SetIdentity(void *matrix) {
  CAMELOT_ASM("three stmia no C store or copy compiles to")
  __asm__ volatile("mov r0, %0\n\tmov r1, #128\n\tmov r2, #0\n\tmov r3, #0\n\t"
                   "mov r4, #0\n\tlsl r1, r1, #9\n\t"
                   "stmia r0!, {r1, r2, r3, r4}\n\t"
                   "stmia r0!, {r1, r2, r3, r4}\n\t"
                   "stmia r0!, {r1, r2, r3, r4}"
                   :
                   : "r"(matrix)
                   : "r0", "r1", "r2", "r3", "r4", "memory");
}

// Resets the current matrix to identity.
static __inline__ void Transform_ResetCurrent(void) {
  s32 zero;

#if GAME_TLA
  CAMELOT_ASM(
      "Transform_SetIdentity's three stmia, applied to the current matrix")
  __asm__ volatile(
      "mov r0, %1\n\tmov r1, #128\n\tlsl r1, r1, #9\n\tmov r2, #0\n\t"
      "mov %0, #0\n\tmov r4, #0\n\t"
      "stmia r0!, {r1, r2, %0, r4}\n\t"
      "stmia r0!, {r1, r2, %0, r4}\n\t"
      "stmia r0!, {r1, r2, %0, r4}"
      : "=l"(zero)
      : "r"(gTransform)
      : "r0", "r1", "r2", "r4", "memory");
#else
  CAMELOT_ASM(
      "Transform_SetIdentity's three stmia, applied to the current matrix")
  __asm__ volatile("mov r0, %1\n\tmov r1, #128\n\tmov r2, #0\n\tmov %0, #0\n\t"
                   "mov r4, #0\n\tlsl r1, r1, #9\n\t"
                   "stmia r0!, {r1, r2, %0, r4}\n\t"
                   "stmia r0!, {r1, r2, %0, r4}\n\t"
                   "stmia r0!, {r1, r2, %0, r4}"
                   : "=l"(zero)
                   : "r"(gTransform)
                   : "r0", "r1", "r2", "r4", "memory");
#endif
}

#endif
