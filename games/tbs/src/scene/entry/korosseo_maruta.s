@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer StageSetup_BuildAndDispatch
	overlay_veneer ColossoLogRollingStage_GetScriptData
	overlay_veneer ColossoLogRollingStage_GetActorData
	overlay_veneer ColossoLogRollingStage_GetEffectData
	overlay_veneer ColossoLogRollingStage_GetSceneEventState
	overlay_veneer ColossoLogRollingStage_GetMessageData
