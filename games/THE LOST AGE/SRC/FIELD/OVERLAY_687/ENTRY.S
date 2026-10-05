.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02001cdc
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02000410
	overlay_veneer Func_0200160c
	overlay_veneer Scene_GetRegions
	overlay_veneer Func_02001ec8
