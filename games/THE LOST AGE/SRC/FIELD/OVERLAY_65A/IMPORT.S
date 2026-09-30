@ Import veneers after the OVERLAY_65A overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_65A overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer WaitFramesFar
	overlay_veneer Func_080003c8
	overlay_veneer Func_080003d0
	overlay_veneer Func_080003d8
	overlay_veneer Object_SetPositionAndResetMotionFar
	overlay_veneer Object_SetPosition
	overlay_veneer Object_CommitPosition
	overlay_veneer Func_08020178
	overlay_veneer Func_080201c0
	overlay_veneer Func_080201e8
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer Func_080202f0
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	overlay_veneer PartyInventory_RemoveFar
	overlay_veneer Battle_WaitMode0Far
	overlay_veneer Func_080c8018
	overlay_veneer Func_080c8020
	overlay_veneer Object_GetByIdFar
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	overlay_veneer ObjectMotion_EnableActionAndSetCallbackFar
	overlay_veneer Func_080c80f8
	overlay_veneer Func_080c8110
	overlay_veneer Object_SetModeByIdFar
	overlay_veneer Func_080c8170
	overlay_veneer Func_080c8200
	overlay_veneer Func_080c8208
	overlay_veneer Func_080c8230
	overlay_veneer Func_080c8238
	overlay_veneer Func_080c8240
	overlay_veneer Func_080c8278
	overlay_veneer Func_080c82c0
	overlay_veneer Func_080c82d8
	overlay_veneer Event_SetStatus1c6Far
	overlay_veneer Event_ClearStatus1c6Far
	overlay_veneer Event_WaitValue1c8FramesFar
	overlay_veneer Func_080c84e0
	overlay_veneer Func_080c86a8
	overlay_veneer Func_080c86e8
	overlay_veneer Func_080c86f8
	overlay_veneer Func_080c8708
	overlay_veneer Func_080c8710
	overlay_veneer Func_080c8718
	overlay_veneer Func_080c8720
	overlay_veneer Func_080c8728
	overlay_veneer Func_080c8730
	overlay_veneer Func_080c8740
	overlay_veneer Func_080c8748
	overlay_veneer Func_080c8750
	overlay_veneer Audio_PlayCue
