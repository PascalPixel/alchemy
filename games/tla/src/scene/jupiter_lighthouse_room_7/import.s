.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global __modsi3
	.thumb_func
__modsi3:
	overlay_veneer IwramSignedRemainder
	.global wait
	.thumb_func
wait:
	overlay_veneer waitFar
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
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global Heap_Alloc
	.thumb_func
Heap_Alloc:
	overlay_veneer Heap_AllocFar
	.global Heap_Release
	.thumb_func
Heap_Release:
	overlay_veneer Sys_FreeFar
	.global Func_02002a8c
	.thumb_func
Func_02002a8c:
	overlay_veneer Resource_DecodeType01Far
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02002aa4
	.thumb_func
Func_02002aa4:
	overlay_veneer Oam_AddSpriteAtDepthFar
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
	.global Func_02002ad4
	.thumb_func
Func_02002ad4:
	overlay_veneer Actor_SetMode
	.global Actor_SetAnimationSpeed
	.thumb_func
Actor_SetAnimationSpeed:
	overlay_veneer Actor_SetAnimationSpeedFar
	.global Func_02002ae4
	.thumb_func
Func_02002ae4:
	overlay_veneer Actor_StartScriptFar
	.global Func_02002aec
	.thumb_func
Func_02002aec:
	overlay_veneer Func_080200c0
	.global Actor_SetPositionAndResetMotion
	.thumb_func
Actor_SetPositionAndResetMotion:
	overlay_veneer Actor_SetPositionAndResetMotionFar
	.global Func_02002afc
	.thumb_func
Func_02002afc:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02002b04
	.thumb_func
Func_02002b04:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02002b0c
	.thumb_func
Func_02002b0c:
	overlay_veneer Actor_CommitPosition
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02002b1c
	.thumb_func
Func_02002b1c:
	overlay_veneer Map_CopyCellsRectFar
	.global Func_02002b24
	.thumb_func
Func_02002b24:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02002b2c
	.thumb_func
Func_02002b2c:
	overlay_veneer Func_080201f0
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Func_02002b3c
	.thumb_func
Func_02002b3c:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02002b44
	.thumb_func
Func_02002b44:
	overlay_veneer Func_08020230
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02002b54
	.thumb_func
Func_02002b54:
	overlay_veneer Func_08020360
	.global Func_02002b5c
	.thumb_func
Func_02002b5c:
	overlay_veneer Func_08020368
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_02002b6c
	.thumb_func
Func_02002b6c:
	overlay_veneer Event_StartFar
	.global Func_02002b74
	.thumb_func
Func_02002b74:
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
	.global Actor_OffsetPositionAndResetMotion
	.thumb_func
Actor_OffsetPositionAndResetMotion:
	overlay_veneer Actor_OffsetPositionAndResetMotionFar
	.global Func_02002ba4
	.thumb_func
Func_02002ba4:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_02002bbc
	.thumb_func
Func_02002bbc:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02002bd4
	.thumb_func
Func_02002bd4:
	overlay_veneer Func_080c8240
	.global Func_02002bdc
	.thumb_func
Func_02002bdc:
	overlay_veneer Func_080c8258
	.global Func_02002be4
	.thumb_func
Func_02002be4:
	overlay_veneer Func_080c8288
	.global Func_02002bec
	.thumb_func
Func_02002bec:
	overlay_veneer Func_080c82e0
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
	.global Func_02002c0c
	.thumb_func
Func_02002c0c:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02002c14
	.thumb_func
Func_02002c14:
	overlay_veneer Camera_WorldToScreenFar
	.global Func_02002c1c
	.thumb_func
Func_02002c1c:
	overlay_veneer Func_080c8710
	.global Func_02002c24
	.thumb_func
Func_02002c24:
	overlay_veneer Func_080c8748
	.global Func_02002c2c
	.thumb_func
Func_02002c2c:
	overlay_veneer Func_080c87e8
	.global Func_02002c34
	.thumb_func
Func_02002c34:
	overlay_veneer Func_080c8830
	.global Func_02002c3c
	.thumb_func
Func_02002c3c:
	overlay_veneer Func_080c8840
	.global Func_02002c44
	.thumb_func
Func_02002c44:
	overlay_veneer Func_080c8848
	.global Func_02002c4c
	.thumb_func
Func_02002c4c:
	overlay_veneer Func_080c8850
	.global Func_02002c54
	.thumb_func
Func_02002c54:
	overlay_veneer Func_080c8858
	.global Func_02002c5c
	.thumb_func
Func_02002c5c:
	overlay_veneer Sound_PlayFar
