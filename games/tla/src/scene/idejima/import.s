.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global Task_Wait
	.thumb_func
Task_Wait:
	overlay_veneer WaitFramesFar
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
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
	.global Engine_ObjectSetMode
	.thumb_func
Engine_ObjectSetMode:
	overlay_veneer Object_SetMode
	.global Engine_ObjectSetScript
	.thumb_func
Engine_ObjectSetScript:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Engine_ObjectCreate
	.thumb_func
Engine_ObjectCreate:
	overlay_veneer Func_080200c0
	.global Func_02003180
	.thumb_func
Func_02003180:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Engine_MapCopyCellsTo
	.thumb_func
Engine_MapCopyCellsTo:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Func_02003190
	.thumb_func
Func_02003190:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Func_020031a8
	.thumb_func
Func_020031a8:
	overlay_veneer Func_080ad208
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Engine_EventStart
	.thumb_func
Engine_EventStart:
	overlay_veneer EventRuntime_StartFar
	.global Engine_EventEnd
	.thumb_func
Engine_EventEnd:
	overlay_veneer EventRuntime_EndFar
	.global Event_PrepareObjectAndApplyValue
	.thumb_func
Event_PrepareObjectAndApplyValue:
	overlay_veneer Event_PrepareObjectAndApplyValueFar
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
	.global ObjectMotion_EnableActionAndSetCallback
	.thumb_func
ObjectMotion_EnableActionAndSetCallback:
	overlay_veneer ObjectMotion_EnableActionAndSetCallbackFar
	.global Object_RefreshSelectorById
	.thumb_func
Object_RefreshSelectorById:
	overlay_veneer Object_RefreshSelectorByIdFar
	.global ObjectMotion_EnableActionAndResetMotion
	.thumb_func
ObjectMotion_EnableActionAndResetMotion:
	overlay_veneer ObjectMotion_EnableActionAndResetMotionFar
	.global Object_SetActionCallbackAndRefreshById
	.thumb_func
Object_SetActionCallbackAndRefreshById:
	overlay_veneer Object_SetActionCallbackAndRefreshByIdFar
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
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02003228
	.thumb_func
Func_02003228:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_02003230
	.thumb_func
Func_02003230:
	overlay_veneer ObjectMotion_SetHorizontalPositionAndAngleWithTerrainFar
	.global Engine_ActorPlaceAtActor
	.thumb_func
Engine_ActorPlaceAtActor:
	overlay_veneer ObjectMotion_MoveToObjectFar
	.global Engine_ActorSetAnimation
	.thumb_func
Engine_ActorSetAnimation:
	overlay_veneer Object_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global ObjectMotion_WaitForAnimationChange
	.thumb_func
ObjectMotion_WaitForAnimationChange:
	overlay_veneer ObjectMotion_WaitForAnimationChangeFar
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
	.global Engine_EventSetMessage
	.thumb_func
Engine_EventSetMessage:
	overlay_veneer EventRuntime_SetMessageFar
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_02003298
	.thumb_func
Func_02003298:
	overlay_veneer Func_080c8198
	.global Engine_EventShowMessage
	.thumb_func
Engine_EventShowMessage:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global Engine_EventChooseYesNo
	.thumb_func
Engine_EventChooseYesNo:
	overlay_veneer Func_080c81a8
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_020032b8
	.thumb_func
Func_020032b8:
	overlay_veneer Battle_ShowPairedUnitWorkAndWaitFar
	.global Engine_ActorShowEmote
	.thumb_func
Engine_ActorShowEmote:
	overlay_veneer Func_080c8210
	.global Engine_ActorSetAttachedEffect
	.thumb_func
Engine_ActorSetAttachedEffect:
	overlay_veneer Func_080c8218
	.global Engine_CameraSetSpeed
	.thumb_func
Engine_CameraSetSpeed:
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
	overlay_veneer EventRuntime_SetCountdownFar
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
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_02003320
	.thumb_func
Func_02003320:
	overlay_veneer Func_080c8930
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer Audio_PlayCue
