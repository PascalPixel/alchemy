@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer SolSanctumLunaSolRooms_Initialize
	overlay_veneer SolSanctumLunaSolRooms_GetWarpDests
	overlay_veneer SolSanctumLunaSolRooms_GetWarps
	overlay_veneer SolSanctumLunaSolRooms_GetObjectEventTemplates
	overlay_veneer SolSanctumLunaSolRooms_GetEvents
	overlay_veneer SolSanctumLunaSolRooms_GetRegions
