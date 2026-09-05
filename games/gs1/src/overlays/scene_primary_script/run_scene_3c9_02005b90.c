#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
s32 Func_0200b7ce();
s32 Func_0200b7dc();

void FieldScene_RunScene3c9_02005b90(u8 *a0)
{
    s32 p10;
    s32 p8;
    s32 p8b;
    s32 record;
    s32 value;

    p10 = *(s32 *)(a0 + 104);
    p8 = *(u16 *)(a0 + 100);
    record = Func_0200b7ce(p8);
    *(volatile s32 *)(a0 + 8) = (*(s32 *)(p10 + 8) + (record * (*(s32 *)(a0 + 48) + 28)));
    value = Func_0200b7dc(p8);
    *(volatile s32 *)(a0 + 16) = ((value << 4) + 0xa40000);
    *(volatile s32 *)(a0 + 56) = *(s32 *)(a0 + 8);
    *(volatile s32 *)(a0 + 64) = ((value << 4) + 0xa40000);
    p8b = *(volatile u16 *)(a0 + 100);
    p8b = p8b + -0x200;
    *(volatile u16 *)(a0 + 100) = (u16)p8b;
}
