#ifndef ALCHEMY_TRANSFORM_H
#define ALCHEMY_TRANSFORM_H

#include "TYPES.H"

/* The resident runtime's current transform (SYSTEM/RUNTIME_MATRIX.S): three
   rows of 16.16 coefficients, each followed by its translation. */
extern s32 gTransform[12];

/* The stack the current transform is saved on: how many are saved, and the
   next free 48-byte slot of the block Render_ResetTransformState allocates. */
extern s32 gTransformStackDepth;
extern u8 *gTransformStackTop;

/* Loads the identity into a 12-word transform: each row 0x10000, 0, 0 and a
   zero translation. Eight routines (the transform stack's resets at
   0x080049ac-0x08004d04 and the battle transition entries) store it through
   the same fixed r0-r4, three stmia r0!, {r1, r2, r3, r4} after copying the
   matrix address into r0, and use r4 without saving it. GCC 2.96 emits
   neither for C stores or structure copies, so Camelot wrote this macro. */
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
