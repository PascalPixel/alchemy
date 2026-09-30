@ Far-call stub table at 0x08038410: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global UiIcon_CopyResourceToSlotFar
	.thumb_func
UiIcon_CopyResourceToSlotFar:
	overlay_veneer UiIcon_CopyResourceToSlot
	.global Func_08038418
	.thumb_func
Func_08038418:
	overlay_veneer Func_0803d020
	.global Func_08038420
	.thumb_func
Func_08038420:
	overlay_veneer Func_0803c068
	.global Func_08038428
	.thumb_func
Func_08038428:
	overlay_veneer Func_0803c170
	.global Func_08038430
	.thumb_func
Func_08038430:
	overlay_veneer Func_0803c198
