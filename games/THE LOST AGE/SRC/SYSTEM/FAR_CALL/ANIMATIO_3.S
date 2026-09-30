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
	.global Func_08020270
	.thumb_func
Func_08020270:
	overlay_veneer Func_0802c9c8
	.global Animation_ApplyChildValuesFar
	.thumb_func
Animation_ApplyChildValuesFar:
	overlay_veneer Animation_ApplyChildValues
	.global Animation_ApplyChildValuesToRecordFar
	.thumb_func
Animation_ApplyChildValuesToRecordFar:
	overlay_veneer Animation_ApplyChildValuesToRecord
	.global Func_08020288
	.thumb_func
Func_08020288:
	overlay_veneer Func_080227a0
	.global Func_08020290
	.thumb_func
Func_08020290:
	overlay_veneer Func_080236e8
	.global Func_08020298
	.thumb_func
Func_08020298:
	overlay_veneer Func_08024f20
	.global Func_080202a0
	.thumb_func
Func_080202a0:
	overlay_veneer Func_0802b620
