@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer BilibinInteriors_Initialize
	overlay_veneer BilibinInteriors_GetWarpDests
	overlay_veneer BilibinInteriors_GetWarps
	overlay_veneer BilibinInteriors_GetObjectEventTemplates
	overlay_veneer BilibinInteriors_GetEvents
	overlay_veneer BilibinInteriors_GetRegions
