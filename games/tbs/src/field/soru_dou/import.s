@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	import Engine_WorkSetValuesIfNonNegative, Runtime_SetWorkTripleIfNonNegativeFar
	import Engine_GameFlagIsSet, GameFlag_TestFar
	import Engine_GameFlagSet, GameFlag_SetBitFar
	import Resource37c_FinalizeSetup, FieldEffect_InitSparklesFar
	import Resource37c_QueueSoundCue, BattleFx_SetQueuedSoundAndPlayFar
