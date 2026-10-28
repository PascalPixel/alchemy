.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.global Math_Cosine
	.thumb_func
Math_Cosine:
	overlay_veneer Math_CosineFar
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
	.global Func_02005164
	.thumb_func
Func_02005164:
	overlay_veneer Object_SetMode
	.global Func_0200516c
	.thumb_func
Func_0200516c:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02005174
	.thumb_func
Func_02005174:
	overlay_veneer Func_080200c0
	.global Func_0200517c
	.thumb_func
Func_0200517c:
	overlay_veneer Map_PlayMapEditFar
	.global Func_02005184
	.thumb_func
Func_02005184:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Func_0200518c
	.thumb_func
Func_0200518c:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02005194
	.thumb_func
Func_02005194:
	overlay_veneer Func_080201f0
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_020051ac
	.thumb_func
Func_020051ac:
	overlay_veneer Event_StartFar
	.global Func_020051b4
	.thumb_func
Func_020051b4:
	overlay_veneer Event_EndFar
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
	.global ObjectMotion_EnableActionAndResetMotion
	.thumb_func
ObjectMotion_EnableActionAndResetMotion:
	overlay_veneer ObjectMotion_EnableActionAndResetMotionFar
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
	.global Func_0200520c
	.thumb_func
Func_0200520c:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_02005214
	.thumb_func
Func_02005214:
	overlay_veneer ObjectMotion_SetHorizontalPositionAndAngleWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
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
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_0200524c
	.thumb_func
Func_0200524c:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_0200525c
	.thumb_func
Func_0200525c:
	overlay_veneer Func_080c8198
	.global Func_02005264
	.thumb_func
Func_02005264:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_02005274
	.thumb_func
Func_02005274:
	overlay_veneer ObjectMotion_ArmCallbackAndRefreshFar
	.global Func_0200527c
	.thumb_func
Func_0200527c:
	overlay_veneer Battle_ShowPairedUnitWorkAndWaitFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_0200528c
	.thumb_func
Func_0200528c:
	overlay_veneer Func_080c8210
	.global Func_02005294
	.thumb_func
Func_02005294:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Func_020052a4
	.thumb_func
Func_020052a4:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_020052b4
	.thumb_func
Func_020052b4:
	overlay_veneer Func_080c8240
	.global Func_020052bc
	.thumb_func
Func_020052bc:
	overlay_veneer Func_080c8248
	.global Func_020052c4
	.thumb_func
Func_020052c4:
	overlay_veneer Event_SetExitRequestFar
	.global Func_020052cc
	.thumb_func
Func_020052cc:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_020052d4
	.thumb_func
Func_020052d4:
	overlay_veneer BattleEffect_StartBufferInterpolationFar
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
	.global Func_020052f4
	.thumb_func
Func_020052f4:
	overlay_veneer Func_080c8480
	.global Field_DispatchTypeHandler
	.thumb_func
Field_DispatchTypeHandler:
	overlay_veneer Field_DispatchTypeHandlerFar
	.global Func_02005304
	.thumb_func
Func_02005304:
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.global Func_0200530c
	.thumb_func
Func_0200530c:
	overlay_veneer BattleEffect_SetupObjectPairFar
	.global Func_02005314
	.thumb_func
Func_02005314:
	overlay_veneer EffectRuntime_StopCurrentObjectFar
	.global Func_0200531c
	.thumb_func
Func_0200531c:
	overlay_veneer BattleEffect_CleanupSceneObjectsFar
	.global Func_02005324
	.thumb_func
Func_02005324:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_0200532c
	.thumb_func
Func_0200532c:
	overlay_veneer Func_080c8580
	.global Func_02005334
	.thumb_func
Func_02005334:
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
	.global Func_02005354
	.thumb_func
Func_02005354:
	overlay_veneer Func_080c86c0
	.global Func_0200535c
	.thumb_func
Func_0200535c:
	overlay_veneer Func_080c86d0
	.global Func_02005364
	.thumb_func
Func_02005364:
	overlay_veneer Func_080c86d8
	.global Func_0200536c
	.thumb_func
Func_0200536c:
	overlay_veneer Func_080c8718
	.global Func_02005374
	.thumb_func
Func_02005374:
	overlay_veneer Func_080c8720
	.global Func_0200537c
	.thumb_func
Func_0200537c:
	overlay_veneer Func_080c8728
	.global Func_02005384
	.thumb_func
Func_02005384:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_0200538c
	.thumb_func
Func_0200538c:
	overlay_veneer Func_080c8968
	.global Func_02005394
	.thumb_func
Func_02005394:
	overlay_veneer Sound_PlayFar
