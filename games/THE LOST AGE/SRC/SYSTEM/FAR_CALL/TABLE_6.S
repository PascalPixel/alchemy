@ Far-call stub table at 0x08020228: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Runtime_SetWorkTripleIfNonNegativeFar
	.thumb_func
Runtime_SetWorkTripleIfNonNegativeFar:
	overlay_veneer Runtime_SetWorkTripleIfNonNegative
	.global Func_08020230
	.thumb_func
Func_08020230:
	overlay_veneer Func_0802dcd8
