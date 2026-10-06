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
