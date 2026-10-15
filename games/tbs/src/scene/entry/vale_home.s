@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ValeHome_RestoreEntryState
	overlay_veneer ValeHome_GetEntrances
	overlay_veneer SceneData_SelectTableByFlag834
	overlay_veneer SceneData_SelectTableByFlags834And87a
	overlay_veneer SceneData_SelectTableByFlags87a_815_834
	overlay_veneer ValeHome_GetDefaultResult
