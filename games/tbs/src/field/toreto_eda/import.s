@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	import Engine_TaskAddCallback, Scheduler_AddOrUpdateCallbackFar
	import Runtime_SetIrqHandler, Runtime_SetIrqHandlerFar
	import SerialRuntime_PollAndTransfer, SerialRuntime_PollAndTransferFar
	import Map_GetTerrainHeight, Map_GetTerrainHeightFar
	import Engine_EventBegin, Battle_ResetFar
	import Engine_EventEnd, BattleFx_FinishActionFar
	import Object_GetById, Object_GetByIdFar
	import Engine_EventRequestExit, Event_SetExitRequestFar
	import BattleFx_SetPhaseRequest, BattleFx_SetPhaseRequestFar
	import DisplayTransition_InitializeBattleEffectState, DisplayTransition_InitializeBattleEffectStateFar
	import Engine_AudioPlayCue, AudioCommand_PlayFar
