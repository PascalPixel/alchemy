@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	import Engine_GameFlagClear, GameFlag_ClearBitFar
	import Engine_EventWait, Battle_WaitMode0Far
	import Engine_EventBegin, Battle_ResetFar
	import Engine_EventEnd, BattleFx_FinishActionFar
	import Object_GetById, Object_GetByIdFar
	import Engine_ActorSetAnimationAndWait, Motion_SetModeAndWaitAnimationFar
	import Engine_ActorStartRepeatedMotion, ObjectMotion_SetVariantCallbackFar
	import Engine_ActorRunRepeatedMotion, Motion_SetVarCbAndRefreshFar
	import Engine_ActorFaceActor, ObjectMotion_SetAngleTowardFar
	import Engine_ActorFaceEachOther, Object_LinkPairFar
	import Engine_EventSetMessage, Event_SetMessageIdFar
	import Engine_EventShowMessage, BattleEv_RunWaitFar
	import Engine_EventRequestExit, Event_SetExitRequestFar
	import Engine_AudioPlayCue, AudioCommand_PlayFar
