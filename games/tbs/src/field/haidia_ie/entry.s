.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer HaidiaIe_RestoreEntryState
	overlay_veneer HaidiaIe_GetEntrances
	overlay_veneer SceneData_SelectTableByFlag834
	overlay_veneer SceneData_SelectTableByFlags834And87a
	overlay_veneer SceneData_SelectTableByFlags87a_815_834
	overlay_veneer HaidiaIe_GetDefaultResult
