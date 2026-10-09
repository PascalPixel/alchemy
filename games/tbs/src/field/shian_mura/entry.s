@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer ShianMura_SetupScene
	overlay_veneer SceneEffect_GetPrimaryData
	overlay_veneer SceneEffect_GetSecondaryData
	overlay_veneer SceneEffect_PrepareState
	overlay_veneer SceneEffect_GetTertiaryData
	overlay_veneer SceneEffect_GetInitialValue
