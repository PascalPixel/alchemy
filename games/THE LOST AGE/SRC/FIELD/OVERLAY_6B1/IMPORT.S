@ Import veneers after the OVERLAY_6B1 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6B1 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
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
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global Runtime_BumpAllocate
	.thumb_func
Runtime_BumpAllocate:
	overlay_veneer Runtime_BumpAllocateFar
	.global Sys_Free
	.thumb_func
Sys_Free:
	overlay_veneer Sys_FreeFar
	.global Func_02000c24
	.thumb_func
Func_02000c24:
	overlay_veneer Func_080001a8
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02000c3c
	.thumb_func
Func_02000c3c:
	overlay_veneer Func_080001e8
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
	overlay_veneer Object_SetMode
	.global Func_02000c64
	.thumb_func
Func_02000c64:
	overlay_veneer Object_SetCallback
	.global Func_02000c6c
	.thumb_func
Func_02000c6c:
	overlay_veneer Func_080200c0
	.global Func_02000c74
	.thumb_func
Func_02000c74:
	overlay_veneer Object_SetPosition
	.global Func_02000c7c
	.thumb_func
Func_02000c7c:
	overlay_veneer Object_CommitPosition
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02000c8c
	.thumb_func
Func_02000c8c:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_08020198
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_08020198
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_08020198
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_08020198
	.else
	overlay_veneer Func_080201c8
	.endif
	.endif
	.endif
	.endif
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02000ca4
	.thumb_func
Func_02000ca4:
	overlay_veneer Func_08038210
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02000cb4
	.thumb_func
Func_02000cb4:
	overlay_veneer Func_080c8018
	.global Func_02000cbc
	.thumb_func
Func_02000cbc:
	overlay_veneer Func_080c8020
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_EnableActionAndSetCallback
	.thumb_func
ObjectMotion_EnableActionAndSetCallback:
	overlay_veneer ObjectMotion_EnableActionAndSetCallbackFar
	.global Object_SetActionCallbackAndRefreshById
	.thumb_func
Object_SetActionCallbackAndRefreshById:
	overlay_veneer Object_SetActionCallbackAndRefreshByIdFar
	.global Func_02000cdc
	.thumb_func
Func_02000cdc:
	overlay_veneer Func_080c8110
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global ObjectMotion_SetVariantCallback
	.thumb_func
ObjectMotion_SetVariantCallback:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_02000d04
	.thumb_func
Func_02000d04:
	overlay_veneer Func_080c8280
	.global Func_02000d0c
	.thumb_func
Func_02000d0c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Object_GetTriggerTileAheadOfCurrentFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Object_GetTriggerTileAheadOfCurrentFar
	.else
	overlay_veneer Func_080c82c0
	.endif
	.endif
	.global Func_02000d14
	.thumb_func
Func_02000d14:
	overlay_veneer Func_080c82e0
	.global Func_02000d1c
	.thumb_func
Func_02000d1c:
	overlay_veneer Func_080c8378
	.global Func_02000d24
	.thumb_func
Func_02000d24:
	overlay_veneer Func_080c8380
	.global Func_02000d2c
	.thumb_func
Func_02000d2c:
	overlay_veneer Func_080c8390
	.global Event_SetStatus1c6
	.thumb_func
Event_SetStatus1c6:
	overlay_veneer Event_SetStatus1c6Far
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	.ifdef TLA_EDITION_ES
	overlay_veneer Event_SetStatus1c6Far
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Event_SetStatus1c6Far
	.else
	overlay_veneer Event_WaitValue1c8FramesFar
	.endif
	.endif
	.global Func_02000d44
	.thumb_func
Func_02000d44:
	overlay_veneer Func_080c84e0
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
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87d8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87d8
	.else
	overlay_veneer Func_080c87e8
	.endif
	.endif
	.global Func_02000d64
	.thumb_func
Func_02000d64:
	overlay_veneer Audio_PlayCue
