#ifndef ALCHEMY_BATTLE_EFFECT_COUNTERS_H
#define ALCHEMY_BATTLE_EFFECT_COUNTERS_H

#include "LAYOUT_GUARD.H"
#include "TYPES.H"

#include "BATTLE_EFFECT_RUNTIME.H"

struct BattleEffectCounterState {
    u8 padding_000[0x16c];
    s16 counters[12];
};

LAYOUT_OFFSET_GUARD(
    BattleEffectCounterState_Counters,
    struct BattleEffectCounterState,
    counters,
    0x16c);
LAYOUT_SIZE_GUARD(
    BattleEffectCounterState_Size, struct BattleEffectCounterState, 0x184);


#endif
