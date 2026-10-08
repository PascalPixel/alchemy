.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	overlay_veneer Func_02000b64
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Func_02000080
	overlay_veneer Func_02000af4
	overlay_veneer Scene_GetRegions
	overlay_veneer Func_02000e84
