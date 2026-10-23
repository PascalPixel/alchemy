@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer IncreaseItemAndLevel_Initialize
	overlay_veneer IncreaseItemAndLevel_GetWarpDests
	overlay_veneer IncreaseItemAndLevel_GetWarps
	overlay_veneer IncreaseItemAndLevel_GetObjectEventTemplates
	overlay_veneer IncreaseItemAndLevel_GetEvents
	overlay_veneer MapData_ReturnZero
