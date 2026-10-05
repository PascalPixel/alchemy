#include "TYPES.H"

struct BattleActionGlyph {
    u8 unknown_00[4];
    u16 icon;
};

struct BattleActionGlyph *BattleAction_Get(s32 action);
void UiIcon_BuildAbilityIconTiles(s32 icon, s32 with_base, s32 *slot, s32 *tile, s32 reuse);

void Ability_LoadGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile, s32 reuse)
{
    UiIcon_BuildAbilityIconTiles(BattleAction_Get(action)->icon, with_base, slot, tile, reuse);
}
