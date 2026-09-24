@ Far-call stub table heading the blank-display loader at 0x080f6000: fixed 8-byte
@ veneers, each loading its target into r4 and branching, which the calling
@ convention permits. No compiler emits them; the shared overlay veneer macro
@ builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080f6000
	.thumb_func
Func_080f6000:
	.irp target, 0x080f6009
	overlay_veneer \target
	.endr
