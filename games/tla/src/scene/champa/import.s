.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	overlay_veneer GameFlag_ClearBitFar
	.global Func_020029dc
	.thumb_func
Func_020029dc:
	overlay_veneer Object_SetMode
	.global Func_020029e4
	.thumb_func
Func_020029e4:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_020029ec
	.thumb_func
Func_020029ec:
	overlay_veneer Func_080200c0
	.global Func_020029f4
	.thumb_func
Func_020029f4:
	overlay_veneer Func_08020198
	.global Func_020029fc
	.thumb_func
Func_020029fc:
	overlay_veneer Func_080201a0
	.global Func_02002a04
	.thumb_func
Func_02002a04:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02002a0c
	.thumb_func
Func_02002a0c:
	overlay_veneer Func_080201f0
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global UiText_OpenMessageWindow
	.thumb_func
UiText_OpenMessageWindow:
	overlay_veneer UiText_OpenMessageWindowFar
	.global UiWork_IsComplete
	.thumb_func
UiWork_IsComplete:
	overlay_veneer UiWork_IsCompleteFar
	.global Func_02002a2c
	.thumb_func
Func_02002a2c:
	overlay_veneer Func_080380f8
	.global UiWork_FinalizeEntityMatchingLocalizedId
	.thumb_func
UiWork_FinalizeEntityMatchingLocalizedId:
	overlay_veneer UiWork_FinalizeEntityMatchingLocalizedIdFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02002a44
	.thumb_func
Func_02002a44:
	overlay_veneer EventRuntime_StartFar
	.global Func_02002a4c
	.thumb_func
Func_02002a4c:
	overlay_veneer EventRuntime_EndFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global Func_02002a5c
	.thumb_func
Func_02002a5c:
	overlay_veneer Func_080c8078
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
	.global Func_02002a9c
	.thumb_func
Func_02002a9c:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_02002aa4
	.thumb_func
Func_02002aa4:
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
	.global ObjectMotion_SetVariantCallback
	.thumb_func
ObjectMotion_SetVariantCallback:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Object_TurnToFaceEachOther
	.thumb_func
Object_TurnToFaceEachOther:
	overlay_veneer Object_LinkPairFar
	.global Func_02002adc
	.thumb_func
Func_02002adc:
	overlay_veneer EventRuntime_SetMessageFar
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_02002aec
	.thumb_func
Func_02002aec:
	overlay_veneer Func_080c8198
	.global Func_02002af4
	.thumb_func
Func_02002af4:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_02002b04
	.thumb_func
Func_02002b04:
	overlay_veneer ObjectMotion_ArmCallbackAndRefreshFar
	.global Func_02002b0c
	.thumb_func
Func_02002b0c:
	overlay_veneer Battle_ShowPairedUnitWorkAndWaitFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02002b1c
	.thumb_func
Func_02002b1c:
	overlay_veneer Func_080c8210
	.global Func_02002b24
	.thumb_func
Func_02002b24:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02002b3c
	.thumb_func
Func_02002b3c:
	overlay_veneer Func_080c8240
	.global Func_02002b44
	.thumb_func
Func_02002b44:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Event_FinishScreenTransition
	.thumb_func
Event_FinishScreenTransition:
	overlay_veneer Event_FinishScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Func_02002b5c
	.thumb_func
Func_02002b5c:
	overlay_veneer ObjectEvent_PlaceOnGroundFar
	.global Func_02002b64
	.thumb_func
Func_02002b64:
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global BattleEffect_GetResourceId
	.thumb_func
BattleEffect_GetResourceId:
	overlay_veneer BattleEffect_GetResourceIdFar
	.global Func_02002b74
	.thumb_func
Func_02002b74:
	overlay_veneer Motion_LaunchFromFocusedObjectFar
	.global ObjectMotion_OffsetPositionAndReset
	.thumb_func
ObjectMotion_OffsetPositionAndReset:
	overlay_veneer ObjectMotion_OffsetPositionAndResetFar
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global ObjectTable_ReadActiveValue
	.thumb_func
ObjectTable_ReadActiveValue:
	overlay_veneer ObjectTable_ReadActiveValueFar
	.global Object_SetActionCallback
	.thumb_func
Object_SetActionCallback:
	overlay_veneer Object_SetActionCallbackFar
	.global Func_02002ba4
	.thumb_func
Func_02002ba4:
	overlay_veneer Func_080c86d0
	.global Func_02002bac
	.thumb_func
Func_02002bac:
	overlay_veneer Func_080c86d8
	.global Func_02002bb4
	.thumb_func
Func_02002bb4:
	overlay_veneer EventRuntime_GetControlledOwnerFar
	.global Func_02002bbc
	.thumb_func
Func_02002bbc:
	overlay_veneer Func_080c8860
	.global Func_02002bc4
	.thumb_func
Func_02002bc4:
	overlay_veneer Sound_PlayFar
