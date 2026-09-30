@ Far-call stub table at 0x08020040: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08020040
	.thumb_func
Func_08020040:
	overlay_veneer Func_08022d40
	.global ResourceObject_ReleaseFar
	.thumb_func
ResourceObject_ReleaseFar:
	overlay_veneer Func_08022e90
	.global Func_08020050
	.thumb_func
Func_08020050:
	overlay_veneer Func_08022010
