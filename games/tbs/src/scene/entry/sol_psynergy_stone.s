@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer Resource37c_Run
	overlay_veneer State_Run
	overlay_veneer SceneData_GetPrimaryTable37c
	overlay_veneer SceneData_GetSecondaryTable37c
	overlay_veneer SceneData_GetTertiaryTable37c
	overlay_veneer SolPsynergyStone_GetView
