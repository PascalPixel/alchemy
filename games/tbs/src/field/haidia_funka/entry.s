@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer ConditionalSceneSetup_InitForScene15
	overlay_veneer ConditionalSceneSetup_GetScriptData
	overlay_veneer ConditionalSceneSetup_GetMessageData
	overlay_veneer ConditionalSceneSetup_GetActorData
	overlay_veneer ConditionalSceneSetup_GetEffectData
	overlay_veneer ConditionalSceneSetup_GetInitialState
