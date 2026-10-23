@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer TolbiShipMast_Initialize
	overlay_veneer TolbiShipMast_GetWarpDests
	overlay_veneer TolbiShipMast_GetWarps
	overlay_veneer TolbiShipMast_GetObjectEventTemplates
	overlay_veneer TolbiShipMast_GetEvents
	overlay_veneer TolbiShipMast_GetRegions
