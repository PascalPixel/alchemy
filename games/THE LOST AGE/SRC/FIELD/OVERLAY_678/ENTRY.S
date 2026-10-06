.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02001774
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02000058
	overlay_veneer Func_02001148
	overlay_veneer Scene_GetRegions
	overlay_veneer Scene_PrepareMap
