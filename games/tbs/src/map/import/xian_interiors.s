@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.__divsi3, 1
	.set import.ObjectEvent_SetMode, 1
	.set import.Engine_ObjectSetScript, 1
	.set import.Engine_ObjectCreate, 1
	.set import.Engine_ObjectEventSetSpriteFlags, 1
	.set import.Engine_ObjectSetBlendMode, 1
	.set import.Engine_GameFlagIsSet, 1
	.set import.Engine_GameFlagClear, 1
	.set import.Engine_ScriptWait, 1
	.set import.Engine_ScriptStart, 1
	.set import.Engine_ScriptEnd, 1
	.set import.MapTable_Prepare, 1
	.set import.ObjectEvent_GetById, 1
	.set import.Engine_ObjectEventSetAnimation, 1
	.set import.Engine_ObjectEventFaceObjectEvent, 1
	.set import.ObjectGroup_SetChildValue, 1
	.set import.Engine_ScriptSetMessage, 1
	.set import.Engine_ScriptShowMessage, 1
	.set import.Engine_ScriptShowMessageAndWait, 1
	.set import.Engine_ScriptAskYesNo, 1
	.set import.Engine_ShopOpen, 1
	.set import.Engine_InnOpen, 1
	.include "games/tbs/src/system/overlay.inc"
