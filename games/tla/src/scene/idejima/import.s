.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global Task_Wait
	.thumb_func
Task_Wait:
	overlay_veneer waitFar
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
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
	.global Import_ActorSetMode
	.thumb_func
Import_ActorSetMode:
	overlay_veneer Actor_SetMode
	.global Import_ActorSetScript
	.thumb_func
Import_ActorSetScript:
	overlay_veneer Actor_StartScriptFar
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Func_080200c0
	.global Func_02003180
	.thumb_func
Func_02003180:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Import_MapCopyCellPictureIndicesRect
	.thumb_func
Import_MapCopyCellPictureIndicesRect:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Func_02003190
	.thumb_func
Func_02003190:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global Func_020031a8
	.thumb_func
Func_020031a8:
	overlay_veneer Func_080ad208
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Import_EventStart
	.thumb_func
Import_EventStart:
	overlay_veneer Event_StartFar
	.global Import_EventEnd
	.thumb_func
Import_EventEnd:
	overlay_veneer Event_EndFar
	.global Event_PrepareObjectAndApplyValue
	.thumb_func
Event_PrepareObjectAndApplyValue:
	overlay_veneer Event_PrepareObjectAndApplyValueFar
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
	.global Actor_SetMovementTypeById
	.thumb_func
Actor_SetMovementTypeById:
	overlay_veneer Actor_SetMovementTypeByIdFar
	.global Actor_RefreshSelectorById
	.thumb_func
Actor_RefreshSelectorById:
	overlay_veneer Actor_RefreshSelectorByIdFar
	.global Actor_Stop
	.thumb_func
Actor_Stop:
	overlay_veneer Actor_StopFar
	.global Actor_SetMovementTypeAndWaitById
	.thumb_func
Actor_SetMovementTypeAndWaitById:
	overlay_veneer Actor_SetMovementTypeAndWaitByIdFar
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
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_02003228
	.thumb_func
Func_02003228:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Func_02003230
	.thumb_func
Func_02003230:
	overlay_veneer Actor_SetHorizontalPositionAndAngleWithTerrainFar
	.global Import_ActorPlaceAtActor
	.thumb_func
Import_ActorPlaceAtActor:
	overlay_veneer Actor_MoveToObjectFar
	.global Import_ActorSetAnimation
	.thumb_func
Import_ActorSetAnimation:
	overlay_veneer Actor_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global Actor_WaitForAnimationChange
	.thumb_func
Actor_WaitForAnimationChange:
	overlay_veneer Actor_WaitForAnimationChangeFar
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
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Import_EventSetMessageId
	.thumb_func
Import_EventSetMessageId:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_02003298
	.thumb_func
Func_02003298:
	overlay_veneer Func_080c8198
	.global Import_EventShowMessageAndWait
	.thumb_func
Import_EventShowMessageAndWait:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Import_EventAskYesNoWithActorAnimation
	.thumb_func
Import_EventAskYesNoWithActorAnimation:
	overlay_veneer Func_080c81a8
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Func_020032b8
	.thumb_func
Func_020032b8:
	overlay_veneer Battle_ShowPairedUnitWorkAndWaitFar
	.global Import_ActorShowEmote
	.thumb_func
Import_ActorShowEmote:
	overlay_veneer Func_080c8210
	.global Import_ActorSetAttachedEffect
	.thumb_func
Import_ActorSetAttachedEffect:
	overlay_veneer Func_080c8218
	.global Import_CameraSetSpeed
	.thumb_func
Import_CameraSetSpeed:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_020032e0
	.thumb_func
Func_020032e0:
	overlay_veneer Func_080c8240
	.global Func_020032e8
	.thumb_func
Func_020032e8:
	overlay_veneer Event_SetExitRequestFar
	.global Func_020032f0
	.thumb_func
Func_020032f0:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
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
	.global Func_02003310
	.thumb_func
Func_02003310:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Actor_WatchAndSetScript
	.thumb_func
Actor_WatchAndSetScript:
	overlay_veneer Actor_WatchAndSetScriptFar
	.global Func_02003320
	.thumb_func
Func_02003320:
	overlay_veneer Func_080c8930
	.global Import_SoundPlay
	.thumb_func
Import_SoundPlay:
	overlay_veneer Sound_PlayFar
