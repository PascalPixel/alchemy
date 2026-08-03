#include "resource_388.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_0200015c(s32);
u8 Func_0200017a(s32, s32);
void Func_0200017e(s32, s32, s32);
s32 Func_02000184(s32);
u8 Func_0200018e(s32, s32);
void *Func_02000190(s32);
void *Func_0200019c(s32);
u8 Func_0200019e(s32, s32, s32, s32, s32, s32);

s32 Func_02000054(void) {
    void *temp_r1;
    /* zero, arg5 and arg6 look gratuitous but are load-bearing: they keep the
       values out of the immediate operands of the stores/call so the allocator
       gives them their own registers. */
    s8 zero;
    s32 arg5;
    s32 arg6;
    /* Likewise arg1/arg2: initialising them here (outside the call's basic
       block) stops the call expander from pre-copying the two 24-bit
       constants ahead of the r0 argument. */
    s32 arg1 = 0xD80000;
    s32 arg2 = 0x880000;

    temp_r1 = *(void **)0x03001EBC;
    M2C_FIELD(temp_r1, s32 *, 0x1C0) = 0x204;
    M2C_FIELD(temp_r1, s32 *, 0x1C8) = 0x18;
    if (Func_0200015c(0x300) != 0) {
        Func_0200017e(8, arg1, arg2);
        Func_0200018e(8, 2);
        Func_0200017a(Func_02000184(8), 0);
        M2C_FIELD(Func_02000190(8), s8 *, 0x23) = 2;
        zero = 0;
        M2C_FIELD(Func_0200019c(8), s8 *, 0x59) = zero;
        arg5 = 0xB;
        arg6 = 6;
        Func_0200019e(0xB, 0x24, 5, 5, arg5, arg6);
    }
    return 0;
}
