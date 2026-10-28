.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global __umodsi3
	.thumb_func
__umodsi3:
	overlay_veneer IwramUnsignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global Heap_Alloc
	.thumb_func
Heap_Alloc:
	overlay_veneer Heap_AllocFar
	.global Heap_Release
	.thumb_func
Heap_Release:
	overlay_veneer Sys_FreeFar
	.global Func_02000c24
	.thumb_func
Func_02000c24:
	overlay_veneer Resource_DecodeType01Far
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02000c3c
	.thumb_func
Func_02000c3c:
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
	.global Func_02000c5c
	.thumb_func
Func_02000c5c:
	overlay_veneer Actor_SetMode
	.global Func_02000c64
	.thumb_func
Func_02000c64:
	overlay_veneer Actor_StartScriptFar
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Func_080200c0
	.global Func_02000c74
	.thumb_func
Func_02000c74:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02000c7c
	.thumb_func
Func_02000c7c:
	overlay_veneer Actor_CommitPosition
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02000c8c
	.thumb_func
Func_02000c8c:
	overlay_veneer GetMapCellCollisionFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02000ca4
	.thumb_func
Func_02000ca4:
	overlay_veneer Func_08038210
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02000cb4
	.thumb_func
Func_02000cb4:
	overlay_veneer Event_StartFar
	.global Func_02000cbc
	.thumb_func
Func_02000cbc:
	overlay_veneer Event_EndFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetMovementTypeById
	.thumb_func
Actor_SetMovementTypeById:
	overlay_veneer Actor_SetMovementTypeByIdFar
	.global Actor_SetMovementTypeAndWaitById
	.thumb_func
Actor_SetMovementTypeAndWaitById:
	overlay_veneer Actor_SetMovementTypeAndWaitByIdFar
	.global Func_02000cdc
	.thumb_func
Func_02000cdc:
	overlay_veneer Actor_MoveToObjectFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_StartRepeatedMotion
	.thumb_func
Actor_StartRepeatedMotion:
	overlay_veneer Actor_StartRepeatedMotionFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Func_02000d04
	.thumb_func
Func_02000d04:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Func_02000d0c
	.thumb_func
Func_02000d0c:
	overlay_veneer Func_080c82c0
	.global Func_02000d14
	.thumb_func
Func_02000d14:
	overlay_veneer Func_080c82e0
	.global Func_02000d1c
	.thumb_func
Func_02000d1c:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_02000d24
	.thumb_func
Func_02000d24:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global Func_02000d2c
	.thumb_func
Func_02000d2c:
	overlay_veneer BattleEffect_StartBufferInterpolationFar
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Func_02000d44
	.thumb_func
Func_02000d44:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02000d4c
	.thumb_func
Func_02000d4c:
	overlay_veneer Func_080c8740
	.global Func_02000d54
	.thumb_func
Func_02000d54:
	overlay_veneer Func_080c8748
	.global Func_02000d5c
	.thumb_func
Func_02000d5c:
	overlay_veneer Func_080c87e8
	.global Func_02000d64
	.thumb_func
Func_02000d64:
	overlay_veneer Sound_PlayFar
