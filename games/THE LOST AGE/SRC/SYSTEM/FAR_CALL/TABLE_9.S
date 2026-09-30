@ Far-call stub table at 0x08020340: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08020340
	.thumb_func
Func_08020340:
	overlay_veneer Func_080237e8
	.global Func_08020348
	.thumb_func
Func_08020348:
	overlay_veneer Func_08026f80
	.global Func_08020350
	.thumb_func
Func_08020350:
	overlay_veneer Func_0802da88
