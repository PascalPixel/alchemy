.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer FieldScene_SetupActor27OnEntry
	overlay_veneer SceneData_GetPrimaryTable
	overlay_veneer SceneData_GetSecondaryTable
	overlay_veneer BiribinoHeya_PreparePlacements
	overlay_veneer SceneData_GetTertiaryTable
	overlay_veneer SceneData_ReturnZero
