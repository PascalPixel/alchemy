@ Far-call stub table heading the battle effect code at 0x080c9000: fixed 8-byte
@ veneers, each loading its target into r4 and branching, which the calling
@ convention permits. No compiler emits them; the shared overlay veneer macro
@ builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080c9000
	.thumb_func
Func_080c9000:
	.irp target, 0x080ed409, 0x080e3a3d, 0x080d6579, 0x080d6661
	overlay_veneer \target
	.endr
	.irp target, 0x080cb7f9, 0x080cbc0d, 0x080cc5d9, 0x080ccaed
	overlay_veneer \target
	.endr
	.irp target, 0x080ccbdd
	overlay_veneer \target
	.endr
