@ Far-call stub table at 0x08020040: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global ResourceObject_CreateFar
	.thumb_func
ResourceObject_CreateFar:
	overlay_veneer ResourceObject_Create
	.global ResourceObject_ReleaseFar
	.thumb_func
ResourceObject_ReleaseFar:
	overlay_veneer ResourceObject_Release
	.global Func_08020050
	.thumb_func
Func_08020050:
	overlay_veneer Func_08022010
