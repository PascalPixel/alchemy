@ Far-call stub table at 0x080c8508: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080c8508
	.thumb_func
Func_080c8508:
	overlay_veneer Func_080ceb58
	overlay_veneer BattleFx_HasTrigger
	.global Func_080c8518
	.thumb_func
Func_080c8518:
	overlay_veneer Func_080dc0d8
