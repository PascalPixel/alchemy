@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.__divsi3, 1
	.set import.ObjectDispatch_SelectAnimation, 1
	.set import.ObjectDispatch_Initialize, 1
	.set import.Object_CreateField, 1
	.set import.ObjectDispatch_SetSingleChildFlags, 1
	.set import.ObjectDispatch_SetChildBlendMode, 1
	.set import.GameFlag_Test, 1
	.set import.GameFlag_ClearBit, 1
	.set import.FieldScript_WaitFrames, 1
	.set import.FieldScript_Start, 1
	.set import.FieldScript_End, 1
	.set import.Map_AssignViewFlags, 1
	.set import.ObjectEvent_GetById, 1
	.set import.Engine_ObjectEventSetAnimation, 1
	.set import.ObjectMotion_SetAngleToward, 1
	.set import.ObjectGroup_SetChildValue, 1
	.set import.FieldScript_SetMessageId, 1
	.set import.FieldScript_ShowMessageAndWait, 1
	.set import.FieldScript_ShowMessageThenWait, 1
	.set import.FieldScript_AskYesNoAndShowResponse, 1
	.set import.Shop_Run, 1
	.set import.Inn_CheckIn, 1
	.include "games/tbs/src/system/overlay.inc"
