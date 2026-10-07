/* ported from games/THE BROKEN SEAL/SRC/GAME/CHARACTER/OWNERS.C */
#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "EDITION.H"
#include "GAME_FLAGS.H"
#include "PARTY_STATE.H"
#include "TYPES.H"
#include "BATTLE_SUMMON.H"
#include "RUNTIME_INTERFACES.H"
#include "FIXED_MATH.H"
#include "OWNER_STATE.H"
#include "IWRAM_CALL.H"

s32 GameFlag_SetBit(s32);

struct TradeFlags {
    u32 flags;
};

struct ElementLevelBonus {
    u16 power;
    u16 resist;
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



s32 Owner_GetDefaultElement(struct BattleUnit *state)
{
    const struct EnemyDefinition *record =
        (const struct EnemyDefinition *)Owner_GetRecord(state->class_id);
    u8 value = record->element_preset;

    if (value > 43)
        value = 0;
    return Enemy_ElementPresetTable[value].first;
}
