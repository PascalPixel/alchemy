@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	import Engine_ActorSetSpriteFlags, ObjectDispatch_SetSingleChildFlagsFar
	import Engine_GameFlagIsSet, GameFlag_TestFar
	import Engine_GameFlagSet, GameFlag_SetBitFar
	import Engine_GameFlagClear, GameFlag_ClearBitFar
	import Engine_EventBegin, Battle_ResetFar
	import Engine_EventEnd, BattleFx_FinishActionFar
	import Object_GetById, Object_GetByIdFar
	import Engine_EventSetMessage, Event_SetMessageIdFar
	import Engine_EventShowMessage, BattleEv_RunWaitFar
	import Engine_EventAskYesNo, BattleEventRuntime_ProcessActionFar
	import Engine_ShopOpen, Shop_RunFar
	import Engine_SanctumOpen, Shop_ConfirmActFar
	import Engine_InnOpen, Inn_CheckInFar
