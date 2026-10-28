.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	overlay_veneer Func_02000ecc
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02000de4
	overlay_veneer Func_02000e58
	overlay_veneer Func_02000db4
	overlay_veneer Scene_PrepareMap
