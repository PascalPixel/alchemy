.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_020013b8
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_020004c8
	overlay_veneer Func_02001044
	overlay_veneer Func_02000488
	overlay_veneer Scene_PrepareMap
