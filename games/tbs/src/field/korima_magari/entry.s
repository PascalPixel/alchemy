@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer KorimaMagari_SetupBoard
	overlay_veneer KorimaMagari_GetEntrances
	overlay_veneer KorimaMagari_GetExits
	overlay_veneer KorimaMagari_GetPlacements
	overlay_veneer KorimaMagari_GetEvents
	overlay_veneer SceneData_ReturnZero
