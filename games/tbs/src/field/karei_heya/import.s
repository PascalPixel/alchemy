@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	import Engine_MapAnimateCells, Map_PlayMetatileCopySequenceFar
	import Engine_MapCopyCellsTo, Map_CopyMetatileIndicesRectFar
	import Engine_GameFlagIsSet, GameFlag_TestFar
	import Engine_GameFlagSet, GameFlag_SetBitFar
	import Engine_EventWait, Battle_WaitMode0Far
	import Engine_EventBegin, Battle_ResetFar
	import Engine_EventEnd, BattleFx_FinishActionFar
	import FieldScene_PrepareActors, Scene_AssignViewFlagsFar
	import Object_GetById, Object_GetByIdFar
	import Engine_ActorDestroy, ObjectTable_DestroyByIdFar
	import Engine_ActorSetSpeed, ObjectMotion_SetSpeedParametersFar
	import Engine_ActorCenterAndWalk, ObjectMotion_CenterOnColumnFar
	import Engine_ActorSetAnimation, Object_SetModeByIdFar
	import Engine_ActorSetChildValue, ObjectGroup_ConfigureChildValueFar
	import Engine_EventSetMessage, Event_SetMessageIdFar
	import Engine_EventShowMessage, BattleEv_RunWaitFar
	import Engine_EventAskYesNo, BattleEventRuntime_ProcessActionFar
	import Engine_EventRequestExit, Event_SetExitRequestFar
	import Engine_EventCloseScreen, Event_FinishScreenTransitionFar
	import Engine_EventWaitForScreen, Event_WaitTransitionFramesFar
	import Engine_SanctumOpen, Shop_ConfirmActFar
	import Engine_InnOpen, Inn_CheckInFar
	import Engine_AudioPlayCue, AudioCommand_PlayFar
