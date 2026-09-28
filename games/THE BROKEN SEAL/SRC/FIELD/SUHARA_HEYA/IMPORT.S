@ Import veneers after the SUHARA_HEYA overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — SUHARA_HEYA overlay import veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_GameFlagIsSet
	.thumb_func
Engine_GameFlagIsSet:
	overlay_veneer GameFlag_TestFar
	.global Engine_GameFlagSet
	.thumb_func
Engine_GameFlagSet:
	overlay_veneer GameFlag_SetBitFar
	overlay_veneer Battle_WaitMode0Far
	overlay_veneer Battle_ResetFar
	overlay_veneer BattleFx_FinishActionFar
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	.global Engine_ActorGet
	.thumb_func
Engine_ActorGet:
	overlay_veneer Object_GetByIdFar
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	overlay_veneer Object_SetModeByIdFar
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	overlay_veneer Motion_SetVarCbAndRefreshFar
	overlay_veneer ObjectMotion_SetAngleTowardFar
	overlay_veneer Object_LinkPairFar
	.global Engine_EventSetMessage
	.thumb_func
Engine_EventSetMessage:
	overlay_veneer Event_SetValue1d8Far
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Engine_EventShowMessage
	.thumb_func
Engine_EventShowMessage:
	overlay_veneer BattleEv_RunWaitFar
	overlay_veneer ObjectMotion_ArmCallbackFar
	overlay_veneer BattleFx_SpawnLinkedFar
	overlay_veneer Motion_CamBoundsFar
	overlay_veneer BattleFx_CommitObjectPositionAndWaitFar
	.global Engine_EventRequestExit
	.thumb_func
Engine_EventRequestExit:
	overlay_veneer Event_SetValue170Far
	overlay_veneer Audio_PlayCueFromEventWorkFar
	overlay_veneer Motion_LaunchFromFocusedObjectFar
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	overlay_veneer Shop_RunFar
	.global Shop_ConfirmAct
	.thumb_func
Shop_ConfirmAct:
	overlay_veneer Shop_ConfirmActFar
	.global Inn_CheckIn
	.thumb_func
Inn_CheckIn:
	overlay_veneer Inn_CheckInFar
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer AudioCommand_PlayFar
