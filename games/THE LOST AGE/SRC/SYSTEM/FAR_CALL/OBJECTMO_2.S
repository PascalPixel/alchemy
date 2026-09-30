@ Far-call stub table at 0x080c85c8: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080c85c8
	.thumb_func
Func_080c85c8:
	overlay_veneer Func_080d1eac
	.global Event_ClearInvalidPackedValuesFar
	.thumb_func
Event_ClearInvalidPackedValuesFar:
	overlay_veneer Func_080d2454
	overlay_veneer Func_080d7a78
	overlay_veneer Func_080d7ab4
	.global Func_080c85e8
	.thumb_func
Func_080c85e8:
	overlay_veneer Func_080d3070
	.global Func_080c85f0
	.thumb_func
Func_080c85f0:
	overlay_veneer ObjectMotion_OffsetPositionAndReset
	.global Func_080c85f8
	.thumb_func
Func_080c85f8:
	overlay_veneer ObjectMotion_CommitPositionAndActivate
	.global Func_080c8600
	.thumb_func
Func_080c8600:
	overlay_veneer Object_LinkObjectAndSetCallback
	overlay_veneer Func_080e0308
	overlay_veneer Func_080e035c
	overlay_veneer Func_080d3c2c
	.global Func_080c8620
	.thumb_func
Func_080c8620:
	overlay_veneer ObjectTable_ReadActiveValue
