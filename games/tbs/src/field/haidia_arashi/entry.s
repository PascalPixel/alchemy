@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer FieldScene_RunFlagGatedActorSetup
	overlay_veneer HaidiaArashi_GetEntrances
	overlay_veneer HaidiaArashi_GetExits
	overlay_veneer HaidiaArashi_GetPlacements
	overlay_veneer HaidiaArashi_GetEvents
	overlay_veneer SceneData_ReturnZero
