@ Import veneers after the SUHARA_GATE overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — SUHARA_GATE overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer WaitFramesFar
	overlay_veneer Func_080003c8
	overlay_veneer Func_080003d0
	overlay_veneer Func_08020178
	overlay_veneer Func_080201e8
	overlay_veneer Battle_WaitMode0Far
	overlay_veneer Func_080c8018
	overlay_veneer Func_080c8020
	overlay_veneer Object_GetByIdFar
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	overlay_veneer ObjectMotion_SnapHeadingAndOffsetFar
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	overlay_veneer Func_080c80f8
	overlay_veneer Object_SetModeByIdFar
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	overlay_veneer ObjectMotion_LaunchFar
	overlay_veneer Motion_SetVarCbAndRefreshFar
	overlay_veneer Func_080c8158
	overlay_veneer Func_080c8180
	overlay_veneer Func_080c8198
	overlay_veneer Func_080c81d0
	overlay_veneer Func_080c8210
	overlay_veneer Func_080c8228
	overlay_veneer Func_080c8238
	overlay_veneer Func_080c8240
	overlay_veneer Func_080c8278
	overlay_veneer Func_080c8280
	overlay_veneer Func_080c8288
	overlay_veneer Func_080c8290
	overlay_veneer Func_080c8298
	overlay_veneer Event_SetStatus1c6Far
	overlay_veneer Event_WaitValue1c8FramesFar
	overlay_veneer Func_080c84e0
	overlay_veneer Func_080c85e8
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	overlay_veneer Object_SetActionCallbackFar
	overlay_veneer Func_080c87b0
	overlay_veneer Func_080c87b8
	overlay_veneer Audio_PlayCue
