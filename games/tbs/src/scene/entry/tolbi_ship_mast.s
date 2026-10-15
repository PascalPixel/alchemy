@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer TolbiShipMast_ApplyEntryState
	overlay_veneer TolbiShipMast_GetEntrances
	overlay_veneer TolbiShipMast_GetExits
	overlay_veneer TolbiShipMast_GetPlacements
	overlay_veneer TolbiShipMast_GetEvents
	overlay_veneer TolbiShipMast_GetRegions
