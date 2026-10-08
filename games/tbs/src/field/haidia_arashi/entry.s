.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer FieldScene_RunFlagGatedActorSetup
	overlay_veneer HaidiaArashi_GetEntrances
	overlay_veneer HaidiaArashi_GetExits
	overlay_veneer HaidiaArashi_GetPlacements
	overlay_veneer HaidiaArashi_GetEvents
	overlay_veneer SceneData_ReturnZero
