#ifndef ALCHEMY_TRANSFORM_H
#define ALCHEMY_TRANSFORM_H

#include "TYPES.H"

/* The resident runtime's current transform (SYSTEM/RUNTIME_MATRIX.S): nine
   16.16 coefficients followed by three translation components. */
extern s32 gTransform[12];

/* The stack the current transform is saved on: how many are saved, and the
   next free 48-byte slot of the block Render_ResetTransformState allocates. */
extern s32 gTransformStackDepth;
extern u8 *gTransformStackTop;

void SceneTransform_RewindStack(void);
void Graphics_SaveTransferWorkOnce(void);
void Graphics_SaveTransferWork(void *destination);
void Graphics_LoadTransferWork(const void *source);
void Graphics_PrepareTransfer(s32 *eye, s32 *target, s32 *out);
/* Builds the look-at transform from eye to target into the current one. */
void Graphics_PrepareTransferInIwramWork(s32 *eye, s32 *target);
/* Builds the look-at transform and applies it to the current one. */
void Graphics_PrepareTransferAndRun(s32 *eye, s32 *target);

/* Set the 3x3 transform to the identity and clear its translation. */
/* CAMELOT_ASM: eight routines share this fixed-register identity store */
#define Transform_SetIdentity(matrix)                                          \
    {                                                                          \
        /* CAMELOT_ASM: the identity row's four fixed registers */          \
        register s32 zero2_ __asm__("r3"), one_ __asm__("r1"), zero1_ __asm__("r2"), zero3_ __asm__("r4"); \
        /* CAMELOT_ASM: three stmia no C store or copy compiles to */       \
        __asm__ volatile("mov r0, %4\n\tmov %1, #128\n\tmov %2, #0\n\tmov %0, #0\n\t" \
                         "mov %3, #0\n\tlsl %1, %1, #9\n\t"                   \
                         "stmia r0!, {%1, %2, %0, %3}\n\t"                    \
                         "stmia r0!, {%1, %2, %0, %3}\n\t"                    \
                         "stmia r0!, {%1, %2, %0, %3}"                        \
                         : "=r"(zero2_), "=r"(one_), "=r"(zero1_), "=r"(zero3_) \
                         : "r"(matrix) : "r0", "memory");                     \
    }

#endif
