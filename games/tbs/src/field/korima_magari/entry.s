.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer KorimaMagari_SetupBoard
	overlay_veneer KorimaMagari_GetEntrances
	overlay_veneer KorimaMagari_GetExits
	overlay_veneer KorimaMagari_GetPlacements
	overlay_veneer KorimaMagari_GetEvents
	overlay_veneer SceneData_ReturnZero
