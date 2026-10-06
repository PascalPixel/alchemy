.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer ConditionalSceneSetup_InitForScene15
	overlay_veneer ConditionalSceneSetup_GetScriptData
	overlay_veneer ConditionalSceneSetup_GetMessageData
	overlay_veneer ConditionalSceneSetup_GetActorData
	overlay_veneer ConditionalSceneSetup_GetEffectData
	overlay_veneer ConditionalSceneSetup_GetInitialState
