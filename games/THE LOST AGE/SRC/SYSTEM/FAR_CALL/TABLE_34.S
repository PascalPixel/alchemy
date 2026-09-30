@ Far-call stub table at 0x080c8730: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080c8730
	.thumb_func
Func_080c8730:
	overlay_veneer Func_080eab98
	.global Func_080c8738
	.thumb_func
Func_080c8738:
	overlay_veneer Func_080eaf28
	.global Func_080c8740
	.thumb_func
Func_080c8740:
	overlay_veneer Func_080cdc74
	.global Func_080c8748
	.thumb_func
Func_080c8748:
	overlay_veneer Func_080cdd80
	.global Func_080c8750
	.thumb_func
Func_080c8750:
	overlay_veneer Func_080eaeb4
	.global Func_080c8758
	.thumb_func
Func_080c8758:
	overlay_veneer Object_Spawn
	.global Func_080c8760
	.thumb_func
Func_080c8760:
	overlay_veneer Func_080e25e8
	.global Func_080c8768
	.thumb_func
Func_080c8768:
	overlay_veneer Func_080dbcd8
