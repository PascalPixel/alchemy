.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Resource_FarCall00D
Resource_FarCall00D:
	.global Runtime_BlankDisplayAndRunFar
	.thumb_func
Runtime_BlankDisplayAndRunFar:
	overlay_veneer Runtime_BlankDisplayAndRun
