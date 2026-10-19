@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer BilibinInteriors_Initialize
	overlay_veneer BilibinInteriors_GetEntrances
	overlay_veneer BilibinInteriors_GetExits
	overlay_veneer BilibinInteriors_GetPlacements
	overlay_veneer BilibinInteriors_GetEvents
	overlay_veneer BilibinInteriors_GetRegions
