.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02000a50
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02000064
	overlay_veneer Func_02000218
	overlay_veneer Scene_GetRegions
	overlay_veneer Func_02000c0c
