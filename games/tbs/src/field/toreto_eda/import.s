@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	.global Engine_TaskAddCallback
	.thumb_func
Engine_TaskAddCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global Runtime_SetIrqHandler
	.thumb_func
Runtime_SetIrqHandler:
	overlay_veneer Runtime_SetIrqHandlerFar
	.global SerialRuntime_PollAndTransfer
	.thumb_func
SerialRuntime_PollAndTransfer:
	overlay_veneer SerialRuntime_PollAndTransferFar
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Engine_EventBegin
	.thumb_func
Engine_EventBegin:
	overlay_veneer Battle_ResetFar
	.global Engine_EventEnd
	.thumb_func
Engine_EventEnd:
	overlay_veneer BattleFx_FinishActionFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Engine_EventRequestExit
	.thumb_func
Engine_EventRequestExit:
	overlay_veneer Event_SetExitRequestFar
	.global BattleFx_SetPhaseRequest
	.thumb_func
BattleFx_SetPhaseRequest:
	overlay_veneer BattleFx_SetPhaseRequestFar
	.global DisplayTransition_InitializeBattleEffectState
	.thumb_func
DisplayTransition_InitializeBattleEffectState:
	overlay_veneer DisplayTransition_InitializeBattleEffectStateFar
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer AudioCommand_PlayFar
