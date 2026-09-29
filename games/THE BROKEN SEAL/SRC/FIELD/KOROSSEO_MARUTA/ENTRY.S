@ Entry veneers opening the KOROSSEO_MARUTA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — KOROSSEO_MARUTA overlay entry veneers
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
