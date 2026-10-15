@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.Scheduler_AddOrUpdateCallback, 1
	.set import.Runtime_SetIrqHandler, 1
	.set import.SerialRuntime_PollAndTransfer, 1
	.set import.Map_GetTerrainHeight, 1
	.set import.Engine_EventBegin, 1
	.set import.Engine_EventEnd, 1
	.set import.Object_GetById, 1
	.set import.Engine_EventRequestExit, 1
	.set import.BattleFx_SetPhaseRequest, 1
	.set import.DisplayTransition_InitializeBattleEffectState, 1
	.set import.Engine_AudioPlayCue, 1
	.include "games/tbs/src/system/overlay.inc"
