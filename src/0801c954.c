#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_08016418(struct Work *work, s32 release);
s32 Func_08017394(void *arg0);
void Func_080030f8(s32);
s32 Func_08003f3c(u32 index);
void Func_08002dd8(s32);

void Func_0801c954(void) {
    u8 *r5;
    u8 *r6;
    u16 *p;

    r5 = *(u8 **)0x03001E9C;
    Func_08016418(M2C_FIELD(r5, s32 *, 0xFF4), 0);
    while (Func_08017394(M2C_FIELD(r5, s32 *, 0xFF4)) == 0) {
        Func_080030f8(1);
    }
    if (M2C_FIELD(r5, u16 *, 0x46) != 0) {
        Func_08003f3c(M2C_FIELD(r5, u16 *, 0x48));
    }
    p = (u16 *)(r5 + 0x352);
    if (*p != 0) {
        p = (u16 *)((u8 *)p + 2);
        Func_08003f3c(*p);
    }
    Func_08002dd8(0x13);
}
