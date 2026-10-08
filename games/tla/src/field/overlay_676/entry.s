.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	overlay_veneer Func_02002278
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02000218
	overlay_veneer Func_020004ec
	overlay_veneer Scene_GetRegions
	overlay_veneer Func_020022b0
