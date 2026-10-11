@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	import Engine_GameFlagIsSet, GameFlag_TestFar
	import Engine_GameFlagSet, GameFlag_SetBitFar
	import Engine_EventWait, Battle_WaitMode0Far
	import Engine_EventBegin, Battle_ResetFar
	import Engine_EventEnd, BattleFx_FinishActionFar
	import ScenePlacement_ClipToView, Scene_AssignViewFlagsFar
	import Engine_EventChooseYesNo, Inventory_PromptAndSetObjectModeFar
	import Object_GetById, Object_GetByIdFar
	import Engine_ActorStartRepeatedMotion, ObjectMotion_SetVariantCallbackFar
	import Engine_ActorFaceActor, ObjectMotion_SetAngleTowardFar
	import Engine_EventSetMessage, Event_SetMessageIdFar
	import Engine_EventOpenMessage, UiText_OpenMessageAtObjectFar
	import Engine_EventShowMessage, BattleEv_RunWaitFar
	import Engine_EventShowMessageAndWait, BattlePres_RunActionThenWaitIfModeZeroFar
	import Engine_EventAskYesNo, BattleEventRuntime_ProcessActionFar
	import Engine_ActorFaceDirection, ObjectMotion_ArmCallbackFar
	import Engine_ActorSetAttachedEffect, BattleFx_ConfigureLinkedObjectFar
	import Engine_ShopOpen, Shop_RunFar
	import Engine_InnOpen, Inn_CheckInFar
