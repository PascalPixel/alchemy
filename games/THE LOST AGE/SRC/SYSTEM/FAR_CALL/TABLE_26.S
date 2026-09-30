@ Far-call stub table at 0x080c82a8: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080c82a8
	.thumb_func
Func_080c82a8:
	overlay_veneer Func_080d5bec
	overlay_veneer Func_080cc54c
	overlay_veneer Func_080ca6a4
	.global Func_080c82c0
	.thumb_func
Func_080c82c0:
	overlay_veneer Func_080cd91c
	overlay_veneer Func_080dd054
	overlay_veneer Func_080decb8
	.global Func_080c82d8
	.thumb_func
Func_080c82d8:
	overlay_veneer Func_080cdbf8
	.global Func_080c82e0
	.thumb_func
Func_080c82e0:
	overlay_veneer Func_080d4d08
	.global Func_080c82e8
	.thumb_func
Func_080c82e8:
	overlay_veneer Func_080d50f8
	overlay_veneer Func_080d53b8
	.global Func_080c82f8
	.thumb_func
Func_080c82f8:
	overlay_veneer Func_080d2260
	overlay_veneer Func_080d690c
	overlay_veneer Func_080d7304
	overlay_veneer Func_080d73b4
	overlay_veneer Func_080d73e0
	overlay_veneer Func_080d7408
	overlay_veneer Func_080d7430
	.global Func_080c8330
	.thumb_func
Func_080c8330:
	overlay_veneer Func_080d6b90
	overlay_veneer Func_080d7240
	overlay_veneer Func_080d00f8
	overlay_veneer Func_080d0bec
	.global Func_080c8350
	.thumb_func
Func_080c8350:
	overlay_veneer Func_080d01cc
	overlay_veneer Func_080d0520
	overlay_veneer Func_080d0748
	.global Func_080c8368
	.thumb_func
Func_080c8368:
	overlay_veneer Func_080ccec8
	overlay_veneer Func_080d1684
