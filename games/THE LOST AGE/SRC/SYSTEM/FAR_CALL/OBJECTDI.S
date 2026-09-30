@ Far-call stub table at 0x08020080: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08020080
	.thumb_func
Func_08020080:
	overlay_veneer Func_08022acc
	overlay_veneer Func_080230e0
	.global Object_SetMode
	.thumb_func
Object_SetMode:
	overlay_veneer ObjectDispatch_ApplyArgumentToChildren
	.global ObjectDispatch_ApplyValueToChildrenFar
	.thumb_func
ObjectDispatch_ApplyValueToChildrenFar:
	overlay_veneer ObjectDispatch_ApplyValueToChildren
	overlay_veneer ObjectDispatch_ApplyPairToChildren
