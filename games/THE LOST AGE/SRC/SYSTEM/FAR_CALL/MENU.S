@ Far-call stub table at 0x080381e0: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080381e0
	.thumb_func
Func_080381e0:
	overlay_veneer Func_0803d2f0
	overlay_veneer Func_0803f778
	.global Menu_SelectSaveSlotActionFar
	.thumb_func
Menu_SelectSaveSlotActionFar:
	overlay_veneer Menu_SelectSaveSlotAction
	.global Func_080381f8
	.thumb_func
Func_080381f8:
	overlay_veneer Func_0804d7fc
