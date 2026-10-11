@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	import __divsi3, IwramSignedDivide
	import Object_SetMode, ObjectDispatch_ApplyArgumentToChildrenFar
	import Engine_ObjectSetScript, ObjectDispatch_InitializeFar
	import Engine_ObjectCreate, Object_CreateFar
	import Engine_ActorSetSpriteFlags, ObjectDispatch_SetSingleChildFlagsFar
	import Engine_ObjectSetBlendMode, ObjectDispatch_SetChildBlendModeFar
	import Engine_GameFlagIsSet, GameFlag_TestFar
	import Engine_GameFlagClear, GameFlag_ClearBitFar
	import Engine_EventWait, Battle_WaitMode0Far
	import Engine_EventBegin, Battle_ResetFar
	import Engine_EventEnd, BattleFx_FinishActionFar
	import SceneTable_Prepare, Scene_AssignViewFlagsFar
	import Object_GetById, Object_GetByIdFar
	import Engine_ActorSetAnimation, Object_SetModeByIdFar
	import Engine_ActorFaceActor, ObjectMotion_SetAngleTowardFar
	import ObjectGroup_SetChildValue, ObjectGroup_SetChildValueFar
	import Engine_EventSetMessage, Event_SetMessageIdFar
	import Engine_EventShowMessage, BattleEv_RunWaitFar
	import Engine_EventShowMessageAndWait, BattlePres_RunActionThenWaitIfModeZeroFar
	import Engine_EventAskYesNo, BattleEventRuntime_ProcessActionFar
	import Engine_ShopOpen, Shop_RunFar
	import Engine_InnOpen, Inn_CheckInFar
