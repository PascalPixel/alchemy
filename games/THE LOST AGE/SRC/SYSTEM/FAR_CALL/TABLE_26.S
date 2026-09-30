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
	.global Func_080c82b0
	.thumb_func
Func_080c82b0:
	overlay_veneer Func_080cc54c
	.global Func_080c82b8
	.thumb_func
Func_080c82b8:
	overlay_veneer Func_080ca6a4
	.global Func_080c82c0
	.thumb_func
Func_080c82c0:
	overlay_veneer Func_080cd91c
	.global Func_080c82c8
	.thumb_func
Func_080c82c8:
	overlay_veneer Func_080dd054
	.global Func_080c82d0
	.thumb_func
Func_080c82d0:
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
	.global Func_080c82f0
	.thumb_func
Func_080c82f0:
	overlay_veneer Func_080d53b8
	.global Func_080c82f8
	.thumb_func
Func_080c82f8:
	overlay_veneer Func_080d2260
	.global Func_080c8300
	.thumb_func
Func_080c8300:
	overlay_veneer Func_080d690c
	.global Func_080c8308
	.thumb_func
Func_080c8308:
	overlay_veneer Func_080d7304
	.global Func_080c8310
	.thumb_func
Func_080c8310:
	overlay_veneer Func_080d73b4
	.global Func_080c8318
	.thumb_func
Func_080c8318:
	overlay_veneer Func_080d73e0
	.global Func_080c8320
	.thumb_func
Func_080c8320:
	overlay_veneer Func_080d7408
	.global Func_080c8328
	.thumb_func
Func_080c8328:
	overlay_veneer Func_080d7430
	.global Func_080c8330
	.thumb_func
Func_080c8330:
	overlay_veneer Func_080d6b90
	.global Func_080c8338
	.thumb_func
Func_080c8338:
	overlay_veneer Func_080d7240
	.global Func_080c8340
	.thumb_func
Func_080c8340:
	overlay_veneer Func_080d00f8
	.global Func_080c8348
	.thumb_func
Func_080c8348:
	overlay_veneer Func_080d0bec
	.global Func_080c8350
	.thumb_func
Func_080c8350:
	overlay_veneer Func_080d01cc
	.global Func_080c8358
	.thumb_func
Func_080c8358:
	overlay_veneer Func_080d0520
	.global Func_080c8360
	.thumb_func
Func_080c8360:
	overlay_veneer Func_080d0748
	.global Func_080c8368
	.thumb_func
Func_080c8368:
	overlay_veneer Func_080ccec8
	.global Func_080c8370
	.thumb_func
Func_080c8370:
	overlay_veneer Func_080d1684
