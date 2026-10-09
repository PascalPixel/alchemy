@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.global FarCall_BlankLoadTable
FarCall_BlankLoadTable:
	.global Runtime_BlankDisplayLoadValueAndRunFar
	.thumb_func
Runtime_BlankDisplayLoadValueAndRunFar:
	overlay_veneer Runtime_BlankDisplayLoadValueAndRun
