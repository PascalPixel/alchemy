@ Far-call stub table at 0x080c8008: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080c8008
	.thumb_func
Func_080c8008:
	overlay_veneer Func_080c9944
	.global Battle_WaitMode0Far
	.thumb_func
Battle_WaitMode0Far:
	overlay_veneer Battle_WaitMode0
	.global Func_080c8018
	.thumb_func
Func_080c8018:
	overlay_veneer Func_080d22a8
	.global Func_080c8020
	.thumb_func
Func_080c8020:
	overlay_veneer Func_080d2350
	overlay_veneer Func_080d2398
	overlay_veneer Func_080d23b8
	overlay_veneer Func_080cacec
	overlay_veneer Func_080d23cc
	.global Event_PrepareObjectAndApplyValueFar
	.thumb_func
Event_PrepareObjectAndApplyValueFar:
	overlay_veneer Event_PrepareObjectAndApplyValue
	overlay_veneer Event_PrepareTwoObjectsAndApply
	.global Func_080c8058
	.thumb_func
Func_080c8058:
	overlay_veneer Func_080d2488
	.global Func_080c8060
	.thumb_func
Func_080c8060:
	overlay_veneer Func_080d260c
	overlay_veneer Func_080d27ec
	.global Func_080c8070
	.thumb_func
Func_080c8070:
	overlay_veneer Func_080d2840
	.global Func_080c8078
	.thumb_func
Func_080c8078:
	overlay_veneer Func_080d296c
	.global Func_080c8080
	.thumb_func
Func_080c8080:
	overlay_veneer Func_080d2a0c
