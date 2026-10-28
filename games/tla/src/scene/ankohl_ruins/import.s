.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global wait
	.thumb_func
wait:
	overlay_veneer waitFar
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
	.ifndef TLA_EDITION_JA
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.endif
	.ifndef TLA_EDITION_JA
	.global Math_Cosine
	.thumb_func
Math_Cosine:
	overlay_veneer Math_CosineFar
	.endif
	.global Heap_Alloc
	.thumb_func
Heap_Alloc:
	overlay_veneer Heap_AllocFar
	.global Heap_Release
	.thumb_func
Heap_Release:
	overlay_veneer Sys_FreeFar
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02002dac
	.thumb_func
Func_02002dac:
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
	.global Func_02002dcc
	.thumb_func
Func_02002dcc:
	overlay_veneer Task_FindFar
	.global Func_02002dd4
	.thumb_func
Func_02002dd4:
	overlay_veneer Actor_SetMode
	.global Func_02002ddc
	.thumb_func
Func_02002ddc:
	overlay_veneer Actor_StartScriptFar
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Func_080200c0
	.global Func_02002dec
	.thumb_func
Func_02002dec:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02002df4
	.thumb_func
Func_02002df4:
	overlay_veneer Map_CopyCellsRectFar
	.global Func_02002dfc
	.thumb_func
Func_02002dfc:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02002e04
	.thumb_func
Func_02002e04:
	overlay_veneer Func_080201f0
	.global Func_02002e0c
	.thumb_func
Func_02002e0c:
	overlay_veneer Func_08020210
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02002e24
	.thumb_func
Func_02002e24:
	overlay_veneer Map_SetScreenShakeFar
	.ifndef TLA_EDITION_JA
	.global Func_02002e2c
	.thumb_func
Func_02002e2c:
	overlay_veneer Func_08020230
	.endif
	.global Func_02002e34
	.thumb_func
Func_02002e34:
	overlay_veneer Actor_SetSpritePriorityDirectFar
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_02002e44
	.thumb_func
Func_02002e44:
	overlay_veneer Event_StartFar
	.global Func_02002e4c
	.thumb_func
Func_02002e4c:
	overlay_veneer Event_EndFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_OffsetPositionAndResetMotion
	.thumb_func
Actor_OffsetPositionAndResetMotion:
	overlay_veneer Actor_OffsetPositionAndResetMotionFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_02002e6c
	.thumb_func
Func_02002e6c:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Func_02002e7c
	.thumb_func
Func_02002e7c:
	overlay_veneer Actor_JumpDownLedgeFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_02002e94
	.thumb_func
Func_02002e94:
	overlay_veneer Func_080c8210
	.global Func_02002e9c
	.thumb_func
Func_02002e9c:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02002eb4
	.thumb_func
Func_02002eb4:
	overlay_veneer Func_080c8240
	.global Func_02002ebc
	.thumb_func
Func_02002ebc:
	overlay_veneer Event_SetExitRequestFar
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.ifndef TLA_EDITION_JA
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.endif
	.global Func_02002ed4
	.thumb_func
Func_02002ed4:
	overlay_veneer Func_080c8480
	.global Func_02002edc
	.thumb_func
Func_02002edc:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02002ee4
	.thumb_func
Func_02002ee4:
	overlay_veneer ObjectTable_SnapshotFar
	.global Func_02002eec
	.thumb_func
Func_02002eec:
	overlay_veneer Sound_PlayFar
