.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global wait
	.thumb_func
wait:
	overlay_veneer waitFar
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
	.global Heap_Alloc
	.thumb_func
Heap_Alloc:
	overlay_veneer Heap_AllocFar
	.global Heap_Release
	.thumb_func
Heap_Release:
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
	overlay_veneer Window_LoadGraphicsFar
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	overlay_veneer GameFlag_ClearBitFar
	.global WindowSystem_InitField
	.thumb_func
WindowSystem_InitField:
	overlay_veneer WindowSystem_InitFieldFar
	.global Window_Close
	.thumb_func
Window_Close:
	overlay_veneer Window_CloseFar
	.global Message_OpenWindow
	.thumb_func
Message_OpenWindow:
	overlay_veneer Message_OpenWindowFar
	.global Message_AllTyped
	.thumb_func
Message_AllTyped:
	overlay_veneer Message_AllTypedFar
	.global Func_02000804
	.thumb_func
Func_02000804:
	overlay_veneer Func_080380f8
	.global Text_GetMessageSize
	.thumb_func
Text_GetMessageSize:
	overlay_veneer Text_GetMessageSizeFar
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Func_02000824
	.thumb_func
Func_02000824:
	overlay_veneer Event_SetDestinationFar
	.global BattleEffect_GetResourceId
	.thumb_func
BattleEffect_GetResourceId:
	overlay_veneer BattleEffect_GetResourceIdFar
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
	overlay_veneer Sound_PlayFar
