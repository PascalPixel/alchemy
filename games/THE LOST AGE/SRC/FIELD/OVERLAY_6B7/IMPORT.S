@ Import veneers after the OVERLAY_6B7 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6B7 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_MathRemainder
	.thumb_func
Engine_MathRemainder:
	overlay_veneer IwramSignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Owner_GetState
	.thumb_func
Owner_GetState:
	overlay_veneer Owner_GetStateFar
	.global UiWindow_Create
	.thumb_func
UiWindow_Create:
	overlay_veneer UiWindow_CreateFar
	.global UiWork_Finalize
	.thumb_func
UiWork_Finalize:
	overlay_veneer UiWork_FinalizeFar
	.global UiText_DrawResource
	.thumb_func
UiText_DrawResource:
	overlay_veneer UiText_DrawResourceFar
	.global UiText_DrawCharacterAtOffset
	.thumb_func
UiText_DrawCharacterAtOffset:
	overlay_veneer UiText_DrawCharacterAtOffsetFar
	.global UiText_DrawStringAtOffset
	.thumb_func
UiText_DrawStringAtOffset:
	overlay_veneer UiText_DrawStringAtOffsetFar
	.global UiText_DrawStringInWindow
	.thumb_func
UiText_DrawStringInWindow:
	overlay_veneer UiText_DrawStringInWindowFar
	.global UiText_DrawNumberAtOffset
	.thumb_func
UiText_DrawNumberAtOffset:
	overlay_veneer UiText_DrawNumberAtOffsetFar
	.global UiText_DrawNumberInWindow
	.thumb_func
UiText_DrawNumberInWindow:
	overlay_veneer UiText_DrawNumberInWindowFar
	.global Func_0200068c
	.thumb_func
Func_0200068c:
	overlay_veneer UiText_DrawQuantity
	.global RenderOutput_RedrawSavedRect
	.thumb_func
RenderOutput_RedrawSavedRect:
	overlay_veneer RenderOutput_RedrawSavedRectFar
	.global RenderOutput_ClearList
	.thumb_func
RenderOutput_ClearList:
	overlay_veneer RenderOutput_ClearListFar
	.global Func_020006a4
	.thumb_func
Func_020006a4:
	overlay_veneer Func_08038318
	.global Func_020006ac
	.thumb_func
Func_020006ac:
	overlay_veneer Func_08038320
	.global Owner_RecalculateStats
	.thumb_func
Owner_RecalculateStats:
	overlay_veneer Owner_RecalculateStatsFar
	.global Func_020006bc
	.thumb_func
Func_020006bc:
	overlay_veneer Item_Get
	.global PartyInventory_Add
	.thumb_func
PartyInventory_Add:
	overlay_veneer PartyInventory_AddFar
	.global Party_ListActiveOwners
	.thumb_func
Party_ListActiveOwners:
	overlay_veneer Party_ListActiveOwnersFar
	.global Party_AdvanceOwnerCountToTarget
	.thumb_func
Party_AdvanceOwnerCountToTarget:
	overlay_veneer Party_AdvanceOwnerCountToTargetFar
	.global PartyInventory_HasSpace
	.thumb_func
PartyInventory_HasSpace:
	overlay_veneer PartyInventory_HasSpaceFar
	.global Func_020006e4
	.thumb_func
Func_020006e4:
	overlay_veneer Func_080c8018
	.global Func_020006ec
	.thumb_func
Func_020006ec:
	overlay_veneer Func_080c8020
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Func_020006fc
	.thumb_func
Func_020006fc:
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global ObjectMotion_WaitForAnimationChange
	.thumb_func
ObjectMotion_WaitForAnimationChange:
	overlay_veneer ObjectMotion_WaitForAnimationChangeFar
	.global Func_02000714
	.thumb_func
Func_02000714:
	overlay_veneer Func_080c8180
	.global Func_0200071c
	.thumb_func
Func_0200071c:
	overlay_veneer Func_080c81a0
	.global Func_02000724
	.thumb_func
Func_02000724:
	overlay_veneer Func_080c8278
	.global Func_0200072c
	.thumb_func
Func_0200072c:
	overlay_veneer Func_080c84e0
	.global Func_02000734
	.thumb_func
Func_02000734:
	overlay_veneer Func_080f8038
	.global Func_0200073c
	.thumb_func
Func_0200073c:
	overlay_veneer Audio_PlayCue
