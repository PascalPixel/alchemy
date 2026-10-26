@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer MenuTest_Initialize
	overlay_veneer MenuTest_GetWarpDests
	overlay_veneer MenuTest_GetWarps
	overlay_veneer MenuTest_GetObjectEventTemplates
	overlay_veneer MenuTest_GetEvents
	overlay_veneer Scene_GetWarpRegions
