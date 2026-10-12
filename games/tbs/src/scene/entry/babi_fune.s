@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer BabiFune_SetupScene
	overlay_veneer BabiFune_GetEntrances
	overlay_veneer BabiFune_GetExits
	overlay_veneer BabiFune_GetPlacements
	overlay_veneer BabiFune_GetEvents
	overlay_veneer BabiFune_GetDefaultResult
