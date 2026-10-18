@ Battle-effect dispatch, drawing and presentation setup.
@ Ordered 8-byte jumps into this bank. Named exports are callable symbols;
@ bare veneers preserve entries that have no exported name.
#include "stub.inc"
	.global FarCall_EffectTable
FarCall_EffectTable:
	export BattleEffect_LoadWorkFar, BattleFx_BuildBlitter
	export BattleFx_DispatchByIdRangeFar, BattleFx_DispatchByIdRange
	export BattleFx_RunSceneEffectFar, BattleFx_RunSceneEffect
	export BattleFx_RunTableEffectFar, BattleFx_RunTableEffect
	export BattleFx_RunElementPillarFar, BattleFx_RunElementPillar
	export BattleFx_RunShatteringOpeningFar, BattleFx_RunShatteringOpening
	export BattleFx_RunElementBurstFar, BattleFx_RunElementBurst
	export BattleFx_StartElementStreaksFar, BattleFx_StartElementStreaks
	export BattleFx_EndElementStreaksFar, BattleFx_EndElementStreaks
