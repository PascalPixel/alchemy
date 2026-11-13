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
	.global Heap_AllocEwram
	.thumb_func
Heap_AllocEwram:
	overlay_veneer Heap_AllocEwramFar
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
	.global Import_OamAddSpriteAtDepthClamped
	.thumb_func
Import_OamAddSpriteAtDepthClamped:
	overlay_veneer Oam_AddSpriteAtDepthClampedFar
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
	.global Resource_ReservedNoOp
	.thumb_func
Resource_ReservedNoOp:
	overlay_veneer Resource_ReservedNoOpFar
	.global ScreenFade_ToBlack
	.thumb_func
ScreenFade_ToBlack:
	overlay_veneer ScreenFade_ToBlackFar
	.global ScreenFade_Wait
	.thumb_func
ScreenFade_Wait:
	overlay_veneer ScreenFade_WaitFar
	.global Save_ScanFileSummaries
	.thumb_func
Save_ScanFileSummaries:
	overlay_veneer Save_ScanFileSummariesFar
	.global WindowColor_Apply
	.thumb_func
WindowColor_Apply:
	overlay_veneer WindowColor_ApplyFar
	.global Party_SetLeaderJenna
	.thumb_func
Party_SetLeaderJenna:
	overlay_veneer Party_SetLeaderJennaFar
	.global Party_SetLeaderFelix
	.thumb_func
Party_SetLeaderFelix:
	overlay_veneer Party_SetLeaderFelixFar
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
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Credits_Run
	.thumb_func
Credits_Run:
	overlay_veneer Credits_RunFar
	.global Title_ShowIntro
	.thumb_func
Title_ShowIntro:
	overlay_veneer Title_ShowIntroFar
	.global Title_ShowNintendoLogo
	.thumb_func
Title_ShowNintendoLogo:
	overlay_veneer Title_ShowNintendoLogoFar
	.global Title_ShowCamelotLogo
	.thumb_func
Title_ShowCamelotLogo:
	overlay_veneer Title_ShowCamelotLogoFar
	.global Func_0200063c
	.thumb_func
Func_0200063c:
	overlay_veneer TitleIntro_RunFar
	.global Sound_Play
	.thumb_func
Sound_Play:
	overlay_veneer Sound_PlayFar
