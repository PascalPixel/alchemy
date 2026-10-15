@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer TolbiShipDeck_ResetDeck
	overlay_veneer TolbiShipDeck_GetEntrances
	overlay_veneer TolbiShipDeck_GetExits
	overlay_veneer TolbiShipDeck_GetPlacements
	overlay_veneer TolbiShipDeck_GetEvents
	overlay_veneer TolbiShipDeck_GetRegions
