@ Far-call stub table at 0x080201c0: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Map_GetTerrainHeightFar
	.thumb_func
Map_GetTerrainHeightFar:
	overlay_veneer Func_0802d45c
	.global Func_080201c8
	.thumb_func
Func_080201c8:
	overlay_veneer Func_0802d6b0
	.global Func_080201d0
	.thumb_func
Func_080201d0:
	overlay_veneer Func_0802d4d8
	.global Func_080201d8
	.thumb_func
Func_080201d8:
	overlay_veneer Func_0802d530
	.global Func_080201e0
	.thumb_func
Func_080201e0:
	overlay_veneer Func_0802b344
	.global Func_080201e8
	.thumb_func
Func_080201e8:
	overlay_veneer Func_0802b38c
