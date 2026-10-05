.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_020000fc
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_0200004c
	overlay_veneer Func_020000a4
	overlay_veneer Scene_GetRegions
	overlay_veneer Scene_PrepareMap
