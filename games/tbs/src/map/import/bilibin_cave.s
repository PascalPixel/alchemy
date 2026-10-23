@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.Engine_TaskWait, 1
	.set import.ObjectEvent_SetMode, 1
	.set import.Engine_ObjectSetPosition, 1
	.set import.FieldObject_WaitForMove, 1
	.set import.Engine_MapGridCopyCellsTo, 1
	.set import.Engine_MapGridCopyCellAttributes, 1
	.set import.Object_CheckMovementCollision, 1
	.set import.Engine_ObjectEventSetSpriteFlags, 1
	.set import.Engine_GameFlagIsSet, 1
	.set import.Engine_GameFlagSet, 1
	.set import.Engine_GameFlagClear, 1
	.set import.Engine_ScriptWait, 1
	.set import.Engine_ScriptStart, 1
	.set import.Engine_ScriptEnd, 1
	.set import.ObjectEvent_GetById, 1
	.set import.Engine_ObjectEventSetPosition, 1
	.set import.Engine_ObjectEventSetAnimation, 1
	.set import.Engine_ObjectEventSetAnimationAndWait, 1
	.set import.Engine_ObjectEventSetChildValue, 1
	.set import.Engine_ObjectEventSetSpritePriority, 1
	.set import.Engine_AudioPlayCue, 1
	.include "games/tbs/src/system/overlay.inc"
