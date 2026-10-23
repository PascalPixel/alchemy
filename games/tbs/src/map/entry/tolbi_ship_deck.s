@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer TolbiShipDeck_Initialize
	overlay_veneer TolbiShipDeck_GetWarpDests
	overlay_veneer TolbiShipDeck_GetWarps
	overlay_veneer TolbiShipDeck_GetObjectEventTemplates
	overlay_veneer TolbiShipDeck_GetEvents
	overlay_veneer TolbiShipDeck_GetRegions
