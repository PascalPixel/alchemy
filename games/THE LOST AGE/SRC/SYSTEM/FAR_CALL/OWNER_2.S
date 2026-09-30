@ Far-call stub table at 0x080ad318: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080ad318
	.thumb_func
Func_080ad318:
	overlay_veneer Func_080adc90
	.global Func_080ad320
	.thumb_func
Func_080ad320:
	overlay_veneer Func_080ae868
	.global Owner_SumDjinnCountsFar
	.thumb_func
Owner_SumDjinnCountsFar:
	overlay_veneer Owner_SumDjinnCounts
	.global Func_080ad330
	.thumb_func
Func_080ad330:
	overlay_veneer Func_080ae5fc
