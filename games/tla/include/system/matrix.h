#ifndef ALCHEMY_MATRIX_H
#define ALCHEMY_MATRIX_H

/// @file
/// The current 3D matrix, its one-deep stack, and the unit matrix.

#include "edition.h"
#include "fixed_math.h"
#include "types.h"

/// A transform's twelve 16.16 words: a 3x3 rotation, then the translation's
/// x, y and z.
enum {
  MATRIX_TRANSLATION_X = 9,
  MATRIX_TRANSLATION_Y = 10,
  MATRIX_TRANSLATION_Z = 11,
  MATRIX_WORDS = 12
};

/// A matrix of twelve 16.16 words, as SGL types one; ours holds the
/// rotation's nine and then the translation's three.
typedef FIXED MATRIX[MATRIX_WORDS];

/// The current 3D transform. Iwram_MultiplyMatrix multiplies the matrix it is
/// given into it.
extern MATRIX gCurrentMatrix;

/// Matrices saved by Matrix_Push, 0 or 1: the save area holds one.
extern s32 gMatrixStackDepth;
/// Where Matrix_Push saves gCurrentMatrix next: the save area in heap slot
/// HEAP_SLOT_MATRIX_STACK, or just past it while a matrix is saved.
extern u8 *gMatrixStackTop;

void Matrix_RewindStack(void);
void Matrix_Push(void);
void Matrix_Get(void *destination);
void Matrix_Load(const void *source);
void Matrix_BuildLookAt(FIXED *eye, FIXED *target, FIXED *out);
void Matrix_SetLookAt(FIXED *eye, FIXED *target);
void Matrix_LookAt(FIXED *eye, FIXED *target);

/// Sets a twelve-word matrix to the unit matrix: FIXED_ONE (1.0) on the
/// rotation's diagonal, the rest 0.
static __inline__ void Matrix_SetUnit(void *matrix) {
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

/// The unit matrix's three values set up in Matrix_SetCurrentUnit's order,
/// which differs from Matrix_SetUnit's.
#define MATRIX_UNIT_REGISTER_SETUP                                             \
  "mov r0, %1\n\tmov r1, #128\n\tlsl r1, r1, #9\n\tmov r2, #0\n\t"             \
  "mov %0, #0\n\tmov r4, #0\n\t"

/// Resets the current matrix to the unit matrix.
static __inline__ void Matrix_SetCurrentUnit(void) {
  s32 zero;

  CAMELOT_ASM("Matrix_SetUnit's three stmia, applied to the current matrix")
  __asm__ volatile(MATRIX_UNIT_REGISTER_SETUP "stmia r0!, {r1, r2, %0, r4}\n\t"
                                              "stmia r0!, {r1, r2, %0, r4}\n\t"
                                              "stmia r0!, {r1, r2, %0, r4}"
                   : "=l"(zero)
                   : "r"(gCurrentMatrix)
                   : "r0", "r1", "r2", "r4", "memory");
}

#undef MATRIX_UNIT_REGISTER_SETUP

#endif
