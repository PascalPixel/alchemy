@ Far-call stub table at 0x08020268: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08020268
	.thumb_func
Func_08020268:
	overlay_veneer Func_0802c8a0
	overlay_veneer Func_0802c9c8
	.global Func_08020278
	.thumb_func
Func_08020278:
	overlay_veneer Animation_ApplyChildValues
	.global Func_08020280
	.thumb_func
Func_08020280:
	overlay_veneer Animation_ApplyChildValuesToRecord
	overlay_veneer Func_080227a0
	.global Func_08020290
	.thumb_func
Func_08020290:
	overlay_veneer Func_080236e8
	overlay_veneer Func_08024f20
	.global Func_080202a0
	.thumb_func
Func_080202a0:
	overlay_veneer Func_0802b620
