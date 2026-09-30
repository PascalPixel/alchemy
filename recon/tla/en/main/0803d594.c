/*
 * Draft: Ability_RequestGlyph does not yet match; 3 halfwords differ from ☀️'s C, first at +0x18 (ldrh r0, [r0, #4]).
 * Links as recon/tla/raw/0803d4e4.s.
 */
#include "TYPES.H"

void UiIcon_BuildItemIconTiles(s32, s32, s32, s32, s32);

u8 *BattleAction_Get(s32);

void Ability_RequestGlyph(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    UiIcon_BuildItemIconTiles(BattleAction_Get(arg0)[4], arg1, arg2, arg3, arg4);
}
