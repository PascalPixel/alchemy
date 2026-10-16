.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global MemAlloc
	.thumb_func
MemAlloc:
	overlay_veneer Runtime_BumpAllocateFar
	.global MemFree
	.thumb_func
MemFree:
	overlay_veneer Sys_FreeFar
	.global Func_020007ac
	.thumb_func
Func_020007ac:
	overlay_veneer Resource_DecodeType01Far
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_020007c4
	.thumb_func
Func_020007c4:
	overlay_veneer DebugText_ClearFar
	.global Func_020007cc
	.thumb_func
Func_020007cc:
	overlay_veneer Ui_LoadWindowGraphicsFar
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
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Func_02000824
	.thumb_func
Func_02000824:
	overlay_veneer Event_SetDestinationFar
	.global BattleFx_GetResourceId
	.thumb_func
BattleFx_GetResourceId:
	overlay_veneer BattleFx_GetResourceIdFar
	.global Func_02000834
	.thumb_func
Func_02000834:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87b0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87b0
	.else
	overlay_veneer Func_080c87c0
	.endif
	.endif
	.global Func_0200083c
	.thumb_func
Func_0200083c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87b8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87b8
	.else
	overlay_veneer Func_080c87c8
	.endif
	.endif
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
