@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer GomaCavePillar_Initialize
	overlay_veneer GomaCavePillar_GetEntrances
	overlay_veneer GomaCavePillar_GetExits
	overlay_veneer GomaCavePillar_GetPlacements
	overlay_veneer GomaCavePillar_GetEvents
	overlay_veneer GomaCavePillar_GetRegions
