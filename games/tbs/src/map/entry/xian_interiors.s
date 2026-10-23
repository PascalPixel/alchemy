@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer XianInteriors_Initialize
	overlay_veneer XianInteriors_GetEntrances
	overlay_veneer XianInteriors_GetExits
	overlay_veneer XianInteriors_GetPlacements
	overlay_veneer XianInteriors_GetEvents
	overlay_veneer XianInteriors_GetRegions
