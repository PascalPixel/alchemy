@ Import veneers after the OVERLAY_66E overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_66E overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer IwramUnsignedRemainder
	overlay_veneer Func_080000f8
	overlay_veneer Runtime_AllocateBlockFar
	overlay_veneer Runtime_ReleaseHeapBlockFar
	overlay_veneer Owner_GetStateFar
	overlay_veneer Func_080003c8
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer Func_08020300
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	overlay_veneer Func_08038120
	overlay_veneer Item_Get
	overlay_veneer PartyInventory_FindOwnerFar
	overlay_veneer Func_080ad1d8
	overlay_veneer Func_080ad2e8
	overlay_veneer Battle_WaitMode0Far
	overlay_veneer Func_080c8018
	overlay_veneer Func_080c8020
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	overlay_veneer Object_GetByIdFar
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	overlay_veneer Func_080c80f8
	overlay_veneer Object_SetModeByIdFar
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	overlay_veneer ObjectMotion_WaitForAnimationChangeFar
	overlay_veneer ObjectMotion_LaunchFar
	overlay_veneer Func_080c8170
	overlay_veneer Func_080c8180
	overlay_veneer Func_080c8188
	overlay_veneer Func_080c81a0
	overlay_veneer Func_080c8200
	overlay_veneer Func_080c8208
	overlay_veneer Func_080c8210
	overlay_veneer Func_080c8378
	overlay_veneer Func_080c8380
	overlay_veneer Func_080c8390
	overlay_veneer Func_080c84e0
	overlay_veneer Func_080c8778
	overlay_veneer Func_08108008
	overlay_veneer Func_08108018
	overlay_veneer Func_08108070
	overlay_veneer Audio_PlayCue
