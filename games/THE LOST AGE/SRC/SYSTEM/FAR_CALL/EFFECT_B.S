@ Far-call stubs: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080c8588
	.thumb_func
Func_080c8588:
	overlay_veneer Func_080ebc30
	overlay_veneer Func_080ebe70
	overlay_veneer EffectSlot_SetPosition
	overlay_veneer Func_080ebea8
	overlay_veneer Func_080ebeb4
	overlay_veneer Func_080ebec8
	overlay_veneer Func_080ebf68
