.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global __udivsi3
	.thumb_func
__udivsi3:
	overlay_veneer IwramUnsignedDivide
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global Scheduler_RemoveCallbackFar
	.thumb_func
Scheduler_RemoveCallbackFar:
	overlay_veneer Func_080000d8
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
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global Runtime_AllocateHeapBlockFar
	.thumb_func
Runtime_AllocateHeapBlockFar:
	overlay_veneer Func_08000140
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	overlay_veneer Runtime_ReleaseHeapBlockFar
	.global Resource_ResetEntry
	.thumb_func
Resource_ResetEntry:
	overlay_veneer Resource_ResetEntryFar
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Unit_Get
	.thumb_func
Unit_Get:
	overlay_veneer Unit_GetFar
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
	.global Func_02005a24
	.thumb_func
Func_02005a24:
	overlay_veneer Object_SetMode
	.global Func_02005a2c
	.thumb_func
Func_02005a2c:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02005a34
	.thumb_func
Func_02005a34:
	overlay_veneer Func_080200c0
	.global Func_02005a3c
	.thumb_func
Func_02005a3c:
	overlay_veneer Object_Destroy
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02005a4c
	.thumb_func
Func_02005a4c:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02005a54
	.thumb_func
Func_02005a54:
	overlay_veneer Object_SetPosition
	.global Func_02005a5c
	.thumb_func
Func_02005a5c:
	overlay_veneer Object_CommitPosition
	.global Func_02005a64
	.thumb_func
Func_02005a64:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Func_02005a6c
	.thumb_func
Func_02005a6c:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02005a84
	.thumb_func
Func_02005a84:
	overlay_veneer Func_08038248
	.global Func_02005a8c
	.thumb_func
Func_02005a8c:
	overlay_veneer Func_080ad228
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02005a9c
	.thumb_func
Func_02005a9c:
	overlay_veneer EventRuntime_StartFar
	.global Func_02005aa4
	.thumb_func
Func_02005aa4:
	overlay_veneer EventRuntime_EndFar
	.global Event_PrepareObjectAndApplyValue
	.thumb_func
Event_PrepareObjectAndApplyValue:
	overlay_veneer Event_PrepareObjectAndApplyValueFar
	.global Party_RemoveOwnerRestored
	.thumb_func
Party_RemoveOwnerRestored:
	overlay_veneer Party_RemoveOwnerRestoredFar
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
	.global Func_02005b0c
	.thumb_func
Func_02005b0c:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_02005b14
	.thumb_func
Func_02005b14:
	overlay_veneer ObjectMotion_SetHorizontalPositionAndAngleWithTerrainFar
	.global Func_02005b1c
	.thumb_func
Func_02005b1c:
	overlay_veneer ObjectMotion_MoveToObjectFar
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
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02005b4c
	.thumb_func
Func_02005b4c:
	overlay_veneer EventRuntime_SetMessageFar
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_02005b5c
	.thumb_func
Func_02005b5c:
	overlay_veneer Func_080c8198
	.global Func_02005b64
	.thumb_func
Func_02005b64:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global Func_02005b6c
	.thumb_func
Func_02005b6c:
	overlay_veneer Func_080c81a8
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_02005b7c
	.thumb_func
Func_02005b7c:
	overlay_veneer ObjectMotion_ArmCallbackAndRefreshFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02005b8c
	.thumb_func
Func_02005b8c:
	overlay_veneer Func_080c8208
	.global Func_02005b94
	.thumb_func
Func_02005b94:
	overlay_veneer Func_080c8210
	.global Func_02005b9c
	.thumb_func
Func_02005b9c:
	overlay_veneer Func_080c8218
	.global Func_02005ba4
	.thumb_func
Func_02005ba4:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02005bb4
	.thumb_func
Func_02005bb4:
	overlay_veneer Func_080c8240
	.global Func_02005bbc
	.thumb_func
Func_02005bbc:
	overlay_veneer Func_080c8258
	.global Func_02005bc4
	.thumb_func
Func_02005bc4:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Func_02005bcc
	.thumb_func
Func_02005bcc:
	overlay_veneer Func_080c82e0
	.global Func_02005bd4
	.thumb_func
Func_02005bd4:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_02005bdc
	.thumb_func
Func_02005bdc:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global Func_02005be4
	.thumb_func
Func_02005be4:
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
	.global Func_02005c04
	.thumb_func
Func_02005c04:
	overlay_veneer Func_080c8480
	.global Field_DispatchTypeHandler
	.thumb_func
Field_DispatchTypeHandler:
	overlay_veneer Field_DispatchTypeHandlerFar
	.global Func_02005c14
	.thumb_func
Func_02005c14:
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.global Func_02005c1c
	.thumb_func
Func_02005c1c:
	overlay_veneer BattleEffect_SetupObjectPairFar
	.global Func_02005c24
	.thumb_func
Func_02005c24:
	overlay_veneer EffectRuntime_StopCurrentObjectFar
	.global Func_02005c2c
	.thumb_func
Func_02005c2c:
	overlay_veneer BattleEffect_CleanupSceneObjectsFar
	.global Func_02005c34
	.thumb_func
Func_02005c34:
	overlay_veneer EventObject_InitializeFar
	.global Func_02005c3c
	.thumb_func
Func_02005c3c:
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global Func_02005c44
	.thumb_func
Func_02005c44:
	overlay_veneer Func_080c8518
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_02005c54
	.thumb_func
Func_02005c54:
	overlay_veneer Func_080c86a8
	.global Func_02005c5c
	.thumb_func
Func_02005c5c:
	overlay_veneer Func_080c86e8
	.global Func_02005c64
	.thumb_func
Func_02005c64:
	overlay_veneer Func_080c86f8
	.global Func_02005c6c
	.thumb_func
Func_02005c6c:
	overlay_veneer Func_080c8708
	.global Func_02005c74
	.thumb_func
Func_02005c74:
	overlay_veneer Func_080c87b0
	.global Func_02005c7c
	.thumb_func
Func_02005c7c:
	overlay_veneer Func_080c8930
	.global Func_02005c84
	.thumb_func
Func_02005c84:
	overlay_veneer Func_08108008
	.global Func_02005c8c
	.thumb_func
Func_02005c8c:
	overlay_veneer Func_08108010
	.global Func_02005c94
	.thumb_func
Func_02005c94:
	overlay_veneer Func_08108018
	.global Func_02005c9c
	.thumb_func
Func_02005c9c:
	overlay_veneer Sound_PlayFar
