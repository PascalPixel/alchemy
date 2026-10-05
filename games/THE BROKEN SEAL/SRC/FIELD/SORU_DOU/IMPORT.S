.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Resource37c_SetViewScale
	.thumb_func
Resource37c_SetViewScale:
	overlay_veneer Runtime_SetWorkTripleIfNonNegativeFar
	.global Resource37c_TestSetupGate
	.thumb_func
Resource37c_TestSetupGate:
	overlay_veneer GameFlag_TestFar
	.global Resource37c_SetSetupFlag
	.thumb_func
Resource37c_SetSetupFlag:
	overlay_veneer GameFlag_SetBitFar
	.global Resource37c_FinalizeSetup
	.thumb_func
Resource37c_FinalizeSetup:
	overlay_veneer FieldEffect_InitSparklesFar
	.global Resource37c_QueueSoundCue
	.thumb_func
Resource37c_QueueSoundCue:
	overlay_veneer BattleFx_SetQueuedSoundAndPlayFar
