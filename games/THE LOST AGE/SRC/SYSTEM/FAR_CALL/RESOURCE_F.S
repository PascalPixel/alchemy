.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Resource_FarCall00F
Resource_FarCall00F:
	.global Runtime_BlankDisplayAndRunSceneFar
	.thumb_func
Runtime_BlankDisplayAndRunSceneFar:
	overlay_veneer Runtime_BlankDisplayAndRunScene
