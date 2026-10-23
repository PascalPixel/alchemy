@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.__divsi3, 1
	.set import.WaitFrames, 1
	.set import.Trig_Sin, 1
	.set import.Trig_Cos, 1
	.set import.ObjectDispatch_SelectAnimation, 1
	.set import.ObjectDispatch_Initialize, 1
	.set import.Object_CreateField, 1
	.set import.MapGrid_CopyCellAttributeRect, 1
	.set import.ObjectDispatch_SetSingleChildFlags, 1
	.set import.ObjectDispatch_SetChildBlendMode, 1
	.set import.MapGrid_SetScreenShake, 1
	.set import.MapGrid_WaitForShakeToSettle, 1
	.set import.GameFlag_Get, 1
	.set import.GameFlag_Set, 1
	.set import.GameFlag_Clear, 1
	.set import.ObjectEvent_GetById, 1
	.set import.ObjectMotion_SetSpeedParameters, 1
	.set import.ObjectMotion_ResetAndSetPosition, 1
	.set import.ObjectMotion_CommitCurrentPositionAndActivate, 1
	.set import.Engine_ObjectEventSetPosition, 1
	.set import.Engine_ObjectEventSetAnimation, 1
	.set import.ObjectGroup_SetChildValue, 1
	.set import.Sound_PlaySong, 1
	.include "games/tbs/src/system/overlay.inc"
