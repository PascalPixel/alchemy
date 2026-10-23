@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer MainMenu_Run
	overlay_veneer MainMenu_GetWarpDests
	overlay_veneer MainMenu_GetWarps
	overlay_veneer MainMenu_GetObjectEventTemplates
	overlay_veneer MainMenu_GetEvents
	overlay_veneer MapData_ReturnZero
