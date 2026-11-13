.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
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
	overlay_veneer Heap_ReleaseFar
	.global Decompress_TaggedLz
	.thumb_func
Decompress_TaggedLz:
	overlay_veneer Decompress_TaggedLzFar
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global VramCache_FindFreeEntry
	.thumb_func
VramCache_FindFreeEntry:
	overlay_veneer VramCache_FindFreeEntryFar
	.global DebugText_Clear
	.thumb_func
DebugText_Clear:
	overlay_veneer DebugText_ClearFar
	.global Window_LoadGraphics
	.thumb_func
Window_LoadGraphics:
	overlay_veneer Window_LoadGraphicsFar
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
	.global GameFlag_Clear
	.thumb_func
GameFlag_Clear:
	overlay_veneer GameFlag_ClearFar
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
	.global Portrait_OpenWindow
	.thumb_func
Portrait_OpenWindow:
	overlay_veneer Portrait_OpenWindowFar
	.global Text_GetMessageSize
	.thumb_func
Text_GetMessageSize:
	overlay_veneer Text_GetMessageSizeFar
	.global Event_Wait
	.thumb_func
Event_Wait:
	overlay_veneer Event_WaitFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Event_SetDestination
	.thumb_func
Event_SetDestination:
	overlay_veneer Event_SetDestinationFar
	.global Speaker_GetVoice
	.thumb_func
Speaker_GetVoice:
	overlay_veneer Speaker_GetVoiceFar
	.global EffectSprite_Init
	.thumb_func
EffectSprite_Init:
	overlay_veneer EffectSprite_InitFar
	.global EffectSprite_Draw
	.thumb_func
EffectSprite_Draw:
	overlay_veneer EffectSprite_DrawFar
	.global EndingPicture_Start
	.thumb_func
EndingPicture_Start:
	overlay_veneer EndingPicture_StartFar
	.global EndingPicture_Show
	.thumb_func
EndingPicture_Show:
	overlay_veneer EndingPicture_ShowFar
	.global SceneState_SetScrollTarget
	.thumb_func
SceneState_SetScrollTarget:
	overlay_veneer SceneState_SetScrollTargetFar
	.global EndingPicture_WaitForScroll
	.thumb_func
EndingPicture_WaitForScroll:
	overlay_veneer EndingPicture_WaitForScrollFar
	.global EndingPicture_LowerTopStrip
	.thumb_func
EndingPicture_LowerTopStrip:
	overlay_veneer EndingPicture_LowerTopStripFar
	.global Sound_Play
	.thumb_func
Sound_Play:
	overlay_veneer Sound_PlayFar
