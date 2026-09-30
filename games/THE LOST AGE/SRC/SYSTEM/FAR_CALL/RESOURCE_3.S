@ Far-call stub table at 0x080202a8: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080202a8
	.thumb_func
Func_080202a8:
	overlay_veneer ResourceMetadata_SumCommandLengths
	overlay_veneer Func_0802dc48
	overlay_veneer Func_080227e0
	.global Func_080202c0
	.thumb_func
Func_080202c0:
	overlay_veneer Func_0802d6e8
	.global Func_080202c8
	.thumb_func
Func_080202c8:
	overlay_veneer Animation_ApplyChildValue
	overlay_veneer Func_0802a52c
	.global Func_080202d8
	.thumb_func
Func_080202d8:
	overlay_veneer Func_0802cd4c
	.global Map_DisableUpdateCallbackFar
	.thumb_func
Map_DisableUpdateCallbackFar:
	overlay_veneer Func_0802cd70
	overlay_veneer Func_08024f80
	.global Func_080202f0
	.thumb_func
Func_080202f0:
	overlay_veneer Func_0802d658
	.global Func_080202f8
	.thumb_func
Func_080202f8:
	overlay_veneer Func_08023f94
	.global Func_08020300
	.thumb_func
Func_08020300:
	overlay_veneer Func_0802372c
