@ Battle-effect dispatch, drawing and presentation setup.
@ Ordered 8-byte jumps into this bank. Named exports are callable symbols;
@ bare veneers preserve entries that have no exported name.
#include "stub.inc"
	.global FarCall_EffectTable
FarCall_EffectTable:
	export BattleEffect_LoadWorkFar, BattleEffect_LoadWork
	export BattleFx_DispatchByIdRangeFar, BattleFx_DispatchByIdRange
	export BattleFx_RunSceneEffectFar, BattleFx_RunSceneEffect
	export BattleFx_RunTableEffectFar, BattleFx_RunTableEffect
	export BattleFx_RunIceFleckBurstFar, BattleFx_RunIceFleckBurst
	export BattleEffect_RunTileAndPaletteAnimationFar, BattleEffect_RunTileAndPaletteAnimation
	export BattleFx_RunGatheringBurstFar, BattleFx_RunGatheringBurst
	export BattlePresentation_PrepareSceneFar, BattlePresentation_PrepareScene
	export BattleFx_ScheduleCallbacksAndReleaseBlocksFar, BattleFx_ScheduleCallbacksAndReleaseBlocks
