.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global WaitTicks
	.thumb_func
WaitTicks:
	overlay_veneer WaitTicksFar
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
	.global Func_02001f4c
	.thumb_func
Func_02001f4c:
	overlay_veneer Object_SetMode
	.global Func_02001f54
	.thumb_func
Func_02001f54:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Engine_ObjectCreate
	.thumb_func
Engine_ObjectCreate:
	overlay_veneer Func_080200c0
	.global Func_02001f64
	.thumb_func
Func_02001f64:
	overlay_veneer Object_Destroy
	.global Func_02001f6c
	.thumb_func
Func_02001f6c:
	overlay_veneer Object_SetPosition
	.global Func_02001f74
	.thumb_func
Func_02001f74:
	overlay_veneer Map_CopyMetatileIndicesRectFar
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02001f8c
	.thumb_func
Func_02001f8c:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02001f94
	.thumb_func
Func_02001f94:
	overlay_veneer Func_08020230
	.global Func_02001f9c
	.thumb_func
Func_02001f9c:
	overlay_veneer Func_080202f8
	.global FieldEvent_WaitTicks
	.thumb_func
FieldEvent_WaitTicks:
	overlay_veneer FieldEvent_WaitTicksFar
	.global Func_02001fac
	.thumb_func
Func_02001fac:
	overlay_veneer EventRuntime_StartFar
	.global Func_02001fb4
	.thumb_func
Func_02001fb4:
	overlay_veneer EventRuntime_EndFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02001fdc
	.thumb_func
Func_02001fdc:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02001fec
	.thumb_func
Func_02001fec:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02001ffc
	.thumb_func
Func_02001ffc:
	overlay_veneer Func_080c8240
	.global Func_02002004
	.thumb_func
Func_02002004:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Func_0200200c
	.thumb_func
Func_0200200c:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Party_SetFields1eeAnd1f0
	.thumb_func
Party_SetFields1eeAnd1f0:
	overlay_veneer Party_SetFields1eeAnd1f0Far
	.global Func_0200201c
	.thumb_func
Func_0200201c:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_02002024
	.thumb_func
Func_02002024:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
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
	.global Func_02002044
	.thumb_func
Func_02002044:
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global Func_0200204c
	.thumb_func
Func_0200204c:
	overlay_veneer Party_PlaySavedMusicFar
	.global Func_02002054
	.thumb_func
Func_02002054:
	overlay_veneer Audio_PlayCue
