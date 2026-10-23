@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.GameFlag_ClearBit, 1
	.set import.FieldScript_WaitFrames, 1
	.set import.FieldScript_Start, 1
	.set import.FieldScript_End, 1
	.set import.ObjectEvent_GetById, 1
	.set import.ObjectMotion_SetAnimationAndWait, 1
	.set import.ObjectMotion_SetVariantCallback, 1
	.set import.ObjectMotion_SetVarCbAndRefresh, 1
	.set import.ObjectMotion_SetAngleToward, 1
	.set import.ObjectEvent_LinkPair, 1
	.set import.FieldScript_SetMessageId, 1
	.set import.FieldScript_ShowMessageAndWait, 1
	.set import.Engine_ScriptRequestWarp, 1
	.set import.Sound_PlaySong, 1
	.include "games/tbs/src/system/overlay.inc"
