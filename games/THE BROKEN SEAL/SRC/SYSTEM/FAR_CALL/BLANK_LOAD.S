.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global FarCall_BlankLoadTable
FarCall_BlankLoadTable:
	.global Runtime_BlankDisplayLoadValueAndRunFar
	.thumb_func
Runtime_BlankDisplayLoadValueAndRunFar:
	overlay_veneer Runtime_BlankDisplayLoadValueAndRun
