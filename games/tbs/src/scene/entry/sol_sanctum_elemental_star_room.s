@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer SolSanctumElementalStarRoom_Initialize
	overlay_veneer SolSanctumElementalStarRoom_GetWarpDests
	overlay_veneer SolSanctumElementalStarRoom_GetWarps
	overlay_veneer SolSanctumElementalStarRoom_GetObjectEventTemplates
	overlay_veneer SolSanctumElementalStarRoom_GetEvents
	overlay_veneer SolSanctumElementalStarRoom_GetRegions
