@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer LacquerRoomsB_Initialize
	overlay_veneer LacquerRoomsB_GetWarpDests
	overlay_veneer LacquerRoomsB_GetWarps
	overlay_veneer LacquerRoomsB_GetObjectEventTemplates
	overlay_veneer LacquerRoomsB_GetEvents
	overlay_veneer LacquerRoomsB_GetRegions
