#include "TYPES.H"
#include "SCENE.H"

u8 *BattleAction_Get(s32);

void Ability_RequestGlyph(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    Sys_SetRange(BattleAction_Get(arg0)[4], arg1, arg2, arg3, arg4);
}
