.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global FarCall_BlankRunTable
FarCall_BlankRunTable:
	.global Runtime_BlankDisplayAndRunFar
	.thumb_func
Runtime_BlankDisplayAndRunFar:
	overlay_veneer Runtime_BlankDisplayAndRun
