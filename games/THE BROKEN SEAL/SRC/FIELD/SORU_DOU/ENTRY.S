.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Resource37c_Run
	overlay_veneer State_Run
	overlay_veneer SceneData_GetPrimaryTable37c
	overlay_veneer SceneData_GetSecondaryTable37c
	overlay_veneer SceneData_GetTertiaryTable37c
	overlay_veneer SoruDou_GetView
