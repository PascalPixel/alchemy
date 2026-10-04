@ Far-call stub table at 0x08020380: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08020380
	.thumb_func
Func_08020380:
.ifdef TLA_EDITION_DE
	overlay_veneer Func_08022c78
.else
.ifdef TLA_EDITION_ES
	overlay_veneer Func_08022c78
.else
.ifdef TLA_EDITION_FR
	overlay_veneer Func_08022c78
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_08022c78
.else
	overlay_veneer VramBlock_Reset93
.endif
.endif
.endif
.endif
	.global Func_08020388
	.thumb_func
Func_08020388:
	overlay_veneer Func_08022c84
	.global Func_08020390
	.thumb_func
Func_08020390:
	overlay_veneer Func_0802a650
	.global Func_08020398
	.thumb_func
Func_08020398:
	overlay_veneer Func_0802de8c
