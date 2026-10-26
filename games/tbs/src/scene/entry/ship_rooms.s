@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ShipRooms_ApplyEntryState
	overlay_veneer ShipRooms_SelectWarpDests
	overlay_veneer ShipRooms_GetWarps
	overlay_veneer ShipRooms_SelectObjectEventTemplates
	overlay_veneer ShipRooms_SelectEvents
	overlay_veneer ShipRooms_GetRegions
