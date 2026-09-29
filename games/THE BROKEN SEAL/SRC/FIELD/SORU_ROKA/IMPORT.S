@ Import veneers after the SORU_ROKA overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — SORU_ROKA overlay import veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global SetCameraZoom
	.thumb_func
SetCameraZoom:
	overlay_veneer Runtime_SetWorkTripleIfNonNegativeFar
	.global TestSceneFlag
	.thumb_func
TestSceneFlag:
	overlay_veneer GameFlag_TestFar
	.global InitializeSceneRecordBuffer
	.thumb_func
InitializeSceneRecordBuffer:
	overlay_veneer FieldEffect_InitSparklesFar
	.global QueueSceneSound
	.thumb_func
QueueSceneSound:
	overlay_veneer BattleFx_SetQueuedSoundAndPlayFar
