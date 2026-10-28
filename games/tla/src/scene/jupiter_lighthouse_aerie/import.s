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
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
	.global Task_RemoveFar
	.thumb_func
Task_RemoveFar:
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
	.global HeapSlot_AllocFar
	.thumb_func
HeapSlot_AllocFar:
	overlay_veneer Func_08000140
	.global HeapSlot_Release
	.thumb_func
HeapSlot_Release:
	overlay_veneer HeapSlot_ReleaseFar
	.global Heap_Alloc
	.thumb_func
Heap_Alloc:
	overlay_veneer Heap_AllocFar
	.global Heap_Release
	.thumb_func
Heap_Release:
	overlay_veneer Sys_FreeFar
	.global Func_020029a0
	.thumb_func
Func_020029a0:
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
	.global Func_020029c0
	.thumb_func
Func_020029c0:
	overlay_veneer Oam_AddSpriteAtDepthFar
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
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
	.global Func_020029f0
	.thumb_func
Func_020029f0:
	overlay_veneer Actor_SetMode
	.global Actor_SetAnimationSpeed
	.thumb_func
Actor_SetAnimationSpeed:
	overlay_veneer Actor_SetAnimationSpeedFar
	.global Func_02002a00
	.thumb_func
Func_02002a00:
	overlay_veneer Actor_StartScriptFar
	.global Func_02002a08
	.thumb_func
Func_02002a08:
	overlay_veneer Func_080200c0
	.global Actor_SetPositionAndResetMotion
	.thumb_func
Actor_SetPositionAndResetMotion:
	overlay_veneer Actor_SetPositionAndResetMotionFar
	.global Func_02002a18
	.thumb_func
Func_02002a18:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02002a20
	.thumb_func
Func_02002a20:
	overlay_veneer Actor_ResetMotion
	.global Func_02002a28
	.thumb_func
Func_02002a28:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02002a30
	.thumb_func
Func_02002a30:
	overlay_veneer Actor_CommitPosition
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02002a40
	.thumb_func
Func_02002a40:
	overlay_veneer Map_CopyCellsRectFar
	.global Func_02002a48
	.thumb_func
Func_02002a48:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02002a50
	.thumb_func
Func_02002a50:
	overlay_veneer Func_080201f0
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Func_02002a60
	.thumb_func
Func_02002a60:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02002a68
	.thumb_func
Func_02002a68:
	overlay_veneer Func_08020230
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02002a78
	.thumb_func
Func_02002a78:
	overlay_veneer Func_08020360
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02002a88
	.thumb_func
Func_02002a88:
	overlay_veneer Event_StartFar
	.global Func_02002a90
	.thumb_func
Func_02002a90:
	overlay_veneer Event_EndFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetSpeedParameters
	.thumb_func
Actor_SetSpeedParameters:
	overlay_veneer Actor_SetSpeedParametersFar
	.global Actor_Stop
	.thumb_func
Actor_Stop:
	overlay_veneer Actor_StopFar
	.global Actor_SetPositionAndCommit
	.thumb_func
Actor_SetPositionAndCommit:
	overlay_veneer Actor_SetPositionAndCommitFar
	.global Func_02002ab8
	.thumb_func
Func_02002ab8:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Func_02002ad8
	.thumb_func
Func_02002ad8:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02002af0
	.thumb_func
Func_02002af0:
	overlay_veneer Func_080c8240
	.global Func_02002af8
	.thumb_func
Func_02002af8:
	overlay_veneer Func_080c8258
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
	.global Func_02002b18
	.thumb_func
Func_02002b18:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02002b20
	.thumb_func
Func_02002b20:
	overlay_veneer Camera_WorldToScreenFar
	.global Func_02002b28
	.thumb_func
Func_02002b28:
	overlay_veneer Func_080c8780
	.global Func_02002b30
	.thumb_func
Func_02002b30:
	overlay_veneer Func_080c87c0
	.global Func_02002b38
	.thumb_func
Func_02002b38:
	overlay_veneer Func_080c87c8
	.global Func_02002b40
	.thumb_func
Func_02002b40:
	overlay_veneer Func_080c87d0
	.global Func_02002b48
	.thumb_func
Func_02002b48:
	overlay_veneer Func_080c87e8
	.global Func_02002b50
	.thumb_func
Func_02002b50:
	overlay_veneer Func_080c8830
	.global Func_02002b58
	.thumb_func
Func_02002b58:
	overlay_veneer Func_080c8848
	.global Sound_WaitForJingle
	.thumb_func
Sound_WaitForJingle:
	overlay_veneer Sound_WaitForJingleFar
	.global Func_02002b68
	.thumb_func
Func_02002b68:
	overlay_veneer Sound_PlayFar
