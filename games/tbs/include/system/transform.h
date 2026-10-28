#ifndef ALCHEMY_TRANSFORM_H
#define ALCHEMY_TRANSFORM_H

/// @file
/// The 3D transform and its stack.

#include "edition.h"
#include "types.h"

/// The current 3D transform, twelve 16.16 words: a 3x3 rotation, then a
/// translation. Iwram_TransformMatrix multiplies the matrix it is given into
/// it.
extern s32 gTransform[12];

/// Matrices saved by Transform_Push, 0 or 1: the save area holds one.
extern s32 gTransformStackDepth;
/// Where Transform_Push saves gTransform next: the save area in heap slot
/// HEAP_SLOT_TRANSFORM_STACK, or just past it while a matrix is saved.
extern u8 *gTransformStackTop;

void Transform_RewindStack(void);

/// Sets a twelve-word matrix to identity: Q16_ONE (1.0) on the rotation's
/// diagonal, the rest 0.
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

/// The games order the identity-register setup differently before the same
/// stores.
#define TRANSFORM_IDENTITY_REGISTER_SETUP                                      \
  "mov r0, %1\n\tmov r1, #128\n\tmov r2, #0\n\tmov %0, #0\n\t"                 \
  "mov r4, #0\n\tlsl r1, r1, #9\n\t"

/// Resets the current matrix to identity.
static __inline__ void Transform_ResetCurrent(void) {
  s32 zero;

  CAMELOT_ASM(
      "Transform_SetIdentity's three stmia, applied to the current matrix")
  __asm__ volatile(TRANSFORM_IDENTITY_REGISTER_SETUP
                   "stmia r0!, {r1, r2, %0, r4}\n\t"
                   "stmia r0!, {r1, r2, %0, r4}\n\t"
                   "stmia r0!, {r1, r2, %0, r4}"
                   : "=l"(zero)
                   : "r"(gTransform)
                   : "r0", "r1", "r2", "r4", "memory");
}

#undef TRANSFORM_IDENTITY_REGISTER_SETUP

#endif
