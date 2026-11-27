#ifndef ALCHEMY_BATTLE_EFFECT_STUBS_H
#define ALCHEMY_BATTLE_EFFECT_STUBS_H

/* @file */
/* The battle effects module's stub calls: the Psynergy, summon and element */
/* effects a battle plays. */

s32 btEffectBuildBlitterStub(s32 slot, s32 widthShift, s32 heightShift,
							 s32 flags, u32 mode);
void btEffectRunByIdRangeStub(struct BATTLE_EFFECT_PARM *args);
void btEffectRunSummonStub(struct BATTLE_EFFECT_PARM *args);
void btEffectRunFromTableStub(struct BATTLE_EFFECT_PARM *args);
void btEffectRunElementPillarStub(struct BATTLE_EFFECT_PARM *args);
void btEffectRunShatteringOpeningStub(struct BATTLE_EFFECT_PARM *args);
void btEffectRunElementBurstStub(struct BATTLE_EFFECT_PARM *args);
void btEffectStartElementStreaksStub(s32 element);
void btEffectEndElementStreaksStub(void);

#endif
