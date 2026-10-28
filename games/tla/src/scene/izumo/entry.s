.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	overlay_veneer Func_020009d0
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_020001ac
	overlay_veneer Func_0200049c
	overlay_veneer Scene_GetRegions
	overlay_veneer Scene_PrepareMap
