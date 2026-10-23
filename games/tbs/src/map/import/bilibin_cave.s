@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.WaitFrames, 1
	.set import.ObjectDispatch_SelectAnimation, 1
	.set import.Object_SetMoveTarget, 1
	.set import.Object_WaitForMove, 1
	.set import.MapGrid_CopyMetatileIndicesRect, 1
	.set import.MapGrid_CopyCellAttributeRect, 1
	.set import.MapGrid_CheckCollision, 1
	.set import.ObjectDispatch_SetSingleChildFlags, 1
	.set import.GameFlag_Test, 1
	.set import.Engine_GameFlagSet, 1
	.set import.GameFlag_ClearBit, 1
	.set import.FieldScript_WaitFrames, 1
	.set import.FieldScript_Start, 1
	.set import.FieldScript_End, 1
	.set import.ObjectEvent_GetById, 1
	.set import.Engine_ObjectEventSetPosition, 1
	.set import.Engine_ObjectEventSetAnimation, 1
	.set import.ObjectMotion_SetAnimationAndWait, 1
	.set import.ObjectGroup_ConfigureChildValue, 1
	.set import.ObjectMotion_SetActionVariant, 1
	.set import.Sound_PlaySong, 1
	.include "games/tbs/src/system/overlay.inc"
