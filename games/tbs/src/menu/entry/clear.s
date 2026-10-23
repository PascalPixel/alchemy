@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer SaveMenu_Run
	overlay_veneer Clear_GetWarpDests
	overlay_veneer Clear_GetWarps
	overlay_veneer Clear_GetObjectEventTemplates
	overlay_veneer Clear_GetEvents
	overlay_veneer MapData_ReturnZero
