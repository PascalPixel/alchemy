@ Import veneers after the GOMA_IKE overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — GOMA_IKE overlay import veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Resource388_QueueSlotCommand
	.thumb_func
Resource388_QueueSlotCommand:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Resource388_SetSlotOption
	.thumb_func
Resource388_SetSlotOption:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Resource388_TestSetupGate
	.thumb_func
Resource388_TestSetupGate:
	overlay_veneer GameFlag_TestFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Resource388_SetSlotPosition
	.thumb_func
Resource388_SetSlotPosition:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Resource388_SetSlotMode
	.thumb_func
Resource388_SetSlotMode:
	overlay_veneer Object_SetModeByIdFar
