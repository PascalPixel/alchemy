@ Battle effect dispatch, drawing and presentation setup.
	.syntax unified
	.thumb
	.include "games/tla/src/system/stub.s"
	.global Stubs_BattleEffect
Stubs_BattleEffect:
	stub btEffectBuildBlitterStub, btEffectBuildBlitter
	stub btEffectRunByIdRangeStub, btEffectRunByIdRange
	stub btEffectRunSummonStub, btEffectRunSummon
	stub btEffectRunFromTableStub, btEffectRunFromTable
	stub btEffectRunElementPillarStub, btEffectRunElementPillar
	stub btEffectRunShatteringOpeningStub, btEffectRunShatteringOpening
	stub btEffectRunElementBurstStub, btEffectRunElementBurst
	stub btEffectStartElementStreaksStub, btEffectStartElementStreaks
	stub btEffectEndElementStreaksStub, btEffectEndElementStreaks
	stub btEffectLoadBlitterStub, btEffectLoadBlitter
	stub btEffectRunFormChangeStub, btEffectRunFormChange
