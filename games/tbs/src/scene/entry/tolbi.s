@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer Tolbi_ApplyEntryState
	overlay_veneer Tolbi_GetEntrances
	overlay_veneer Tolbi_GetExits
	overlay_veneer Tolbi_GetPlacements
	overlay_veneer Tolbi_SelectEvents
	overlay_veneer Tolbi_GetRegions
