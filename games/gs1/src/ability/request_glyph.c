#include "types.h"
#include "scene.h"
#include "abi/ability/request_glyph.h"

u8 *Ability_GetData(s32);

void Ability_RequestGlyph(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    Sys_SetRange(Ability_GetData(arg0)[4], arg1, arg2, arg3, arg4);
}
