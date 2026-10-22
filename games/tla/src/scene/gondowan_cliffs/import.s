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
	.global __umodsi3
	.thumb_func
__umodsi3:
	overlay_veneer IwramUnsignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
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
	.global Runtime_BumpAllocateAlternatePool
	.thumb_func
Runtime_BumpAllocateAlternatePool:
	overlay_veneer Runtime_BumpAllocateAlternatePoolFar
	.global MemFree
	.thumb_func
MemFree:
	overlay_veneer Sys_FreeFar
	.global Func_02001184
	.thumb_func
Func_02001184:
	overlay_veneer Resource_DecodeType01Far
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_0200119c
	.thumb_func
Func_0200119c:
	overlay_veneer Runtime_PushSlotEntryFar
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global Func_020011b4
	.thumb_func
Func_020011b4:
	overlay_veneer Object_SetMode
	.global Func_020011bc
	.thumb_func
Func_020011bc:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Engine_ObjectCreate
	.thumb_func
Engine_ObjectCreate:
	overlay_veneer Func_080200c0
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_08020190
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_08020190
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_08020190
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_08020190
	.else
	overlay_veneer Map_GetTerrainHeightFar
	.endif
	.endif
	.endif
	.endif
	.global Func_020011d4
	.thumb_func
Func_020011d4:
	.ifdef TLA_EDITION_DE
	overlay_veneer DisplayBlend_StartScriptFar
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer DisplayBlend_StartScriptFar
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer DisplayBlend_StartScriptFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer DisplayBlend_StartScriptFar
	.else
	overlay_veneer Map_CopyCellAttributeRectFar
	.endif
	.endif
	.endif
	.endif
	.global Func_020011dc
	.thumb_func
Func_020011dc:
	.ifdef TLA_EDITION_DE
	overlay_veneer Map_GetTerrainHeightFar
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Map_GetTerrainHeightFar
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Map_GetTerrainHeightFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Map_GetTerrainHeightFar
	.else
	overlay_veneer Func_080201f0
	.endif
	.endif
	.endif
	.endif
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_020011f4
	.thumb_func
Func_020011f4:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_020011fc
	.thumb_func
Func_020011fc:
	overlay_veneer Func_08020230
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_0200120c
	.thumb_func
Func_0200120c:
	overlay_veneer EventRuntime_StartFar
	.global Func_02001214
	.thumb_func
Func_02001214:
	overlay_veneer EventRuntime_EndFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
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
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_0200124c
	.thumb_func
Func_0200124c:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_02001254
	.thumb_func
Func_02001254:
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
	.global Object_LinkPair
	.thumb_func
Object_LinkPair:
	overlay_veneer Object_LinkPairFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_0200128c
	.thumb_func
Func_0200128c:
	overlay_veneer EventRuntime_SetMessageFar
	.global Func_02001294
	.thumb_func
Func_02001294:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_020012a4
	.thumb_func
Func_020012a4:
	overlay_veneer Func_080c8210
	.global Func_020012ac
	.thumb_func
Func_020012ac:
	overlay_veneer Func_080c8218
	.global Func_020012b4
	.thumb_func
Func_020012b4:
	overlay_veneer Event_SetDestinationFar
	.global Func_020012bc
	.thumb_func
Func_020012bc:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Func_020012c4
	.thumb_func
Func_020012c4:
	overlay_veneer Func_080c8288
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattlePalette_StopBlendAndReleaseBufferFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattlePalette_StopBlendAndReleaseBufferFar
	.else
	overlay_veneer Event_StartScreenTransitionFar
	.endif
	.endif
	.global Event_FinishScreenTransition
	.thumb_func
Event_FinishScreenTransition:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_SetPrimaryBufferValueFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_SetPrimaryBufferValueFar
	.else
	overlay_veneer Event_FinishScreenTransitionFar
	.endif
	.endif
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	.ifdef TLA_EDITION_ES
	overlay_veneer Event_DelayEffectFramesFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Event_DelayEffectFramesFar
	.else
	overlay_veneer Event_WaitTransitionFramesFar
	.endif
	.endif
	.global Func_020012e4
	.thumb_func
Func_020012e4:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.endif
	.endif
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_020012f4
	.thumb_func
Func_020012f4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8698
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8698
	.else
	overlay_veneer Func_080c86a8
	.endif
	.endif
	.global Func_020012fc
	.thumb_func
Func_020012fc:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86c0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86c0
	.else
	overlay_veneer Func_080c86d0
	.endif
	.endif
	.global Func_02001304
	.thumb_func
Func_02001304:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86c8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86c8
	.else
	overlay_veneer Func_080c86d8
	.endif
	.endif
	.global Func_0200130c
	.thumb_func
Func_0200130c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86d8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86d8
	.else
	overlay_veneer Func_080c86e8
	.endif
	.endif
	.global Func_02001314
	.thumb_func
Func_02001314:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8768
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8768
	.else
	overlay_veneer EventRuntime_GetControlledOwnerFar
	.endif
	.endif
	.global Func_0200131c
	.thumb_func
Func_0200131c:
	overlay_veneer Audio_PlayCue
