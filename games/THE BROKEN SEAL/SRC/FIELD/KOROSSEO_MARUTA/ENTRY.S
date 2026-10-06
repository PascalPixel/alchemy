.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer StageSetup_BuildAndDispatch
	overlay_veneer ColossoLogRollingStage_GetScriptData
	overlay_veneer ColossoLogRollingStage_GetActorData
	overlay_veneer ColossoLogRollingStage_GetEffectData
	overlay_veneer ColossoLogRollingStage_GetSceneEventState
	overlay_veneer ColossoLogRollingStage_GetMessageData
