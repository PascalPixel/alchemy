@ Import veneers after the OVERLAY_6B7 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6B7 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer IwramSignedRemainder
	overlay_veneer WaitFramesFar
	overlay_veneer Owner_GetStateFar
	overlay_veneer UiWindow_CreateFar
	overlay_veneer UiWork_FinalizeFar
	overlay_veneer UiText_DrawResourceFar
	overlay_veneer UiText_DrawCharacterAtOffsetFar
	overlay_veneer UiText_DrawStringAtOffsetFar
	overlay_veneer UiText_DrawStringInWindowFar
	overlay_veneer UiText_DrawNumberAtOffsetFar
	overlay_veneer UiText_DrawNumberInWindowFar
	overlay_veneer UiText_DrawQuantity
	overlay_veneer RenderOutput_RedrawSavedRectFar
	overlay_veneer RenderOutput_ClearListFar
	overlay_veneer Func_08038318
	overlay_veneer Func_08038320
	overlay_veneer Owner_RecalculateStatsFar
	overlay_veneer Item_Get
	overlay_veneer PartyInventory_AddFar
	overlay_veneer Party_ListActiveOwnersFar
	overlay_veneer Party_AdvanceOwnerCountToTargetFar
	overlay_veneer PartyInventory_HasSpaceFar
	overlay_veneer Func_080c8018
	overlay_veneer Func_080c8020
	overlay_veneer Object_GetByIdFar
	overlay_veneer Func_080c80f8
	overlay_veneer Object_SetModeByIdFar
	overlay_veneer ObjectMotion_WaitForAnimationChangeFar
	overlay_veneer Func_080c8180
	overlay_veneer Func_080c81a0
	overlay_veneer Func_080c8278
	overlay_veneer Func_080c84e0
	overlay_veneer Func_080f8038
	overlay_veneer Audio_PlayCue
