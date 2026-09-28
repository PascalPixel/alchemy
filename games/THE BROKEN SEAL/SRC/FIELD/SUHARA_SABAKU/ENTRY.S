@ Entry veneers opening the SUHARA_SABAKU overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — SUHARA_SABAKU overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer SuharaSabaku_RunSceneScript
	overlay_veneer SceneData_SelectDataByRuntimeSelector
	overlay_veneer SuharaSabaku_GetExits
	overlay_veneer SceneData_SelectOverlayDataByRuntimeSelector
	overlay_veneer SuharaSabaku_SelectEvents
	overlay_veneer SceneData_ReturnZero
