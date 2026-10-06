.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Resource_FarCall00D
Resource_FarCall00D:
	.global Runtime_BlankDisplayAndRunFar
	.thumb_func
Runtime_BlankDisplayAndRunFar:
	overlay_veneer Runtime_BlankDisplayAndRun
