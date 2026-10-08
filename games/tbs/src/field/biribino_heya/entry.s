.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer FieldScene_SetupActor27OnEntry
	overlay_veneer SceneData_GetPrimaryTable
	overlay_veneer SceneData_GetSecondaryTable
	overlay_veneer BiribinoHeya_PreparePlacements
	overlay_veneer SceneData_GetTertiaryTable
	overlay_veneer SceneData_ReturnZero
