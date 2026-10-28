.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global Func_02002ae8
	.thumb_func
Func_02002ae8:
	overlay_veneer Map_PlayMapEditFar
	.global Func_02002af0
	.thumb_func
Func_02002af0:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02002af8
	.thumb_func
Func_02002af8:
	overlay_veneer Func_080201f0
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global Func_02002b10
	.thumb_func
Func_02002b10:
	overlay_veneer MessageArgument_PushFar
	.global Func_02002b18
	.thumb_func
Func_02002b18:
	overlay_veneer PartyInventory_FindRoomFar
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_02002b28
	.thumb_func
Func_02002b28:
	overlay_veneer Event_StartFar
	.global Func_02002b30
	.thumb_func
Func_02002b30:
	overlay_veneer Event_EndFar
	.global PartyInventory_GiveItem
	.thumb_func
PartyInventory_GiveItem:
	overlay_veneer PartyInventory_GiveItemFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetSpeedParameters
	.thumb_func
Actor_SetSpeedParameters:
	overlay_veneer Actor_SetSpeedParametersFar
	.global Actor_ResetAndSetPosition
	.thumb_func
Actor_ResetAndSetPosition:
	overlay_veneer Actor_ResetAndSetPositionFar
	.global Actor_ResetAndSetPositionInMode2
	.thumb_func
Actor_ResetAndSetPositionInMode2:
	overlay_veneer Actor_ResetAndSetPositionInMode2Far
	.global Actor_SetPositionAndReset
	.thumb_func
Actor_SetPositionAndReset:
	overlay_veneer Actor_SetPositionAndResetFar
	.global Actor_CenterOnColumn
	.thumb_func
Actor_CenterOnColumn:
	overlay_veneer Actor_CenterOnColumnFar
	.global Actor_OffsetPositionAndResetMotion
	.thumb_func
Actor_OffsetPositionAndResetMotion:
	overlay_veneer Actor_OffsetPositionAndResetMotionFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_02002b88
	.thumb_func
Func_02002b88:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Func_02002b90
	.thumb_func
Func_02002b90:
	overlay_veneer Actor_SetHorizontalPositionAndAngleWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global Actor_Jump
	.thumb_func
Actor_Jump:
	overlay_veneer Actor_JumpFar
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
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_02002bd0
	.thumb_func
Func_02002bd0:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_02002be0
	.thumb_func
Func_02002be0:
	overlay_veneer Func_080c8198
	.global Func_02002be8
	.thumb_func
Func_02002be8:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Func_02002bf8
	.thumb_func
Func_02002bf8:
	overlay_veneer Actor_ArmCallbackAndRefreshFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_02002c08
	.thumb_func
Func_02002c08:
	overlay_veneer Func_080c8210
	.global Func_02002c10
	.thumb_func
Func_02002c10:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02002c28
	.thumb_func
Func_02002c28:
	overlay_veneer Func_080c8240
	.global Func_02002c30
	.thumb_func
Func_02002c30:
	overlay_veneer Event_SetExitRequestFar
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_FinishScreenTransition
	.thumb_func
Event_FinishScreenTransition:
	overlay_veneer Event_FinishScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Func_02002c50
	.thumb_func
Func_02002c50:
	overlay_veneer Func_080c83e0
	.global Func_02002c58
	.thumb_func
Func_02002c58:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02002c60
	.thumb_func
Func_02002c60:
	overlay_veneer Party_PlaySavedMusicFar
	.global Func_02002c68
	.thumb_func
Func_02002c68:
	overlay_veneer Motion_LaunchFromFocusedObjectFar
	.global Actor_OffsetPositionAndReset
	.thumb_func
Actor_OffsetPositionAndReset:
	overlay_veneer Actor_OffsetPositionAndResetFar
	.global Actor_CommitPositionAndActivate
	.thumb_func
Actor_CommitPositionAndActivate:
	overlay_veneer Actor_CommitPositionAndActivateFar
	.global Func_02002c80
	.thumb_func
Func_02002c80:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_02002c88
	.thumb_func
Func_02002c88:
	overlay_veneer Func_08108008
	.global Func_02002c90
	.thumb_func
Func_02002c90:
	overlay_veneer Sound_PlayFar
