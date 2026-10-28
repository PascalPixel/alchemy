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
	.global Task_RemoveFar
	.thumb_func
Task_RemoveFar:
	overlay_veneer Func_080000d8
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
	.global Func_02001214
	.thumb_func
Func_02001214:
	overlay_veneer Object_SetMode
	.global Func_0200121c
	.thumb_func
Func_0200121c:
	overlay_veneer Object_SetPosition
	.global Func_02001224
	.thumb_func
Func_02001224:
	overlay_veneer Object_CommitPosition
	.global Func_0200122c
	.thumb_func
Func_0200122c:
	overlay_veneer Map_PlayMapEditFar
	.global Func_02001234
	.thumb_func
Func_02001234:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Func_02001244
	.thumb_func
Func_02001244:
	overlay_veneer Func_08038210
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02001254
	.thumb_func
Func_02001254:
	overlay_veneer EventRuntime_StartFar
	.global Func_0200125c
	.thumb_func
Func_0200125c:
	overlay_veneer EventRuntime_EndFar
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
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global Func_0200129c
	.thumb_func
Func_0200129c:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global ObjectMotion_SetVariantCallback
	.thumb_func
ObjectMotion_SetVariantCallback:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global Object_TurnToFaceEachOther
	.thumb_func
Object_TurnToFaceEachOther:
	overlay_veneer Object_LinkPairFar
	.global Func_020012bc
	.thumb_func
Func_020012bc:
	overlay_veneer ObjectGroup_ConfigureChildValueFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_020012cc
	.thumb_func
Func_020012cc:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Func_020012d4
	.thumb_func
Func_020012d4:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Func_020012dc
	.thumb_func
Func_020012dc:
	overlay_veneer Func_080c8288
	.global Party_SetFields1eeAnd1f0
	.thumb_func
Party_SetFields1eeAnd1f0:
	overlay_veneer Party_SetFields1eeAnd1f0Far
	.global Func_020012ec
	.thumb_func
Func_020012ec:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_020012f4
	.thumb_func
Func_020012f4:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global Func_020012fc
	.thumb_func
Func_020012fc:
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
	.global Func_0200131c
	.thumb_func
Func_0200131c:
	overlay_veneer Func_080c8480
	.global Func_02001324
	.thumb_func
Func_02001324:
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global Func_0200132c
	.thumb_func
Func_0200132c:
	overlay_veneer Func_080c88d8
	.global Func_02001334
	.thumb_func
Func_02001334:
	overlay_veneer Func_080c88e8
	.global Func_0200133c
	.thumb_func
Func_0200133c:
	overlay_veneer Func_080c8968
	.global Func_02001344
	.thumb_func
Func_02001344:
	overlay_veneer Sound_PlayFar
