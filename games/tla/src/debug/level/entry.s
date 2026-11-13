.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	overlay_veneer IncreaseItemAndLevel_Initialize
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer IncreaseItemAndLevel_GetActorPlacements
	overlay_veneer IncreaseItemAndLevel_GetTriggers
	overlay_veneer Scene_GetRegions
	overlay_veneer Scene_PrepareMap
