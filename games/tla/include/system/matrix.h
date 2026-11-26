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

// Its assembly is in system/asm.h, with the rest.
#define ASM_PART_MATRIX
#include "system/asm.h"

#endif
