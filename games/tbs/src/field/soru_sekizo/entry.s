@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer Scene_RunGuardSequenceB
	overlay_veneer SceneEventRuntime_GetScriptData
	overlay_veneer SceneEventRuntime_GetMessageData
	overlay_veneer SceneEventRuntime_GetActorData
	overlay_veneer SceneEventRuntime_GetEffectData
	overlay_veneer SceneEventRuntime_ReturnZero
