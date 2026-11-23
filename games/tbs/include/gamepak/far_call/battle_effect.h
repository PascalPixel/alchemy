#ifndef ALCHEMY_FAR_CALL_BATTLE_EFFECT_H
#define ALCHEMY_FAR_CALL_BATTLE_EFFECT_H

/// @file
/// The battle effects bank's far calls: the Psynergy, summon and element
/// effects a battle plays.

s32 BattleEffect_BuildBlitterFar(s32 slot, s32 widthShift, s32 heightShift,
                                 s32 flags, u32 mode);
void BattleEffect_RunByIdRangeFar(struct BattleEffectParams *args);
void BattleEffect_RunSummonFar(struct BattleEffectParams *args);
void BattleEffect_RunFromTableFar(struct BattleEffectParams *args);
void BattleEffect_RunElementPillarFar(struct BattleEffectParams *args);
void BattleEffect_RunShatteringOpeningFar(struct BattleEffectParams *args);
void BattleEffect_RunElementBurstFar(struct BattleEffectParams *args);
void BattleEffect_StartElementStreaksFar(s32 element);
void BattleEffect_EndElementStreaksFar(void);

#endif
