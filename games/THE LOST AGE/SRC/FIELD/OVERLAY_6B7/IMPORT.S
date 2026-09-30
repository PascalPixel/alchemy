@ Import veneers after the OVERLAY_6B7 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6B7 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer IwramSignedRemainder
	overlay_veneer Func_080000c0
	overlay_veneer Func_080003c0
	overlay_veneer Func_08038010
	overlay_veneer UiWork_FinalizeFar
	overlay_veneer Func_08038078
	overlay_veneer Func_08038080
	overlay_veneer Func_08038090
	overlay_veneer Func_08038098
	overlay_veneer UiText_DrawNumberAtOffsetFar
	overlay_veneer Func_080380b0
	overlay_veneer Func_08038120
	overlay_veneer Func_08038260
	overlay_veneer Func_08038268
	overlay_veneer Func_08038318
	overlay_veneer Func_08038320
	overlay_veneer Owner_RecalculateStatsFar
	overlay_veneer Item_Get
	overlay_veneer Func_080ad028
	overlay_veneer Func_080ad100
	overlay_veneer Func_080ad198
	overlay_veneer Func_080ad1f8
	overlay_veneer Func_080c8018
	overlay_veneer Func_080c8020
	overlay_veneer Func_080c8088
	overlay_veneer Func_080c80f8
	overlay_veneer Func_080c8118
	overlay_veneer Func_080c8130
	overlay_veneer Func_080c8180
	overlay_veneer Func_080c81a0
	overlay_veneer Func_080c8278
	overlay_veneer Func_080c84e0
	overlay_veneer Func_080f8038
	overlay_veneer Audio_PlayCue
