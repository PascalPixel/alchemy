@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
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
