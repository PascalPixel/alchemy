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
	.global WaitTicks
	.thumb_func
WaitTicks:
	overlay_veneer WaitTicksFar
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
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
	.global MemAlloc
	.thumb_func
MemAlloc:
	overlay_veneer Runtime_BumpAllocateFar
	.global MemFree
	.thumb_func
MemFree:
	overlay_veneer Sys_FreeFar
	.global Func_02001bac
	.thumb_func
Func_02001bac:
	overlay_veneer Resource_DecodeType01Far
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02001bc4
	.thumb_func
Func_02001bc4:
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
	.global Func_02001bec
	.thumb_func
Func_02001bec:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_SetAnimationSpeed
	.thumb_func
ObjectDispatch_SetAnimationSpeed:
	overlay_veneer ObjectDispatch_SetAnimationSpeedFar
	.global Func_02001bfc
	.thumb_func
Func_02001bfc:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02001c04
	.thumb_func
Func_02001c04:
	overlay_veneer Func_080200c0
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02001c14
	.thumb_func
Func_02001c14:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02001c1c
	.thumb_func
Func_02001c1c:
	overlay_veneer Object_SetPosition
	.global Func_02001c24
	.thumb_func
Func_02001c24:
	overlay_veneer Object_CommitPosition
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02001c34
	.thumb_func
Func_02001c34:
	overlay_veneer Map_CopyCellsRectFar
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02001c4c
	.thumb_func
Func_02001c4c:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02001c54
	.thumb_func
Func_02001c54:
	overlay_veneer Func_08020230
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Map_EnableUpdateCallback
	.thumb_func
Map_EnableUpdateCallback:
	overlay_veneer Map_EnableUpdateCallbackFar
	.global Map_DisableUpdateCallback
	.thumb_func
Map_DisableUpdateCallback:
	overlay_veneer Map_DisableUpdateCallbackFar
	.global Func_02001c74
	.thumb_func
Func_02001c74:
	overlay_veneer Func_08020360
	.global FieldEvent_WaitTicks
	.thumb_func
FieldEvent_WaitTicks:
	overlay_veneer FieldEvent_WaitTicksFar
	.global Func_02001c84
	.thumb_func
Func_02001c84:
	overlay_veneer EventRuntime_StartFar
	.global Func_02001c8c
	.thumb_func
Func_02001c8c:
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
	.global Func_02001cac
	.thumb_func
Func_02001cac:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02001cc4
	.thumb_func
Func_02001cc4:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02001cdc
	.thumb_func
Func_02001cdc:
	overlay_veneer Func_080c8258
	.global Func_02001ce4
	.thumb_func
Func_02001ce4:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Func_02001cec
	.thumb_func
Func_02001cec:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
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
	.global Func_02001d0c
	.thumb_func
Func_02001d0c:
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global Func_02001d14
	.thumb_func
Func_02001d14:
	overlay_veneer Camera_WorldToScreenFar
	.global Func_02001d1c
	.thumb_func
Func_02001d1c:
	overlay_veneer Func_080c8760
	.global Func_02001d24
	.thumb_func
Func_02001d24:
	overlay_veneer Func_080c87e8
	.global Func_02001d2c
	.thumb_func
Func_02001d2c:
	overlay_veneer Func_080c8830
	.global Func_02001d34
	.thumb_func
Func_02001d34:
	overlay_veneer Func_080c8848
	.global Func_02001d3c
	.thumb_func
Func_02001d3c:
	overlay_veneer Func_080c8880
	.global Func_02001d44
	.thumb_func
Func_02001d44:
	overlay_veneer Func_080c8948
	.global Func_02001d4c
	.thumb_func
Func_02001d4c:
	overlay_veneer Audio_PlayCue
