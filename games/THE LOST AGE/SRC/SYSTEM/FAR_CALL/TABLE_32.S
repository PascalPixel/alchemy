@ Far-call stub table at 0x080c8698: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080c8698
	.thumb_func
Func_080c8698:
	overlay_veneer Func_080d2c9c
	.global Func_080c86a0
	.thumb_func
Func_080c86a0:
	overlay_veneer Func_080ca6e4
	.global Func_080c86a8
	.thumb_func
Func_080c86a8:
	overlay_veneer Func_080d897c
	.global Func_080c86b0
	.thumb_func
Func_080c86b0:
	overlay_veneer Func_080d8d68
	.global Func_080c86b8
	.thumb_func
Func_080c86b8:
	overlay_veneer Func_080d9104
	.global Func_080c86c0
	.thumb_func
Func_080c86c0:
	overlay_veneer Func_080d92d4
	.global Func_080c86c8
	.thumb_func
Func_080c86c8:
	overlay_veneer Func_080d9710
	.global Func_080c86d0
	.thumb_func
Func_080c86d0:
	overlay_veneer Func_080d9b08
	.global Func_080c86d8
	.thumb_func
Func_080c86d8:
	overlay_veneer Func_080d9e98
	.global Func_080c86e0
	.thumb_func
Func_080c86e0:
	overlay_veneer Func_080d9ab0
	.global Func_080c86e8
	.thumb_func
Func_080c86e8:
	overlay_veneer Func_080d9ad0
	.global Func_080c86f0
	.thumb_func
Func_080c86f0:
	overlay_veneer Func_080ed804
	.global Func_080c86f8
	.thumb_func
Func_080c86f8:
	overlay_veneer Func_080ea7c0
	.global Func_080c8700
	.thumb_func
Func_080c8700:
	overlay_veneer Func_080ea8d4
	.global Func_080c8708
	.thumb_func
Func_080c8708:
	overlay_veneer Func_080eaa14
