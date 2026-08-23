#include "fixed_math.h"
#include "types.h"
#include "global_cells.h"
#include "battle_party.h"
#include "battle_escape.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

struct BattleEscapeState {
    u8 padding_00[0x45];
    u8 guaranteed;
    u8 failed_attempts;
};

struct BattleUnitLevel {
    u8 padding_00[0x0f];
    u8 level;
};

struct BattleUnitLevel *Func_08077008(s32);
s32 Func_080022ec(s32, s32);
u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
extern u8 Data_02000240[];

s32 BattleEscape_CheckSuccess(void) {
    s32 escaped;
    u8 *failed_attempts;
    s16 living_units[14];
    s32 living_count;
    s32 level_total;
    s32 unit_index;
    s32 chance;
    struct BattleEscapeState *escape_state;

    escaped = 0;
    escape_state = *(struct BattleEscapeState **)ADDR_03001E74;
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
            level_total += Func_08077008(
                (s32)living_units[unit_index])->level;
        }
        chance += Func_080022ec(level_total * 0x1F4, living_count);
        living_count = BattleParty_ListLivingUnits(
            BATTLE_SIDE_ENEMIES,
            living_units);
        level_total = 0;
        for (unit_index = 0; unit_index < living_count; unit_index++) {
            level_total += Func_08077008(
                (s32)living_units[unit_index])->level;
        }
        chance -= Func_080022ec(level_total * 0x1F4, living_count);
        if ((chance > 0) &&
            ((u32)((u32)(0x2710 * Rand()) >> 0x10) < (u32)chance)) {
            escaped = 1;
        }
        *failed_attempts += 1;
    }
    if (Data_02000240[0x22B] == 2) {
        escaped = 0;
    }
    return escaped;
}
