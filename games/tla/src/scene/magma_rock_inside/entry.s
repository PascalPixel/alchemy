.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	overlay_veneer Func_02003678
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_020008f8
	overlay_veneer Func_02003418
	overlay_veneer Scene_GetRegions
	overlay_veneer Func_02003ee0
