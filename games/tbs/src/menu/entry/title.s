@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer Title_Run
	overlay_veneer Title_GetWarpDests
	overlay_veneer Title_GetWarps
	overlay_veneer Title_GetObjectEventTemplates
	overlay_veneer Title_GetEvents
	overlay_veneer Scene_GetWarpRegions
