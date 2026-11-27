#ifndef ALCHEMY_BATTLE_EFFECT_STUBS_H
#define ALCHEMY_BATTLE_EFFECT_STUBS_H

// The battle effects module's stub calls: the Psynergy, summon and element effects a battle plays.

Sint32 btEffectBuildBlitterStub(
	Sint32 slot, Sint32 widthShift, Sint32 heightShift, Sint32 flags, Uint32 mode);
void btEffectRunByIdRangeStub(struct BATTLE_EFFECT_PARM *args);
void btEffectRunSummonStub(struct BATTLE_EFFECT_PARM *args);
void btEffectRunFromTableStub(struct BATTLE_EFFECT_PARM *args);
void btEffectRunElementPillarStub(struct BATTLE_EFFECT_PARM *args);
void btEffectRunShatteringOpeningStub(struct BATTLE_EFFECT_PARM *args);
void btEffectRunElementBurstStub(struct BATTLE_EFFECT_PARM *args);
void btEffectStartElementStreaksStub(Sint32 element);
void btEffectEndElementStreaksStub(void);

#endif
