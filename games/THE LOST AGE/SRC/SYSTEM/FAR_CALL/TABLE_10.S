@ Far-call stub table at 0x08020358: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08020358
	.thumb_func
Func_08020358:
	overlay_veneer GetWorldMapCollision
	.global Func_08020360
	.thumb_func
Func_08020360:
	overlay_veneer Func_0802d600
	.global Func_08020368
	.thumb_func
Func_08020368:
	overlay_veneer Func_0802d644
