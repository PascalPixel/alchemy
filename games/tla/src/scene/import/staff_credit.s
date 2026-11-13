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
	.global __umodsi3
	.thumb_func
__umodsi3:
	overlay_veneer IwramUnsignedRemainder
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
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global Heap_AllocEwram
	.thumb_func
Heap_AllocEwram:
	overlay_veneer Heap_AllocEwramFar
	.global Heap_Release
	.thumb_func
Heap_Release:
	overlay_veneer Sys_FreeFar
	.global Resource_DecodeType01
	.thumb_func
Resource_DecodeType01:
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
	.global DebugText_Clear
	.thumb_func
DebugText_Clear:
	overlay_veneer DebugText_ClearFar
	.global Func_02001f7c
	.thumb_func
Func_02001f7c:
	overlay_veneer Oam_AddSpriteAtDepthFar
	.global Render_ProjectPoint
	.thumb_func
Render_ProjectPoint:
	overlay_veneer Render_ProjectPointFar
	.global Resource_ReservedNoOp
	.thumb_func
Resource_ReservedNoOp:
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
	overlay_veneer Actor_SetMode
	.global Actor_StartScript
	.thumb_func
Actor_StartScript:
	overlay_veneer Actor_StartScriptFar
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Func_080200c0
	.global Func_02001fd4
	.thumb_func
Func_02001fd4:
	overlay_veneer Actor_Destroy
	.global Map_UpdateCurrentTileBlock
	.thumb_func
Map_UpdateCurrentTileBlock:
	overlay_veneer Map_UpdateCurrentTileBlockFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Map_SetScreenShake
	.thumb_func
Map_SetScreenShake:
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
	.global Event_Wait
	.thumb_func
Event_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Event_Start
	.thumb_func
Event_Start:
	overlay_veneer Event_StartFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetHorizontalPositionWithTerrain
	.thumb_func
Actor_SetHorizontalPositionWithTerrain:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Camera_SetSpeedParameters
	.thumb_func
Camera_SetSpeedParameters:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Event_SetDestination
	.thumb_func
Event_SetDestination:
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
	.global Event_ResolvePendingActions
	.thumb_func
Event_ResolvePendingActions:
	overlay_veneer Event_ResolvePendingActionsFar
	.global WorldMap_StartZoom
	.thumb_func
WorldMap_StartZoom:
	overlay_veneer WorldMap_StartZoomFar
	.global Func_02002094
	.thumb_func
Func_02002094:
	overlay_veneer FlagEntries_GetTableFar
	.global Party_GetLeaderActor
	.thumb_func
Party_GetLeaderActor:
	overlay_veneer Party_GetLeaderActorFar
	.global SceneEvent_FindActiveRecord
	.thumb_func
SceneEvent_FindActiveRecord:
	overlay_veneer SceneEvent_FindActiveRecordFar
	.global Func_020020ac
	.thumb_func
Func_020020ac:
	overlay_veneer Resource_FarCall00A + 1
	.global Sound_Play
	.thumb_func
Sound_Play:
	overlay_veneer Sound_PlayFar
