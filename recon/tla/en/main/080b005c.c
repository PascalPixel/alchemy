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
#include "btlsmn.h"
#include "runtime_interfaces.h"
#include "fixed_math.h"
#include "owner_state.h"
#include "iwram_call.h"

s32 GameFlag_SetBit(s32);

struct TradeFlags {
    u32 flags;
};

struct ElementLevelBonus {
    u16 pow;
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



s32 Owner_GetDefaultElement(struct BtlUnit *state)
{
    const struct EnemyDefinition *record =
        (const struct EnemyDefinition *)Owner_GetRecord(state->class_id);
    u8 value = record->element_preset;

    if (value > 43)
        value = 0;
    return Enemy_ElementPresetTable[value].first;
}
