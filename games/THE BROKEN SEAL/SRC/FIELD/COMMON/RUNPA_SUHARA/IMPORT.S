@ Import veneers after the RUNPA_SUHARA overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — RUNPA_SUHARA overlay import veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer GameFlag_TestFar
	overlay_veneer GameFlag_SetBitFar
	overlay_veneer GameFlag_ClearBitFar
	overlay_veneer Battle_ResetFar
	overlay_veneer BattleFx_FinishActionFar
	overlay_veneer Object_GetByIdFar
	overlay_veneer Event_SetValue1d8Far
	overlay_veneer BattleEv_RunWaitFar
	overlay_veneer BattleEventRuntime_ProcessActionFar
	overlay_veneer Shop_RunFar
	overlay_veneer Shop_ConfirmActFar
	overlay_veneer Inn_CheckInFar
