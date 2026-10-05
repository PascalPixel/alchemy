.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02000aa8
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02000440
	overlay_veneer Func_020009dc
	overlay_veneer Scene_GetRegions
	overlay_veneer Scene_PrepareMap
