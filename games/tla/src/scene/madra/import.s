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
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Func_02002b10
	.thumb_func
Func_02002b10:
	overlay_veneer UiText_DrawQuantity
	.global Func_02002b18
	.thumb_func
Func_02002b18:
	overlay_veneer PartyInventory_FindRoomFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02002b28
	.thumb_func
Func_02002b28:
	overlay_veneer EventRuntime_StartFar
	.global Func_02002b30
	.thumb_func
Func_02002b30:
	overlay_veneer EventRuntime_EndFar
	.global PartyInventory_GiveItem
	.thumb_func
PartyInventory_GiveItem:
	overlay_veneer PartyInventory_GiveItemFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global ObjectMotion_CenterOnColumn
	.thumb_func
ObjectMotion_CenterOnColumn:
	overlay_veneer ObjectMotion_CenterOnColumnFar
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02002b88
	.thumb_func
Func_02002b88:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_02002b90
	.thumb_func
Func_02002b90:
	overlay_veneer ObjectMotion_SetHorizontalPositionAndAngleWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global ObjectMotion_Launch
	.thumb_func
ObjectMotion_Launch:
	overlay_veneer ObjectMotion_LaunchFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Object_TurnToFaceEachOther
	.thumb_func
Object_TurnToFaceEachOther:
	overlay_veneer Object_LinkPairFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02002bd0
	.thumb_func
Func_02002bd0:
	overlay_veneer EventRuntime_SetMessageFar
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_02002be0
	.thumb_func
Func_02002be0:
	overlay_veneer Func_080c8198
	.global Func_02002be8
	.thumb_func
Func_02002be8:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_02002bf8
	.thumb_func
Func_02002bf8:
	overlay_veneer ObjectMotion_ArmCallbackAndRefreshFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
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
	overlay_veneer EventRuntime_SetCountdownFar
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
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global Func_02002c60
	.thumb_func
Func_02002c60:
	overlay_veneer Party_PlaySavedMusicFar
	.global Func_02002c68
	.thumb_func
Func_02002c68:
	overlay_veneer Motion_LaunchFromFocusedObjectFar
	.global ObjectMotion_OffsetPositionAndReset
	.thumb_func
ObjectMotion_OffsetPositionAndReset:
	overlay_veneer ObjectMotion_OffsetPositionAndResetFar
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Func_02002c80
	.thumb_func
Func_02002c80:
	overlay_veneer EventRuntime_GetControlledOwnerFar
	.global Func_02002c88
	.thumb_func
Func_02002c88:
	overlay_veneer Func_08108008
	.global Func_02002c90
	.thumb_func
Func_02002c90:
	overlay_veneer Audio_PlayCue
