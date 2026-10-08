.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_WorkSetValuesIfNonNegative
	.thumb_func
Engine_WorkSetValuesIfNonNegative:
	overlay_veneer Runtime_SetWorkTripleIfNonNegativeFar
	.global Engine_GameFlagIsSet
	.thumb_func
Engine_GameFlagIsSet:
	overlay_veneer GameFlag_TestFar
	.global InitializeSceneRecordBuffer
	.thumb_func
InitializeSceneRecordBuffer:
	overlay_veneer FieldEffect_InitSparklesFar
	.global QueueSceneSound
	.thumb_func
QueueSceneSound:
	overlay_veneer BattleFx_SetQueuedSoundAndPlayFar
