@ Import veneers after the OVERLAY_665 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_665 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_080003c8
	overlay_veneer Func_080003d0
	overlay_veneer Func_08020170
	overlay_veneer Func_080201e8
	overlay_veneer PartyInventory_RemoveFar
	overlay_veneer Battle_WaitMode0Far
	overlay_veneer Func_080c8018
	overlay_veneer Func_080c8020
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	overlay_veneer Object_GetByIdFar
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	overlay_veneer ObjectMotion_EnableActionAndSetCallbackFar
	overlay_veneer Object_RefreshSelectorByIdFar
	overlay_veneer ObjectMotion_EnableActionAndResetMotionFar
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	overlay_veneer ObjectMotion_SnapHeadingAndOffsetFar
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	overlay_veneer Func_080c80f8
	overlay_veneer Object_SetModeByIdFar
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	overlay_veneer ObjectMotion_LaunchFar
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	overlay_veneer Motion_SetVarCbAndRefreshFar
	overlay_veneer Func_080c8158
	overlay_veneer Func_080c8180
	overlay_veneer Func_080c8188
	overlay_veneer Func_080c8198
	overlay_veneer Func_080c81a0
	overlay_veneer Func_080c81d0
	overlay_veneer Func_080c8200
	overlay_veneer Func_080c8210
	overlay_veneer Func_080c8218
	overlay_veneer Func_080c8228
	overlay_veneer Func_080c8230
	overlay_veneer Func_080c8238
	overlay_veneer Func_080c8240
	overlay_veneer Func_080c8278
	overlay_veneer Func_080c83b0
	overlay_veneer Func_080c83b8
	overlay_veneer Func_080c83e0
	overlay_veneer Func_080c8408
	overlay_veneer Func_080c84e0
	overlay_veneer ObjectMotion_OffsetPositionAndResetFar
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	overlay_veneer Object_SetActionCallbackFar
	overlay_veneer Func_080c8700
	overlay_veneer Func_080c8708
	overlay_veneer Func_080c8778
	overlay_veneer Func_080c8878
	overlay_veneer Func_08108008
	overlay_veneer Audio_PlayCue
