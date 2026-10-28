.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global Import_GameFlagIsSet
	.thumb_func
Import_GameFlagIsSet:
	overlay_veneer GameFlag_TestFar
	.global Import_GameFlagSet
	.thumb_func
Import_GameFlagSet:
	overlay_veneer GameFlag_SetBitFar
	.global Func_02000c48
	.thumb_func
Func_02000c48:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Import_EventWait
	.thumb_func
Import_EventWait:
	overlay_veneer FieldEvent_WaitFar
	.global Import_EventStart
	.thumb_func
Import_EventStart:
	overlay_veneer Event_StartFar
	.global Import_EventEnd
	.thumb_func
Import_EventEnd:
	overlay_veneer Event_EndFar
	.global Import_EventAskYesNoWithActorAnimation
	.thumb_func
Import_EventAskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global Import_ActorGet
	.thumb_func
Import_ActorGet:
	overlay_veneer Actor_GetByIdFar
	.global Import_ActorSetSpeed
	.thumb_func
Import_ActorSetSpeed:
	overlay_veneer Actor_SetSpeedParametersFar
	.global Actor_ResetAndSetPosition
	.thumb_func
Actor_ResetAndSetPosition:
	overlay_veneer Actor_ResetAndSetPositionFar
	.global Actor_SetPositionAndCommit
	.thumb_func
Actor_SetPositionAndCommit:
	overlay_veneer Actor_SetPositionAndCommitFar
	.global Actor_SetPositionAndReset
	.thumb_func
Actor_SetPositionAndReset:
	overlay_veneer Actor_SetPositionAndResetFar
	.global Import_ActorCenterAndWalk
	.thumb_func
Import_ActorCenterAndWalk:
	overlay_veneer Actor_CenterOnColumnFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_02000ca8
	.thumb_func
Func_02000ca8:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Import_ActorSetAnimation
	.thumb_func
Import_ActorSetAnimation:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_Jump
	.thumb_func
Actor_Jump:
	overlay_veneer Actor_JumpFar
	.global Actor_StartRepeatedMotion
	.thumb_func
Actor_StartRepeatedMotion:
	overlay_veneer Actor_StartRepeatedMotionFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Actor_SetAngleToward
	.thumb_func
Actor_SetAngleToward:
	overlay_veneer Actor_SetAngleTowardFar
	.global Actor_TurnToFaceEachOther
	.thumb_func
Actor_TurnToFaceEachOther:
	overlay_veneer Actor_TurnToFaceEachOtherFar
	.global Import_EventSetMessageId
	.thumb_func
Import_EventSetMessageId:
	overlay_veneer Event_SetMessageIdFar
	.global Import_FieldMessageShowAtActor
	.thumb_func
Import_FieldMessageShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Import_EventShowMessageAndWait
	.thumb_func
Import_EventShowMessageAndWait:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Import_ActorSetSpritePriority
	.thumb_func
Import_ActorSetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_02000d08
	.thumb_func
Func_02000d08:
	overlay_veneer Func_080c8210
	.global Import_EventSetExitRequest
	.thumb_func
Import_EventSetExitRequest:
	overlay_veneer Event_SetExitRequestFar
	.global Func_02000d18
	.thumb_func
Func_02000d18:
	overlay_veneer Func_080c82e0
	.global Import_EventFinishScreenTransition
	.thumb_func
Import_EventFinishScreenTransition:
	overlay_veneer Event_FinishScreenTransitionFar
	.global Import_EventWaitTransitionFrames
	.thumb_func
Import_EventWaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Actor_SetWideSprite
	.thumb_func
Actor_SetWideSprite:
	overlay_veneer Actor_SetWideSpriteFar
	.global Import_ActorReturnHome
	.thumb_func
Import_ActorReturnHome:
	overlay_veneer Actor_ReturnHomeFar
	.global Import_EventResolvePendingActions
	.thumb_func
Import_EventResolvePendingActions:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02000d48
	.thumb_func
Func_02000d48:
	overlay_veneer Motion_LaunchFromFocusedObjectFar
	.global Func_02000d50
	.thumb_func
Func_02000d50:
	overlay_veneer Func_080c86f8
	.global Func_02000d58
	.thumb_func
Func_02000d58:
	overlay_veneer Func_080c8708
	.global Import_PartyGetLeaderActor
	.thumb_func
Import_PartyGetLeaderActor:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_02000d68
	.thumb_func
Func_02000d68:
	overlay_veneer Func_080c87b0
	.global Import_ShopOpen
	.thumb_func
Import_ShopOpen:
	overlay_veneer Func_08108008
	.global Import_SoundPlay
	.thumb_func
Import_SoundPlay:
	overlay_veneer Sound_PlayFar
