.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.global FarCall_BlankRunTable
FarCall_BlankRunTable:
	.global Runtime_BlankDisplayAndRunFar
	.thumb_func
Runtime_BlankDisplayAndRunFar:
	overlay_veneer Runtime_BlankDisplayAndRun
