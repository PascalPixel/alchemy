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
