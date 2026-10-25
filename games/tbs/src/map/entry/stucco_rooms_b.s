@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer StuccoRoomsB_ApplyEntryState
	overlay_veneer StuccoRoomsB_GetWarpDests
	overlay_veneer StuccoRoomsB_GetWarps
	overlay_veneer StuccoRoomsB_SelectObjectEventTemplates
	overlay_veneer StuccoRoomsB_SelectEvents
	overlay_veneer StuccoRoomsB_GetRegions
