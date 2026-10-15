.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	overlay_veneer Func_02001e64
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02000818
	overlay_veneer Func_02001bec
	overlay_veneer Scene_GetRegions
	overlay_veneer Scene_PrepareMap
