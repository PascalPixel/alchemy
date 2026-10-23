@ Battle animation dispatch, drawing and presentation setup.
@ Ordered 8-byte jumps into this bank. Named exports are callable symbols;
@ bare veneers preserve entries that have no exported name.
#include "stub.inc"
	.global FarCall_EffectTable
FarCall_EffectTable:
	export BattleAnim_BuildBlitterFar, BattleAnim_BuildBlitter
	export BattleAnim_DispatchByIdRangeFar, BattleAnim_DispatchByIdRange
	export BattleAnim_RunSummonEffectFar, BattleAnim_RunSummonEffect
	export BattleAnim_RunTableEffectFar, BattleAnim_RunTableEffect
	export BattleAnim_RunElementPillarFar, BattleAnim_RunElementPillar
	export BattleAnim_RunShatteringOpeningFar, BattleAnim_RunShatteringOpening
	export BattleAnim_RunElementBurstFar, BattleAnim_RunElementBurst
	export BattleAnim_StartElementStreaksFar, BattleAnim_StartElementStreaks
	export BattleAnim_EndElementStreaksFar, BattleAnim_EndElementStreaks
