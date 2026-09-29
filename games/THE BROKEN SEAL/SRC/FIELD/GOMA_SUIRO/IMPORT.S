@ Import veneers after the GOMA_SUIRO overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — GOMA_SUIRO overlay import veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global IwramSignedDivideEntry
	.thumb_func
IwramSignedDivideEntry:
	overlay_veneer IwramSignedDivide
	overlay_veneer WaitFramesFar
	overlay_veneer Trig_SinFar
	overlay_veneer Trig_CosFar
	.global Engine_ObjectSetAnimation
	.thumb_func
Engine_ObjectSetAnimation:
	overlay_veneer ObjectDispatch_ApplyArgumentToChildrenFar
	.global Engine_ObjectSetScript
	.thumb_func
Engine_ObjectSetScript:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Engine_ObjectCreate
	.thumb_func
Engine_ObjectCreate:
	overlay_veneer Object_CreateFar
	overlay_veneer Map_CopyCellAttributeRectFar
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer Animation_SetStateField5Bits2To3Far
	overlay_veneer Runtime_SetWorkTripleIfNonNegativeFar
	overlay_veneer Map_WaitWorkValuesBelow256Far
	overlay_veneer GameFlag_TestFar
	overlay_veneer GameFlag_SetBitFar
	overlay_veneer GameFlag_ClearBitFar
	.global Engine_ActorGet
	.thumb_func
Engine_ActorGet:
	overlay_veneer Object_GetByIdFar
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	overlay_veneer Object_SetModeByIdFar
	.global Engine_ObjectSetPalette
	.thumb_func
Engine_ObjectSetPalette:
	overlay_veneer ObjectGroup_SetChildValueFar
	overlay_veneer AudioCommand_PlayFar
