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
	overlay_veneer Func_080000d0
	.global Runtime_BumpAllocate
	.thumb_func
Runtime_BumpAllocate:
	overlay_veneer Runtime_BumpAllocateFar
	.global Sys_Free
	.thumb_func
Sys_Free:
	overlay_veneer Sys_FreeFar
	overlay_veneer Func_080001a8
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	overlay_veneer Func_080001d8
	overlay_veneer Func_080001f0
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
	overlay_veneer Func_080003d8
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
	overlay_veneer Func_080380f8
	overlay_veneer Func_08038108
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	overlay_veneer Func_080c8268
	.global BattleFx_GetResourceId
	.thumb_func
BattleFx_GetResourceId:
	overlay_veneer BattleFx_GetResourceIdFar
	overlay_veneer Func_080c87c0
	overlay_veneer Func_080c87c8
	overlay_veneer Func_081a0008
	overlay_veneer Func_081a0010
	overlay_veneer Func_081a0018
	overlay_veneer Func_081a0020
	overlay_veneer Func_081a0028
	overlay_veneer Audio_PlayCue
