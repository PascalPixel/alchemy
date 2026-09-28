@ Import veneers after the TITLE overlay's code: fixed 8-byte veneers through
@ which the overlay calls main-image code, each loading its target into r4 and
@ branching, which the calling convention permits.
@ credit: reconstructed_veneer — TITLE overlay import veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer WaitFramesFar
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	overlay_veneer Runtime_BumpAllocateAlternatePoolFar
	overlay_veneer Sys_FreeFar
	overlay_veneer Resource_DecodeType01Far
	overlay_veneer VramBlock_LoadCachedFar
	overlay_veneer Resource_FindFreeEntryFar
	overlay_veneer Runtime_PushSlotEntryFar
	overlay_veneer Resource_GetTableEntryFar
	overlay_veneer RuntimeDispatch_NoOpHookFar
	overlay_veneer Blend_SetDarkenTarget16Far
	overlay_veneer Blend_WaitForTransitionFar
	overlay_veneer SaveState_ScanRecordFlagsFar
	overlay_veneer Party_ApplyStatePresetFar
	overlay_veneer Battle_WaitMode0Far
	overlay_veneer Object_GetByIdFar
	overlay_veneer Event_SetPairWork1c0Far
	overlay_veneer Event_SetStatus1c6Far
	overlay_veneer Event_WaitValue1c8FramesFar
	overlay_veneer Func_080f0000
	overlay_veneer Func_080f2000
	overlay_veneer Title_ShowSplashScreenFar
	overlay_veneer Func_080f2020
	overlay_veneer AudioCommand_PlayFar
