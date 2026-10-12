#include "types.h"

void Func_0803d5c4(s32, s32, s32, s32, s32);

u8 *BtlActGet(s32);

void Ability_RequestGlyph(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    Func_0803d5c4(*(u16 *)(BtlActGet(arg0) + 4), arg1, arg2, arg3, arg4);
}
