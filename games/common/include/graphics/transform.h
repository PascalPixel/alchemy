#ifndef ALCHEMY_TRANSFORM_H
#define ALCHEMY_TRANSFORM_H

#include "types.h"

extern s32 gTransform[12];

extern s32 gTransformStackDepth;
extern u8 *gTransformStackTop;

void SceneTransform_RewindStack(void);
void Graphics_SaveTransferWorkOnce(void);
void Graphics_SaveTransferWork(void *destination);
void Graphics_LoadTransferWork(const void *source);
void Graphics_PrepareTransfer(s32 *eye, s32 *target, s32 *out);
void Graphics_PrepareTransferInIwramWork(s32 *eye, s32 *target);
void Graphics_PrepareTransferAndRun(s32 *eye, s32 *target);

CAMELOT_ASM("eight routines share this fixed-register identity store")
#define Transform_SetIdentity(matrix)                                          \
    {                                                                          \
        CAMELOT_ASM("the identity row's four fixed registers")          \
        register s32 zero2_ __asm__("r3"), one_ __asm__("r1"), zero1_ __asm__("r2"), zero3_ __asm__("r4"); \
        CAMELOT_ASM("three stmia no C store or copy compiles to")       \
        __asm__ volatile("mov r0, %4\n\tmov %1, #128\n\tmov %2, #0\n\tmov %0, #0\n\t" \
                         "mov %3, #0\n\tlsl %1, %1, #9\n\t"                   \
                         "stmia r0!, {%1, %2, %0, %3}\n\t"                    \
                         "stmia r0!, {%1, %2, %0, %3}\n\t"                    \
                         "stmia r0!, {%1, %2, %0, %3}"                        \
                         : "=r"(zero2_), "=r"(one_), "=r"(zero1_), "=r"(zero3_) \
                         : "r"(matrix) : "r0", "memory");                     \
    }

#endif
