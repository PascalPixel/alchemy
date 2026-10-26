@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer CottageRoomsC_Initialize
	overlay_veneer CottageRoomsC_GetWarpDests
	overlay_veneer CottageRoomsC_GetWarps
	overlay_veneer CottageRoomsC_GetObjectEventTemplates
	overlay_veneer CottageRoomsC_GetEvents
	overlay_veneer CottageRoomsC_GetRegions
