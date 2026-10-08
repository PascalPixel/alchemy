.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Resource_FarCall00F
Resource_FarCall00F:
	.global Runtime_BlankDisplayAndRunSceneFar
	.thumb_func
Runtime_BlankDisplayAndRunSceneFar:
	overlay_veneer Runtime_BlankDisplayAndRunScene
