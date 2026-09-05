#include "types.h"

u8 *Ability_GetData(s32);
void Func_08019fcc(s32, s32, s32, s32, s32);

void Ability_RequestGlyph(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    Func_08019fcc(Ability_GetData(arg0)[4], arg1, arg2, arg3, arg4);
}
