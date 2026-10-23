@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.__divsi3, 1
	.set import.Engine_TaskWait, 1
	.set import.Trig_Sin, 1
	.set import.Trig_Cos, 1
	.set import.Object_SetMode, 1
	.set import.Engine_ObjectSetScript, 1
	.set import.Engine_ObjectCreate, 1
	.set import.Engine_MapCopyCellAttributes, 1
	.set import.Engine_ActorSetSpriteFlags, 1
	.set import.Engine_ObjectSetBlendMode, 1
	.set import.Engine_WorkSetValuesIfNonNegative, 1
	.set import.Map_WaitForShakeToSettle, 1
	.set import.Engine_GameFlagIsSet, 1
	.set import.Engine_GameFlagSet, 1
	.set import.Engine_GameFlagClear, 1
	.set import.Object_GetById, 1
	.set import.Engine_ActorSetSpeed, 1
	.set import.Engine_ActorSetDestination, 1
	.set import.Engine_ActorWaitForMove, 1
	.set import.Engine_ActorSetPosition, 1
	.set import.Engine_ActorSetAnimation, 1
	.set import.ObjectGroup_SetChildValue, 1
	.set import.Engine_AudioPlayCue, 1
	.include "games/tbs/src/system/overlay.inc"
