@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.Math_RemainderUnsigned, 1
	.set import.Engine_TaskWait, 1
	.set import.Scheduler_AddOrUpdateCallback, 1
	.set import.Engine_RandomNext, 1
	.set import.Engine_MathSin, 1
	.set import.Engine_MathCos, 1
	.set import.Engine_MapRedraw, 1
	.set import.Engine_ObjectSetPosition, 1
	.set import.Engine_MapCopyCellsTo, 1
	.set import.Engine_MapCopyCellAttributes, 1
	.set import.Engine_ActorSetSpriteFlags, 1
	.set import.Engine_WorkSetValuesIfNonNegative, 1
	.set import.ObjectSystem_SuspendProjectionAndShowSprites, 1
	.set import.ObjectSystem_ResumeProjectionAndHideLayers, 1
	.set import.UiText_ShowCenteredMessage, 1
	.set import.Engine_GameFlagIsSet, 1
	.set import.Engine_GameFlagSet, 1
	.set import.Engine_GameFlagClear, 1
	.set import.Engine_EventWait, 1
	.set import.Engine_EventStart, 1
	.set import.Engine_EventEnd, 1
	.set import.Event_CallWithLastActiveObjectId, 1
	.set import.Engine_EventChooseYesNo, 1
	.set import.Object_GetById, 1
	.set import.Engine_ActorDestroy, 1
	.set import.Engine_ActorSetSpeed, 1
	.set import.Engine_ActorEnableActionCallback, 1
	.set import.Object_RefreshSelectorById, 1
	.set import.Engine_ActorStop, 1
	.set import.Object_SetActionCallbackAndRefreshById, 1
	.set import.Engine_ActorSetDestination, 1
	.set import.Engine_ObjectMotionSetPositionAndCommit, 1
	.set import.Engine_ActorWalkTo, 1
	.set import.Engine_ActorWalkToAndWait, 1
	.set import.ObjectMotion_CenterOnColumn, 1
	.ifdef TBS_EDITION_DE
	.set import.Engine_ActorWaitForMove, 1
	.endif
	.ifdef TBS_EDITION_ES
	.set import.Engine_ActorWaitForMove, 1
	.endif
	.ifdef TBS_EDITION_FR
	.set import.Engine_ActorWaitForMove, 1
	.endif
	.ifdef TBS_EDITION_IT
	.set import.Engine_ActorWaitForMove, 1
	.endif
	.set import.Engine_ActorSetPosition, 1
	.set import.Engine_ActorSetAnimation, 1
	.set import.Engine_ActorSetAnimationAndWait, 1
	.set import.Engine_ActorJump, 1
	.set import.Engine_ActorStartRepeatedMotion, 1
	.set import.Engine_ActorRunRepeatedMotion, 1
	.set import.Engine_ActorFaceActor, 1
	.set import.Engine_ActorSetChildValue, 1
	.set import.Engine_EventSetMessage, 1
	.set import.Engine_EventOpenMessage, 1
	.set import.Engine_EventShowMessage, 1
	.set import.Engine_EventShowMessageAndWait, 1
	.set import.Engine_EventAskYesNo, 1
	.set import.Engine_ActorFaceDirection, 1
	.set import.Ui_SetMessageVoiceFromActor, 1
	.set import.Engine_ActorSetSpritePriority, 1
	.set import.Engine_ActorShowEmote, 1
	.set import.Engine_ActorSetAttachedEffect, 1
	.set import.ObjectVisual_CopyAttributes, 1
	.set import.Engine_CameraFollowActor, 1
	.set import.Engine_CameraSetSpeed, 1
	.set import.Engine_CameraMoveTo, 1
	.set import.Engine_CameraWaitForMove, 1
	.set import.Engine_EventRequestExit, 1
	.set import.Field_RequestBattleFromEncounterTable, 1
	.set import.Event_SetNextDestination, 1
	.set import.Engine_EventOpenScreen, 1
	.set import.Engine_EventCloseScreen, 1
	.set import.Engine_EventWaitForScreen, 1
	.set import.Field_EndBusyAndReveal, 1
	.set import.Engine_AudioPlayCue, 1
	.include "games/tbs/src/system/overlay.inc"
