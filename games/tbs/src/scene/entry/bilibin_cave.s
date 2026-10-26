@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer BilibinCave_Initialize
	overlay_veneer BilibinCave_GetWarpDests
	overlay_veneer BilibinCave_GetWarps
	overlay_veneer BilibinCave_GetObjectEventTemplates
	overlay_veneer BilibinCave_GetEvents
	overlay_veneer BilibinCave_GetRegions
