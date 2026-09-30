@ Far-call stub table at 0x080c8520: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080c8520
	.thumb_func
Func_080c8520:
	overlay_veneer Event_GetSpecialValue
	.global BattleFx_StartItemBreakFar
	.thumb_func
BattleFx_StartItemBreakFar:
	overlay_veneer BattleFx_StartItemBreak
	.global BattleFx_SnapScaleToFullFar
	.thumb_func
BattleFx_SnapScaleToFullFar:
	overlay_veneer BattleFx_SnapScaleToFull
	.global UpdateRisingParticleBurstFar
	.thumb_func
UpdateRisingParticleBurstFar:
	overlay_veneer UpdateRisingParticleBurst
	.global Func_080c8540
	.thumb_func
Func_080c8540:
	overlay_veneer Func_080cb6c8
	.global Func_080c8548
	.thumb_func
Func_080c8548:
	overlay_veneer Func_080cb6f4
	.global Func_080c8550
	.thumb_func
Func_080c8550:
	overlay_veneer Func_080dc0b8
	.global Func_080c8558
	.thumb_func
Func_080c8558:
	overlay_veneer Func_080d295c
	.global Func_080c8560
	.thumb_func
Func_080c8560:
	overlay_veneer Func_080ca18c
	.global Func_080c8568
	.thumb_func
Func_080c8568:
	overlay_veneer Func_080ca1a4
	.global Func_080c8570
	.thumb_func
Func_080c8570:
	overlay_veneer Func_080c9dc8
	.global Func_080c8578
	.thumb_func
Func_080c8578:
	overlay_veneer Func_080ca1bc
	.global Func_080c8580
	.thumb_func
Func_080c8580:
	overlay_veneer Func_080d7788
