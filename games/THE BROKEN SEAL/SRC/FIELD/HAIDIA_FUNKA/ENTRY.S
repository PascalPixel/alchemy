@ Entry veneers opening the HAIDIA_FUNKA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — HAIDIA_FUNKA overlay entry veneers
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
