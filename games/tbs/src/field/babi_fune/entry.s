.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer BabiFune_SetupScene
	overlay_veneer BabiFune_GetEntrances
	overlay_veneer BabiFune_GetExits
	overlay_veneer BabiFune_GetPlacements
	overlay_veneer BabiFune_GetEvents
	overlay_veneer BabiFune_GetDefaultResult
