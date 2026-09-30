#include "TYPES.H"
#include "SCENE.H"
#include "FIXED_MATH.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_ESCAPE.H"
s32 Battle_CollectPartyCommandsFar(void *entries, u16 *excluded_units, s32 excluded_count);
void Runtime_BumpFree(void *ptr);
extern u8 Data_03001e74[];
s32 BattleParty_ListActorIds(s32 groups, u16 *ids);

/* battle/actor/clear_field_12b_for_group.c */
u8 *Owner_GetStateFar(s32);
void Owner_RecalculateStatsFar(u16 id);

struct ActorState_080b90ac {
    u8 padding_000[0x12b];
    u8 field_12b;
};

s32 BattleEscape_CheckSuccess(void)
{
    s32 escaped;
    u8 *failed_attempts;
    s16 living_units[14];
    s32 living_count;
    s32 level_total;
    s32 unit_index;
    s32 chance;
    struct BattleEscapeState *escape_state;

    escaped = 0;
    escape_state = *(struct BattleEscapeState **)((u32)&Data_03001e74);
    if (escape_state->guaranteed == 1) {
        escaped = 1;
    } else {
        failed_attempts = &escape_state->failed_attempts;
        chance = 0x1388 + (escape_state->failed_attempts * 0x7D0);
        living_count = BattleParty_ListLivingUnits(
            BATTLE_SIDE_PARTY,
            living_units);
        level_total = 0;
        for (unit_index = escaped; unit_index < living_count; unit_index++) {
            level_total += Owner_GetStateFar(
                (s32)living_units[unit_index])[0x0f];
        }
        chance += Math_Div(level_total * 0x1F4, living_count);
        living_count = BattleParty_ListLivingUnits(
            BATTLE_SIDE_ENEMIES,
            living_units);
        level_total = 0;
        for (unit_index = 0; unit_index < living_count; unit_index++) {
            level_total += Owner_GetStateFar(
                (s32)living_units[unit_index])[0x0f];
        }
        chance -= Math_Div(level_total * 0x1F4, living_count);
        if ((chance > 0) &&
            ((u32)((u32)(0x2710 * Random16()) >> 0x10) < (u32)chance)) {
            escaped = 1;
        }
        *failed_attempts += 1;
    }
    if (gGameState[0x22B] == 2) {
        escaped = 0;
    }
    return escaped;
}
