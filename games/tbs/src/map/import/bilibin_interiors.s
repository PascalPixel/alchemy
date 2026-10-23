@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.GameFlag_Get, 1
	.set import.GameFlag_Set, 1
	.set import.FieldScript_WaitFrames, 1
	.set import.FieldScript_Start, 1
	.set import.FieldScript_End, 1
	.set import.Map_AssignViewFlags, 1
	.set import.FieldScript_AskYesNoWithObjectEventAnimation, 1
	.set import.ObjectEvent_GetById, 1
	.set import.ObjectMotion_SetVariantCallback, 1
	.set import.ObjectMotion_SetAngleToward, 1
	.set import.FieldScript_SetMessageId, 1
	.set import.FieldMessage_ShowAtObject, 1
	.set import.FieldScript_ShowMessageAndWait, 1
	.set import.FieldScript_ShowMessageThenWait, 1
	.set import.FieldScript_AskYesNoAndShowResponse, 1
	.set import.ObjectMotion_ArmCallback, 1
	.set import.FieldEffect_ConfigureLinkedObject, 1
	.set import.Shop_Run, 1
	.set import.Inn_CheckIn, 1
	.include "games/tbs/src/system/overlay.inc"
