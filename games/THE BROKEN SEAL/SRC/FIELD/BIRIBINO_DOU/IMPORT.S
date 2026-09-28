@ Import veneers after the BIRIBINO_DOU overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — BIRIBINO_DOU overlay import veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer WaitFramesFar
	overlay_veneer ObjectDispatch_ApplyArgumentToChildrenFar
	overlay_veneer Object_SetMoveTargetFar
	overlay_veneer Script_WaitForEventTimeoutFar
	overlay_veneer Map_CopyMetatileIndicesRectFar
	overlay_veneer Map_CopyCellAttributeRectFar
	overlay_veneer Func_080091d8
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer GameFlag_TestFar
	overlay_veneer GameFlag_SetBitFar
	overlay_veneer GameFlag_ClearBitFar
	overlay_veneer Battle_WaitMode0Far
	overlay_veneer Battle_ResetFar
	overlay_veneer BattleFx_FinishActionFar
	overlay_veneer Object_GetByIdFar
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	overlay_veneer Object_SetModeByIdFar
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	overlay_veneer ObjectGroup_ConfigureChildValueFar
	overlay_veneer ObjectMotion_SetActionVariantFar
	overlay_veneer AudioCommand_PlayFar
