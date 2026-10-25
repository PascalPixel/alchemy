@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer WoodenRoomsA_Initialize
	overlay_veneer WoodenRoomsA_GetWarpDests
	overlay_veneer WoodenRoomsA_GetWarps
	overlay_veneer WoodenRoomsA_GetObjectEventTemplates
	overlay_veneer WoodenRoomsA_GetEvents
	overlay_veneer WoodenRoomsA_GetRegions
