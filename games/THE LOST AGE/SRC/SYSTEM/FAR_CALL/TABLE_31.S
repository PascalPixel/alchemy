@ Far-call stub table at 0x080c8628: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080c8628
	.thumb_func
Func_080c8628:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080dd698
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080d4698
.else
	overlay_veneer ObjectTable_Snapshot
.endif
.endif
	.global Func_080c8630
	.thumb_func
Func_080c8630:
	overlay_veneer Func_080cae5c
	.global Func_080c8638
	.thumb_func
Func_080c8638:
	overlay_veneer Func_080d46a4
	.global Func_080c8640
	.thumb_func
Func_080c8640:
	overlay_veneer Func_080d4714
