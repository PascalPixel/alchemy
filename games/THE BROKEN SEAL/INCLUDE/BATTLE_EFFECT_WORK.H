#ifndef ALCHEMY_BATTLE_EFFECT_WORK_H
#define ALCHEMY_BATTLE_EFFECT_WORK_H

#include "LAYOUT_GUARD.H"
#include "TYPES.H"

struct BattleEffectWork {
    u8 unknown_0000[0x7780];
    s32 transfer_mode;
    s32 transfer_value;
    u8 unknown_7788[0x9c];
    s32 transfer_pending;
    void *effect;
};

LAYOUT_OFFSET_GUARD(BattleEffectWork_TransferMode,
    struct BattleEffectWork, transfer_mode, 0x7780);
LAYOUT_OFFSET_GUARD(BattleEffectWork_TransferValue,
    struct BattleEffectWork, transfer_value, 0x7784);
LAYOUT_OFFSET_GUARD(BattleEffectWork_TransferPending,
    struct BattleEffectWork, transfer_pending, 0x7824);
LAYOUT_OFFSET_GUARD(BattleEffectWork_Effect,
    struct BattleEffectWork, effect, 0x7828);

#endif
