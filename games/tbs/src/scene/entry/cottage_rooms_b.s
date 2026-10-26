@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer CottageRoomsB_Initialize
	overlay_veneer CottageRoomsB_GetWarpDests
	overlay_veneer CottageRoomsB_GetWarps
	overlay_veneer CottageRoomsB_GetObjectEventTemplates
	overlay_veneer CottageRoomsB_GetEvents
	overlay_veneer CottageRoomsB_GetRegions
