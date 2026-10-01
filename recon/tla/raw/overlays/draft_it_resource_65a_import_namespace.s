@ Preserved import near miss before identifying the complete resident callees.
@ IT: reduced attempt retains 1 map-import namespace mistakes.
@ These reduced veneers retain the original source target names.
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
