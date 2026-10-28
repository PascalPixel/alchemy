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
	.global Heap_Alloc
	.thumb_func
Heap_Alloc:
	overlay_veneer Heap_AllocFar
	.global Heap_Release
	.thumb_func
Heap_Release:
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
	overlay_veneer Oam_AddSpriteAtDepthFar
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
	overlay_veneer Actor_SetMode
	.global Actor_SetAnimationSpeed
	.thumb_func
Actor_SetAnimationSpeed:
	overlay_veneer Actor_SetAnimationSpeedFar
	.global Func_020021c4
	.thumb_func
Func_020021c4:
	overlay_veneer Actor_StartScriptFar
	.global Func_020021cc
	.thumb_func
Func_020021cc:
	overlay_veneer Func_080200c0
	.global Func_020021d4
	.thumb_func
Func_020021d4:
	overlay_veneer Actor_Destroy
	.global Actor_SetPositionAndResetMotion
	.thumb_func
Actor_SetPositionAndResetMotion:
	overlay_veneer Actor_SetPositionAndResetMotionFar
	.global Func_020021e4
	.thumb_func
Func_020021e4:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_020021ec
	.thumb_func
Func_020021ec:
	overlay_veneer Actor_SetTargetPosition
	.global Func_020021f4
	.thumb_func
Func_020021f4:
	overlay_veneer Actor_CommitPosition
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02002204
	.thumb_func
Func_02002204:
	overlay_veneer Map_CopyCellsRectFar
	.global Func_0200220c
	.thumb_func
Func_0200220c:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02002214
	.thumb_func
Func_02002214:
	overlay_veneer Func_080201f0
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Func_02002224
	.thumb_func
Func_02002224:
	overlay_veneer Map_SetScreenShakeFar
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
	overlay_veneer Func_08020360
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_0200224c
	.thumb_func
Func_0200224c:
	overlay_veneer Event_StartFar
	.global Func_02002254
	.thumb_func
Func_02002254:
	overlay_veneer Event_EndFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetSpeedParameters
	.thumb_func
Actor_SetSpeedParameters:
	overlay_veneer Actor_SetSpeedParametersFar
	.global Actor_SetPositionAndCommit
	.thumb_func
Actor_SetPositionAndCommit:
	overlay_veneer Actor_SetPositionAndCommitFar
	.global Actor_ResetAndSetPositionInMode2
	.thumb_func
Actor_ResetAndSetPositionInMode2:
	overlay_veneer Actor_ResetAndSetPositionInMode2Far
	.global Func_0200227c
	.thumb_func
Func_0200227c:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_0200229c
	.thumb_func
Func_0200229c:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_020022b4
	.thumb_func
Func_020022b4:
	overlay_veneer Func_080c8258
	.global Func_020022bc
	.thumb_func
Func_020022bc:
	overlay_veneer Event_SetExitRequestFar
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
	.global Func_020022dc
	.thumb_func
Func_020022dc:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_020022e4
	.thumb_func
Func_020022e4:
	overlay_veneer Camera_WorldToScreenFar
	.global Actor_Spawn
	.thumb_func
Actor_Spawn:
	overlay_veneer Actor_SpawnFar
	.global Func_020022f4
	.thumb_func
Func_020022f4:
	overlay_veneer Func_080c87e8
	.global Func_020022fc
	.thumb_func
Func_020022fc:
	overlay_veneer Func_080c8830
	.global Func_02002304
	.thumb_func
Func_02002304:
	overlay_veneer Func_080c8840
	.global Func_0200230c
	.thumb_func
Func_0200230c:
	overlay_veneer Func_080c8848
	.global Func_02002314
	.thumb_func
Func_02002314:
	overlay_veneer Func_080c88a0
	.global Func_0200231c
	.thumb_func
Func_0200231c:
	overlay_veneer Func_080c88b0
	.global Func_02002324
	.thumb_func
Func_02002324:
	overlay_veneer Func_080c88b8
	.global Func_0200232c
	.thumb_func
Func_0200232c:
	overlay_veneer Sound_PlayFar
