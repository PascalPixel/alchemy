.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_020004d4
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02000070
	overlay_veneer Func_02000478
	overlay_veneer Scene_GetRegions
	overlay_veneer Scene_PrepareMap
