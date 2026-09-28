@ Entry veneers opening the SORU_SEKIZO overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — SORU_SEKIZO overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Scene_RunGuardSequenceB
	overlay_veneer SceneEventRuntime_GetScriptData
	overlay_veneer SceneEventRuntime_GetMessageData
	overlay_veneer SceneEventRuntime_GetActorData
	overlay_veneer SceneEventRuntime_GetEffectData
	overlay_veneer SceneEventRuntime_ReturnZero
