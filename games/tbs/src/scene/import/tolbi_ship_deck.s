	.set overlay_imports, 1
	.set import.Math_RemainderUnsigned, 1
	.set import.WaitFrames, 1
	.set import.Task_Add, 1
	.set import.Import_RandomNext, 1
	.set import.Trig_Sin, 1
	.set import.Trig_Cos, 1
	.set import.MapGrid_CenterOnFollowedPoint, 1
	.set import.Actor_SetMoveTarget, 1
	.set import.MapGrid_CopyCellPictureIndicesRect, 1
	.set import.MapGrid_CopyCellAttributeRect, 1
	.set import.Actor_SetSpriteFlags, 1
	.set import.MapGrid_SetScreenShake, 1
	.set import.ActorSystem_SuspendProjectionAndShowSprites, 1
	.set import.ActorSystem_ResumeProjectionAndHideLayers, 1
	.set import.FieldMessage_ShowCentered, 1
	.set import.GameFlag_Get, 1
	.set import.GameFlag_Set, 1
	.set import.GameFlag_Clear, 1
	.set import.Event_WaitFrames, 1
	.set import.Event_Start, 1
	.set import.Event_End, 1
	.set import.Event_CallWithLastActiveActorId, 1
	.set import.Event_AskYesNoWithActorAnimation, 1
	.set import.Actor_GetById, 1
	.set import.Actor_ReleaseById, 1
	.set import.Actor_SetSpeedParameters, 1
	.set import.Actor_SetMovementTypeById, 1
	.set import.Actor_RefreshSelectorById, 1
	.set import.Actor_Stop, 1
	.set import.Actor_SetMovementTypeAndWaitById, 1
	.set import.Actor_ResetAndSetPosition, 1
	.set import.Actor_SetPositionAndCommit, 1
	.set import.Actor_ResetAndSetPositionInMode2, 1
	.set import.Actor_SetPositionAndReset, 1
	.set import.Actor_CenterOnColumn, 1
	.ifdef TBS_EDITION_DE
	.set import.Actor_WaitForMoveById, 1
	.endif
	.ifdef TBS_EDITION_ES
	.set import.Actor_WaitForMoveById, 1
	.endif
	.ifdef TBS_EDITION_FR
	.set import.Actor_WaitForMoveById, 1
	.endif
	.ifdef TBS_EDITION_IT
	.set import.Actor_WaitForMoveById, 1
	.endif
	.set import.Import_ActorSetPosition, 1
	.set import.Import_ActorSetAnimation, 1
	.set import.Actor_SetAnimationAndWait, 1
	.set import.Actor_Jump, 1
	.set import.Actor_StartRepeatedMotion, 1
	.set import.Actor_RunRepeatedMotion, 1
	.set import.Actor_SetAngleToward, 1
	.set import.Actor_ConfigureSpriteColor, 1
	.set import.Event_SetMessageId, 1
	.set import.FieldMessage_ShowAtActor, 1
	.set import.Event_ShowMessageAndWait, 1
	.set import.Event_ShowMessageThenWait, 1
	.set import.Event_AskYesNoAndShowResponse, 1
	.set import.Actor_ArmCallback, 1
	.set import.Message_SetVoiceFromActor, 1
	.set import.Actor_SetSpritePriority, 1
	.set import.FieldEffect_SpawnAttached, 1
	.set import.FieldEffect_SetAttached, 1
	.set import.Actor_CopyGraphics, 1
	.set import.FieldCamera_FollowActor, 1
	.set import.FieldCamera_SetSpeedParameters, 1
	.set import.FieldCamera_MoveWithinMapBounds, 1
	.set import.FieldCamera_WaitForArrival, 1
	.set import.Import_EventSetExitRequest, 1
	.set import.Field_RequestBattleFromEncounterTable, 1
	.set import.Event_SetAfterBattleDestination, 1
	.set import.Event_StartScreenTransition, 1
	.set import.Event_FinishScreenTransition, 1
	.set import.Event_WaitTransitionFrames, 1
	.set import.Field_EndBusyAndReveal, 1
	.set import.Sound_Play, 1
	.include "games/tbs/src/cartridge/overlay.inc"
