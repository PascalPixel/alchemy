/* ported from games/tbs/src/game/character/owners.c */
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "game_flags.h"
#include "party_state.h"
#include "types.h"
#include "battle_summon.h"
#include "runtime_interfaces.h"
#include "fixed_math.h"
#include "owner_state.h"
#include "iwram_call.h"

s32 GameFlag_SetBit(s32);

struct TradeFlags {
    u32 flags;
};

struct ElementLevelBonus {
    u16 power;
    u16 res;
};

extern const struct ElementLevelBonus Element_PowerResistByLevel[16];

s32 GameFlag_Test(s32 flag);

#if EDITION_INTERNATIONAL
#define ENEMY_SUFFIX_FIRST '1'
#define ENEMY_SUFFIX_LAST 8
#else
#define ENEMY_SUFFIX_FIRST 'A'
#define ENEMY_SUFFIX_LAST 25
#endif

typedef s32 (*ClearWordsFn)(void *destination, s32 size);



u32 Owner_CalculateBaseElements(s32 owner, s16 destination[4][2])
{
    struct BattleUnit *state = (struct BattleUnit *)UnitGet(owner);
    u32 index;
    u32 result;
    s32 values[4];
    s32 i;

    if (state->class_index == 0) {
        index = ((const struct EnemyDefinition *)Owner_GetRecord(state->class_id))->element_preset;
        if (index > 43)
            index = 0;

        i = 0;
        for (;;) {
            ((s32 *)destination)[i] =
                Enemy_ElementPresetTable[index].values[i];
            i++;
            if (i > 3)
                goto copied;
        }
copied:
        return index;
    }

    Owner_GetDigitValues(state->class_id, (u8 *)state->djinn_available, values);
    i = 0;
    do {
        s32 value;
        s32 ones;
        s32 tens;

        result = (u32)Element_PowerResistByLevel;
        value = values[i];
        ones = value % 10;
        tens = value / 10;

        if (tens > 15)
            tens = 15;
        if (tens < 0)
            tens = 0;

        destination[i][0] = Element_PowerResistByLevel[tens].power + ones;
        destination[i][1] =
            ((volatile const struct ElementLevelBonus *)Element_PowerResistByLevel)[tens].res + ones;
        i++;
    } while (i < 4);
    return result;
}
