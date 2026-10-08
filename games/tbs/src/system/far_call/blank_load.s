.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.global FarCall_BlankLoadTable
FarCall_BlankLoadTable:
	.global Runtime_BlankDisplayLoadValueAndRunFar
	.thumb_func
Runtime_BlankDisplayLoadValueAndRunFar:
	overlay_veneer Runtime_BlankDisplayLoadValueAndRun
