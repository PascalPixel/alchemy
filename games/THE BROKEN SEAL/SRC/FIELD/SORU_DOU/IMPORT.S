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
	.global Engine_GameFlagSet
	.thumb_func
Engine_GameFlagSet:
	overlay_veneer GameFlag_SetBitFar
	.global Resource37c_FinalizeSetup
	.thumb_func
Resource37c_FinalizeSetup:
	overlay_veneer FieldEffect_InitSparklesFar
	.global Resource37c_QueueSoundCue
	.thumb_func
Resource37c_QueueSoundCue:
	overlay_veneer BattleFx_SetQueuedSoundAndPlayFar
