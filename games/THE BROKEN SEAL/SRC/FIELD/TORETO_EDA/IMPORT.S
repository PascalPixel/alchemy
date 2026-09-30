@ Import veneers after the TORETO_EDA overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — TORETO_EDA overlay import veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
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
	.global Battle_Reset
	.thumb_func
Battle_Reset:
	overlay_veneer Battle_ResetFar
	.global BattleFx_FinishAction
	.thumb_func
BattleFx_FinishAction:
	overlay_veneer BattleFx_FinishActionFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Event_SetValue170
	.thumb_func
Event_SetValue170:
	overlay_veneer Event_SetValue170Far
	.global BattleFx_SetPhaseRequest
	.thumb_func
BattleFx_SetPhaseRequest:
	overlay_veneer BattleFx_SetPhaseRequestFar
	.global DisplayTransition_InitializeBattleEffectState
	.thumb_func
DisplayTransition_InitializeBattleEffectState:
	overlay_veneer DisplayTransition_InitializeBattleEffectStateFar
	.global Audio_PlayCue
	.thumb_func
Audio_PlayCue:
	overlay_veneer AudioCommand_PlayFar
