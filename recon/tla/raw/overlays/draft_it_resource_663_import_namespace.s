@ Preserved import near miss before identifying the complete resident callees.
@ IT: reduced attempt retains 2 map-import namespace mistakes.
@ These reduced veneers retain the original source target names.
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Func_02003b70
	.thumb_func
Func_02003b70:
	overlay_veneer Func_08020198
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
