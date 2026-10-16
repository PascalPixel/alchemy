.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global __modsi3
	.thumb_func
__modsi3:
	overlay_veneer IwramSignedRemainder
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
	.global MemAlloc
	.thumb_func
MemAlloc:
	overlay_veneer Runtime_BumpAllocateFar
	.global MemFree
	.thumb_func
MemFree:
	overlay_veneer Sys_FreeFar
	.global Func_02002174
	.thumb_func
Func_02002174:
	overlay_veneer Resource_DecodeType01Far
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_0200218c
	.thumb_func
Func_0200218c:
	overlay_veneer Runtime_PushSlotEntryFar
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global GameFlag_GetByte
	.thumb_func
GameFlag_GetByte:
	overlay_veneer GameFlag_GetByteFar
	.global GameFlag_SetByte
	.thumb_func
GameFlag_SetByte:
	overlay_veneer GameFlag_SetByteFar
	.global Func_020021b4
	.thumb_func
Func_020021b4:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_ApplyValueToChildren
	.thumb_func
ObjectDispatch_ApplyValueToChildren:
	overlay_veneer ObjectDispatch_ApplyValueToChildrenFar
	.global Func_020021c4
	.thumb_func
Func_020021c4:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_020021cc
	.thumb_func
Func_020021cc:
	overlay_veneer Func_080200c0
	.global Func_020021d4
	.thumb_func
Func_020021d4:
	overlay_veneer Object_Destroy
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_020021e4
	.thumb_func
Func_020021e4:
	overlay_veneer Map_ApplyWorkOriginAndSpanFar
	.global Func_020021ec
	.thumb_func
Func_020021ec:
	overlay_veneer Object_SetPosition
	.global Func_020021f4
	.thumb_func
Func_020021f4:
	overlay_veneer Object_CommitPosition
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
	.global Func_02002204
	.thumb_func
Func_02002204:
	.ifdef TLA_EDITION_DE
	overlay_veneer DisplayBlend_EnableRunScriptFar
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer DisplayBlend_EnableRunScriptFar
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer DisplayBlend_EnableRunScriptFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer DisplayBlend_EnableRunScriptFar
	.else
	overlay_veneer Map_CopyCellsRectFar
	.endif
	.endif
	.endif
	.endif
	.global Func_0200220c
	.thumb_func
Func_0200220c:
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
	.global Func_02002214
	.thumb_func
Func_02002214:
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
	.global Func_02002224
	.thumb_func
Func_02002224:
	overlay_veneer Runtime_SetWorkTripleIfNonNegativeFar
	.global Func_0200222c
	.thumb_func
Func_0200222c:
	overlay_veneer Func_08020230
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_0200223c
	.thumb_func
Func_0200223c:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_08020330
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_08020330
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_08020330
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_08020330
	.else
	overlay_veneer Func_08020360
	.endif
	.endif
	.endif
	.endif
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_0200224c
	.thumb_func
Func_0200224c:
	overlay_veneer EventRuntime_BeginFar
	.global Func_02002254
	.thumb_func
Func_02002254:
	overlay_veneer EventRuntime_EndFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global Func_0200227c
	.thumb_func
Func_0200227c:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_0200229c
	.thumb_func
Func_0200229c:
	overlay_veneer Func_080c8218
	.global Object_AttachWorkTargetToObject
	.thumb_func
Object_AttachWorkTargetToObject:
	overlay_veneer Object_AttachWorkTargetToObjectFar
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_020022b4
	.thumb_func
Func_020022b4:
	overlay_veneer Func_080c8258
	.global Func_020022bc
	.thumb_func
Func_020022bc:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	.ifdef TLA_EDITION_ES
	overlay_veneer Runtime_ScheduleCallbackAndReleaseBlock32BFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Runtime_ScheduleCallbackAndReleaseBlock32BFar
	.else
	overlay_veneer Event_StartScreenTransitionFar
	.endif
	.endif
	.global Event_FinishScreenTransition
	.thumb_func
Event_FinishScreenTransition:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_SetPrimaryBufferValueFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_SetPrimaryBufferValueFar
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
	.global Func_020022dc
	.thumb_func
Func_020022dc:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.endif
	.endif
	.global Func_020022e4
	.thumb_func
Func_020022e4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Camera_WorldToScreenFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Camera_WorldToScreenFar
	.else
	overlay_veneer Func_080c85c0
	.endif
	.endif
	.global Object_Spawn
	.thumb_func
Object_Spawn:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8748
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8748
	.else
	overlay_veneer Object_SpawnFar
	.endif
	.endif
	.global Func_020022f4
	.thumb_func
Func_020022f4:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_StepVector2Far
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_StepVector2Far
	.else
	overlay_veneer Func_080c87e8
	.endif
	.endif
	.global Func_020022fc
	.thumb_func
Func_020022fc:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_SetObjectTickModeFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_SetObjectTickModeFar
	.else
	overlay_veneer Func_080c8830
	.endif
	.endif
	.global Func_02002304
	.thumb_func
Func_02002304:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8830
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8830
	.else
	overlay_veneer Func_080c8840
	.endif
	.endif
	.global Func_0200230c
	.thumb_func
Func_0200230c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8838
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8838
	.else
	overlay_veneer Func_080c8848
	.endif
	.endif
	.global Func_02002314
	.thumb_func
Func_02002314:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8890
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8890
	.else
	overlay_veneer Func_080c88a0
	.endif
	.endif
	.global Func_0200231c
	.thumb_func
Func_0200231c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c88a0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c88a0
	.else
	overlay_veneer Func_080c88b0
	.endif
	.endif
	.global Func_02002324
	.thumb_func
Func_02002324:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c88a8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c88a8
	.else
	overlay_veneer Func_080c88b8
	.endif
	.endif
	.global Func_0200232c
	.thumb_func
Func_0200232c:
	overlay_veneer Audio_PlayCue
