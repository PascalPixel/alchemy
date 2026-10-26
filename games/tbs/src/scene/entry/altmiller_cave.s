@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer AltmillerCave_Initialize
	overlay_veneer AltmillerCave_GetWarpDests
	overlay_veneer AltmillerCave_GetWarps
	overlay_veneer AltmillerCave_GetObjectEventTemplates
	overlay_veneer AltmillerCave_GetEvents
	overlay_veneer AltmillerCave_GetRegions
