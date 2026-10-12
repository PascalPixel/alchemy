.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Func_080c8710
	.thumb_func
Func_080c8710:
	overlay_veneer Func_080eab70
	.global Func_080c8718
	.thumb_func
Func_080c8718:
	overlay_veneer Func_080eabd0
	.global Func_080c8720
	.thumb_func
Func_080c8720:
	overlay_veneer Func_080eace8
	.global Func_080c8728
	.thumb_func
Func_080c8728:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080eaf28
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080eaf28
.else
	overlay_veneer Func_080eadfc
.endif
.endif
