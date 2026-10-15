@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.__divsi3, 1
	.set import.Object_SetMode, 1
	.set import.Engine_ObjectSetScript, 1
	.set import.Engine_ObjectCreate, 1
	.set import.Engine_ActorSetSpriteFlags, 1
	.set import.Engine_ObjectSetBlendMode, 1
	.set import.Engine_GameFlagIsSet, 1
	.set import.Engine_GameFlagClear, 1
	.set import.Engine_EventWait, 1
	.set import.Engine_EventBegin, 1
	.set import.Engine_EventEnd, 1
	.set import.SceneTable_Prepare, 1
	.set import.Object_GetById, 1
	.set import.Engine_ActorSetAnimation, 1
	.set import.Engine_ActorFaceActor, 1
	.set import.ObjectGroup_SetChildValue, 1
	.set import.Engine_EventSetMessage, 1
	.set import.Engine_EventShowMessage, 1
	.set import.Engine_EventShowMessageAndWait, 1
	.set import.Engine_EventAskYesNo, 1
	.set import.Engine_ShopOpen, 1
	.set import.Engine_InnOpen, 1
	.include "games/tbs/src/system/overlay.inc"
