#include "types.h"

#define FieldScene_RunScene39b_02000cc0 Func_02000cd0

void Func_02000e7a();
s32 Func_02006692();

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

s32 FieldScene_RunScene39b_02000cc0(s32 a0)
{
    s32 value;
    s32 magic;
    s32 base5_3001e40;
    s32 none;
    u8 storage[40];
    u8 *rec = storage;

    FIELD(rec, s32, 4) = 7;
    base5_3001e40 = 0x3001e40;
    if ((*(volatile s32 *)base5_3001e40 & 1) == 0) {
        FIELD(rec, s32, 4) = 5;
    }
    FIELD(rec, s32, 8) = 0xcccc;
    FIELD(rec, s32, 12) = 0xcccc;
    none = 0;
    FIELD(rec, s32, 0) = none;
    value = Func_02006692();
    magic = -((((((u32)(value << 3) >> 16) << 1) + ((u32)(value << 3) >> 16)) + (((((u32)(value << 3) >> 16) << 1) + ((u32)(value << 3) >> 16)) << 4)) + ((((((u32)(value << 3) >> 16) << 1) + ((u32)(value << 3) >> 16)) + (((((u32)(value << 3) >> 16) << 1) + ((u32)(value << 3) >> 16)) << 4)) << 8));
    Func_02000e7a((*(volatile s32 *)(a0 + 8) + ((8 - (*(volatile s32 *)base5_3001e40 & 15)) << 16)), (*(volatile s32 *)(a0 + 12) + 0x1a0000), *(volatile s32 *)(a0 + 16), none, magic, 0, 0xb0000, rec);
    return 0;
}
