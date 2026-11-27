@ Battle effect dispatch, drawing and presentation setup.
	.include "games/tbs/src/system/stub_table.inc"
	.global btStubsBattleEffect
btStubsBattleEffect:
	stub btEffectBuildBlitterStub, btEffectBuildBlitter
	stub btEffectRunByIdRangeStub, btEffectRunByIdRange
	stub btEffectRunSummonStub, btEffectRunSummon
	stub btEffectRunFromTableStub, btEffectRunFromTable
	stub btEffectRunElementPillarStub, btEffectRunElementPillar
	stub btEffectRunShatteringOpeningStub, btEffectRunShatteringOpening
	stub btEffectRunElementBurstStub, btEffectRunElementBurst
	stub btEffectStartElementStreaksStub, btEffectStartElementStreaks
	stub btEffectEndElementStreaksStub, btEffectEndElementStreaks
