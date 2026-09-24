#include "FIXED_MATH.H"
#include "TYPES.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_TARGET.H"
#include "SYSTEM.H"

s32 BattleTarget_ReplaceDefeated(const u8 *action)
{
    s16 living_units[14];
    s32 target_id;
    s32 living_count;

    target_id = ((const s16 *)action)[5];
    if (BattleUnit_Get(target_id)->hp != 0) {
        return target_id;
    }

    if (target_id > 0x7F) {
        living_count = BattleParty_ListLivingUnits(
            BATTLE_SIDE_ENEMIES,
            living_units);
    } else {
        living_count = BattleParty_ListLivingUnits(
            BATTLE_SIDE_PARTY,
            living_units);
    }

    if (living_count == 0) {
        return 0x100;
    }

    return living_units[(u32)(Random16() * living_count) >> 0x10];
}
