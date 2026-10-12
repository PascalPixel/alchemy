@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer HaidiaMura_ApplyEntryState
	overlay_veneer MapStagedScene_SelectPrimaryData
	overlay_veneer MapStagedScene_SelectSecondaryData
	overlay_veneer MapStagedScene_SelectTertiaryData
	overlay_veneer MapStagedScene_SelectQuaternaryData
	overlay_veneer MapStagedScene_GetEmptyData
