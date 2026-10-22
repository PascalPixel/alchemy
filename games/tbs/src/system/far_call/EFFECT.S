@ Battle-effect dispatch, drawing and presentation setup.
@ Ordered 8-byte jumps into this bank. Named exports are callable symbols;
@ bare veneers preserve entries that have no exported name.
#include "stub.inc"
	.global FarCall_EffectTable
FarCall_EffectTable:
	export BattleEffect_BuildBlitterFar, BattleEffect_BuildBlitter
	export BattleEffect_DispatchByIdRangeFar, BattleEffect_DispatchByIdRange
	export BattleEffect_RunSceneEffectFar, BattleEffect_RunSceneEffect
	export BattleEffect_RunTableEffectFar, BattleEffect_RunTableEffect
	export BattleEffect_RunElementPillarFar, BattleEffect_RunElementPillar
	export BattleEffect_RunShatteringOpeningFar, BattleEffect_RunShatteringOpening
	export BattleEffect_RunElementBurstFar, BattleEffect_RunElementBurst
	export BattleEffect_StartElementStreaksFar, BattleEffect_StartElementStreaks
	export BattleEffect_EndElementStreaksFar, BattleEffect_EndElementStreaks
