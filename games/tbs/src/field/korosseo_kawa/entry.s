.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer KorosseoKawa_ApplyEntryState
	overlay_veneer KorosseoKawa_GetEntrances
	overlay_veneer KorosseoKawa_GetExits
	overlay_veneer KorosseoKawa_GetPlacements
	overlay_veneer KorosseoKawa_GetEvents
	overlay_veneer get_default_result
