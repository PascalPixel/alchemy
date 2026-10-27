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
	.global __umodsi3
	.thumb_func
__umodsi3:
	overlay_veneer IwramUnsignedRemainder
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
	.global Func_02001f54
	.thumb_func
Func_02001f54:
	overlay_veneer Resource_DecodeType01Far
	.global Resource_ResetEntry
	.thumb_func
Resource_ResetEntry:
	overlay_veneer Resource_ResetEntryFar
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02001f74
	.thumb_func
Func_02001f74:
	overlay_veneer DebugText_ClearFar
	.global Func_02001f7c
	.thumb_func
Func_02001f7c:
	overlay_veneer Runtime_PushSlotEntryFar
	.global Render_ProjectPoint
	.thumb_func
Render_ProjectPoint:
	overlay_veneer Render_ProjectPointFar
	.global Func_02001f8c
	.thumb_func
Func_02001f8c:
	overlay_veneer Resource_ReservedNoOpFar
	.global QueueIoWriteDelay2
	.thumb_func
QueueIoWriteDelay2:
	overlay_veneer QueueIoWriteDelay2Far
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
	.global Animation_ApplyChildArgument
	.thumb_func
Animation_ApplyChildArgument:
	overlay_veneer Animation_ApplyChildArgumentFar
	.global Func_02001fbc
	.thumb_func
Func_02001fbc:
	overlay_veneer Object_SetMode
	.global Func_02001fc4
	.thumb_func
Func_02001fc4:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02001fcc
	.thumb_func
Func_02001fcc:
	overlay_veneer Func_080200c0
	.global Func_02001fd4
	.thumb_func
Func_02001fd4:
	overlay_veneer Object_Destroy
	.global Func_02001fdc
	.thumb_func
Func_02001fdc:
	overlay_veneer Map_UpdateCurrentTileBlockFar
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02001ff4
	.thumb_func
Func_02001ff4:
	overlay_veneer Map_SetScreenShakeFar
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02002004
	.thumb_func
Func_02002004:
	overlay_veneer Func_08020350
	.global Func_0200200c
	.thumb_func
Func_0200200c:
	overlay_veneer Func_080ad310
	.global Func_02002014
	.thumb_func
Func_02002014:
	overlay_veneer Func_080ad318
	.global FieldEvent_WaitTicks
	.thumb_func
FieldEvent_WaitTicks:
	overlay_veneer FieldEvent_WaitTicksFar
	.global Func_02002024
	.thumb_func
Func_02002024:
	overlay_veneer EventRuntime_StartFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Func_02002034
	.thumb_func
Func_02002034:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_02002044
	.thumb_func
Func_02002044:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02002054
	.thumb_func
Func_02002054:
	overlay_veneer Event_SetDestinationFar
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
	.global Func_02002074
	.thumb_func
Func_02002074:
	overlay_veneer Func_080c8430
	.global Func_0200207c
	.thumb_func
Func_0200207c:
	overlay_veneer Func_080c8438
	.global Func_02002084
	.thumb_func
Func_02002084:
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global Func_0200208c
	.thumb_func
Func_0200208c:
	overlay_veneer Func_080c8638
	.global Func_02002094
	.thumb_func
Func_02002094:
	overlay_veneer Func_080c86f0
	.global Func_0200209c
	.thumb_func
Func_0200209c:
	overlay_veneer EventRuntime_GetControlledOwnerFar
	.global Func_020020a4
	.thumb_func
Func_020020a4:
	overlay_veneer SceneEvent_FindActiveRecordFar
	.global Func_020020ac
	.thumb_func
Func_020020ac:
	overlay_veneer Resource_FarCall00A + 1
	.global Func_020020b4
	.thumb_func
Func_020020b4:
	overlay_veneer Audio_PlayCue
