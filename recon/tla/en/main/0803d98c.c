#include "TYPES.H"
#include "SCENE.H"
void UiIcon_BuildItemIconTiles(s32, s32, s32, s32, s32);

u8 *BattleAction_Get(s32);

void Ability_RequestGlyph(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    UiIcon_BuildItemIconTiles(BattleAction_Get(arg0)[4], arg1, arg2, arg3, arg4);
}
