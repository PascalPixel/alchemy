@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer TolbiShipInteriors_ApplyEntryState
	overlay_veneer TolbiShipInteriors_SelectWarpDests
	overlay_veneer TolbiShipInteriors_GetWarps
	overlay_veneer TolbiShipInteriors_SelectObjectEventTemplates
	overlay_veneer TolbiShipInteriors_SelectEvents
	overlay_veneer TolbiShipInteriors_GetRegions
