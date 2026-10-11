@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	import Engine_MapAnimateCells, Map_PlayMetatileCopySequenceFar
	import Engine_MapCopyCellAttributes, Map_CopyCellAttributeRectFar
	import Engine_GameFlagIsSet, GameFlag_TestFar
	import Engine_GameFlagSet, GameFlag_SetBitFar
	import Engine_EventWait, Battle_WaitMode0Far
	import Engine_EventChooseYesNo, Inventory_PromptAndSetObjectModeFar
	import Engine_ActorSetAnimation, Object_SetModeByIdFar
	import Engine_EventSetMessage, Event_SetMessageIdFar
	import Engine_EventOpenMessage, UiText_OpenMessageAtObjectFar
	import Engine_EventShowMessage, BattleEv_RunWaitFar
	import Engine_EventRequestExit, Event_SetExitRequestFar
	import Engine_ActorWalkBy, ObjectMotion_OffsetPositionAndResetFar
	import Engine_AudioPlayCue, AudioCommand_PlayFar
