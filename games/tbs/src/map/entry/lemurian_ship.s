@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer LemurianShip_SetupMap
	overlay_veneer LemurianShip_GetWarpDests
	overlay_veneer LemurianShip_GetWarps
	overlay_veneer LemurianShip_GetObjectEventTemplates
	overlay_veneer LemurianShip_GetEvents
	overlay_veneer LemurianShip_GetRegions
