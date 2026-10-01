@ Import veneers after the OVERLAY_6B3 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6B3 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global Runtime_BumpAllocate
	.thumb_func
Runtime_BumpAllocate:
	overlay_veneer Runtime_BumpAllocateFar
	.global Sys_Free
	.thumb_func
Sys_Free:
	overlay_veneer Sys_FreeFar
	.global Func_020007ac
	.thumb_func
Func_020007ac:
	overlay_veneer Func_080001a8
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_020007c4
	.thumb_func
Func_020007c4:
	overlay_veneer Func_080001d8
	.global Func_020007cc
	.thumb_func
Func_020007cc:
	overlay_veneer Func_080001f0
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	overlay_veneer GameFlag_ClearBitFar
	.global UiWork_InitializeWithResourceCounters
	.thumb_func
UiWork_InitializeWithResourceCounters:
	overlay_veneer UiWork_InitializeWithResourceCountersFar
	.global UiWork_Finalize
	.thumb_func
UiWork_Finalize:
	overlay_veneer UiWork_FinalizeFar
	.global UiText_OpenMessageWindow
	.thumb_func
UiText_OpenMessageWindow:
	overlay_veneer UiText_OpenMessageWindowFar
	.global UiWork_IsComplete
	.thumb_func
UiWork_IsComplete:
	overlay_veneer UiWork_IsCompleteFar
	.global Func_02000804
	.thumb_func
Func_02000804:
	overlay_veneer Func_080380f8
	.global UiText_GetResourceDimensions
	.thumb_func
UiText_GetResourceDimensions:
	overlay_veneer UiText_GetResourceDimensionsFar
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Func_02000824
	.thumb_func
Func_02000824:
	overlay_veneer Event_SetPairWork1c0Far
	.global BattleFx_GetResourceId
	.thumb_func
BattleFx_GetResourceId:
	overlay_veneer BattleFx_GetResourceIdFar
	.global Func_02000834
	.thumb_func
Func_02000834:
	overlay_veneer Func_080c87c0
	.global Func_0200083c
	.thumb_func
Func_0200083c:
	overlay_veneer Func_080c87c8
	.global Func_02000844
	.thumb_func
Func_02000844:
	overlay_veneer Func_081a0008
	.global Func_0200084c
	.thumb_func
Func_0200084c:
	overlay_veneer Func_081a0010
	.global Func_02000854
	.thumb_func
Func_02000854:
	overlay_veneer Func_081a0018
	.global Func_0200085c
	.thumb_func
Func_0200085c:
	overlay_veneer Func_081a0020
	.global Func_02000864
	.thumb_func
Func_02000864:
	overlay_veneer Func_081a0028
	.global Func_0200086c
	.thumb_func
Func_0200086c:
	overlay_veneer Audio_PlayCue
