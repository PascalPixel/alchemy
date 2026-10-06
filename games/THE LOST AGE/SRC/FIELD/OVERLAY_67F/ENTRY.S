.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_020012dc
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_020010b0
	overlay_veneer Func_020012b0
	overlay_veneer Func_02001070
	overlay_veneer Scene_PrepareMap
