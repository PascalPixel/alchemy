@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.__divsi3, 1
	.set import.Engine_TaskWait, 1
	.set import.Object_SetMode, 1
	.set import.Engine_ObjectSetPosition, 1
	.set import.FieldObject_WaitForMove, 1
	.set import.Engine_MapCopyCellAttributes, 1
	.set import.Object_CheckMovementCollision, 1
	.set import.Engine_ActorSetSpriteFlags, 1
	.set import.Engine_GameFlagIsSet, 1
	.set import.Engine_GameFlagSet, 1
	.set import.Engine_EventWait, 1
	.set import.Engine_EventBegin, 1
	.set import.Engine_EventEnd, 1
	.set import.Object_GetById, 1
	.set import.Engine_ActorSetSpeed, 1
	.set import.Engine_ActorMoveToAndWait, 1
	.set import.Engine_ActorSetDestinationOffset, 1
	.set import.Engine_ActorWaitForMove, 1
	.set import.Engine_ActorSetAnimation, 1
	.set import.Engine_ColorBufferApplyTarget, 1
	.set import.Engine_ColorBufferApplySource, 1
	.set import.FieldFx_PlayQueuedSound, 1
	.set import.Engine_AudioPlayCue, 1
	.include "games/tbs/src/system/overlay.inc"
