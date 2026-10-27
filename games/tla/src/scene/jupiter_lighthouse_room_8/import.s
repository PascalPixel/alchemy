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
	.global Func_02002118
	.thumb_func
Func_02002118:
	overlay_veneer Resource_DecodeType01Far
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02002130
	.thumb_func
Func_02002130:
	overlay_veneer Runtime_PushSlotEntryFar
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
	.global GameFlag_GetByte
	.thumb_func
GameFlag_GetByte:
	overlay_veneer GameFlag_GetByteFar
	.global GameFlag_SetByte
	.thumb_func
GameFlag_SetByte:
	overlay_veneer GameFlag_SetByteFar
	.global Func_02002160
	.thumb_func
Func_02002160:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_SetAnimationSpeed
	.thumb_func
ObjectDispatch_SetAnimationSpeed:
	overlay_veneer ObjectDispatch_SetAnimationSpeedFar
	.global Func_02002170
	.thumb_func
Func_02002170:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02002178
	.thumb_func
Func_02002178:
	overlay_veneer Func_080200c0
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02002188
	.thumb_func
Func_02002188:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02002190
	.thumb_func
Func_02002190:
	overlay_veneer Object_SetPosition
	.global Func_02002198
	.thumb_func
Func_02002198:
	overlay_veneer Object_CommitPosition
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_020021a8
	.thumb_func
Func_020021a8:
	overlay_veneer Map_CopyCellsRectFar
	.global Func_020021b0
	.thumb_func
Func_020021b0:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_020021b8
	.thumb_func
Func_020021b8:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Func_020021c8
	.thumb_func
Func_020021c8:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_020021d0
	.thumb_func
Func_020021d0:
	overlay_veneer Func_08020230
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_020021e0
	.thumb_func
Func_020021e0:
	overlay_veneer Func_08020360
	.global FieldEvent_WaitTicks
	.thumb_func
FieldEvent_WaitTicks:
	overlay_veneer FieldEvent_WaitTicksFar
	.global Func_020021f0
	.thumb_func
Func_020021f0:
	overlay_veneer EventRuntime_StartFar
	.global Func_020021f8
	.thumb_func
Func_020021f8:
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
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02002228
	.thumb_func
Func_02002228:
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
	.global Func_02002248
	.thumb_func
Func_02002248:
	overlay_veneer Func_080c8208
	.global Func_02002250
	.thumb_func
Func_02002250:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02002268
	.thumb_func
Func_02002268:
	overlay_veneer Func_080c8258
	.global Func_02002270
	.thumb_func
Func_02002270:
	overlay_veneer EventRuntime_SetCountdownFar
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
	.global Func_02002290
	.thumb_func
Func_02002290:
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global Func_02002298
	.thumb_func
Func_02002298:
	overlay_veneer Camera_WorldToScreenFar
	.global Func_020022a0
	.thumb_func
Func_020022a0:
	overlay_veneer Event_NoOpFar
	.global Func_020022a8
	.thumb_func
Func_020022a8:
	overlay_veneer Func_080c8738
	.global Func_020022b0
	.thumb_func
Func_020022b0:
	overlay_veneer Func_080c87e8
	.global Func_020022b8
	.thumb_func
Func_020022b8:
	overlay_veneer Func_080c8830
	.global Func_020022c0
	.thumb_func
Func_020022c0:
	overlay_veneer Func_080c8848
	.global Func_020022c8
	.thumb_func
Func_020022c8:
	overlay_veneer Audio_PlayCue
