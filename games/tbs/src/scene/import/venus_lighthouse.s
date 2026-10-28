	.set overlay_imports, 1
	.set import.__divsi3, 1
	.set import.WaitFrames, 1
	.set import.Task_Add, 1
	.set import.Import_RandomNext, 1
	.set import.Trig_Sin, 1
	.set import.Trig_Cos, 1
	.ifndef TBS_EDITION_JA
	.set import.Vector_AddPolarOffset, 1
	.endif
	.set import.Actor_SelectAnimation, 1
	.set import.Actor_StartScript, 1
	.set import.Actor_Create, 1
	.set import.Actor_Release, 1
	.set import.MapGrid_CenterOnFollowedPoint, 1
	.set import.Actor_WaitForScriptEnd, 1
	.set import.Actor_SetMoveTarget, 1
	.set import.Actor_WaitForMove, 1
	.set import.MapGrid_PlayMapEdit, 1
	.set import.MapGrid_CopyCellPictureIndicesRect, 1
	.set import.BlendScript_Suspend, 1
	.set import.MapGrid_CopyCellPictureCellsRect, 1
	.set import.MapGrid_CopyCellAttributeRect, 1
	.set import.MapGrid_CheckCollision, 1
	.set import.Actor_SetSpriteFlags, 1
	.set import.Actor_SetBlendMode, 1
	.set import.MapGrid_SetScreenShake, 1
	.set import.MapGrid_WaitForShakeToSettle, 1
	.set import.MapGrid_SetCellBehaviorAt, 1
	.set import.FieldMessage_Show, 1
	.set import.GameFlag_Get, 1
	.set import.GameFlag_Set, 1
	.set import.GameFlag_Clear, 1
	.set import.Event_WaitFrames, 1
	.set import.Event_Start, 1
	.set import.Event_End, 1
	.set import.Scene_AssignViewFlags, 1
	.set import.Actor_GetById, 1
	.set import.Actor_SetSpeedParameters, 1
	.set import.Actor_SetMovementTypeById, 1
	.set import.Actor_RefreshSelectorById, 1
	.set import.Actor_ResetAndSetPosition, 1
	.set import.Actor_SetPositionAndCommit, 1
	.set import.Actor_SetPositionAndReset, 1
	.set import.Actor_OffsetPositionAndResetMotion, 1
	.set import.Actor_WaitForMoveById, 1
	.set import.Import_ActorSetPosition, 1
	.set import.Import_ActorSetAnimation, 1
	.set import.Actor_SetAnimationSpeedById, 1
	.set import.Actor_SetAnimationAndWait, 1
	.set import.Actor_RunRepeatedMotion, 1
	.set import.Actor_JumpDownLedge, 1
	.set import.Actor_SetAngleToward, 1
	.set import.Actor_TurnToFaceEachOther, 1
	.set import.Actor_ConfigureSpriteColor, 1
	.set import.Actor_SetSpriteColor, 1
	.set import.Event_SetMessageId, 1
	.set import.Event_ShowMessageAndWait, 1
	.set import.Event_ShowMessageThenWait, 1
	.set import.Actor_ArmCallback, 1
	.set import.Actor_SetSpritePriority, 1
	.set import.FieldEffect_SpawnAttached, 1
	.set import.FieldEffect_SetAttached, 1
	.set import.FieldCamera_SetSpeedParameters, 1
	.set import.FieldCamera_MoveWithinMapBounds, 1
	.set import.FieldCamera_WaitForArrival, 1
	.set import.Actor_PlaceCurrentWithinCameraBounds, 1
	.set import.FieldCamera_GetActor, 1
	.set import.Import_EventSetExitRequest, 1
	.set import.PaletteFade_SetTarget, 1
	.set import.PaletteFade_SetSource, 1
	.set import.PaletteFade_StartInterpolation, 1
	.set import.Event_StartScreenTransition, 1
	.set import.Event_FinishScreenTransition, 1
	.set import.Event_WaitTransitionFrames, 1
	.set import.FieldEffect_StartFadeOverlay, 1
	.ifdef TBS_EDITION_ES
	.set import.Field_EndBusyAndReveal, 1
	.endif
	.ifdef TBS_EDITION_FR
	.set import.Field_EndBusyAndReveal, 1
	.endif
	.ifdef TBS_EDITION_IT
	.set import.Field_EndBusyAndReveal, 1
	.endif
	.set import.Actor_CommitPositionAndActivate, 1
	.set import.Scene_QueueMusic, 1
	.set import.Scene_PlayQueuedMusic, 1
	.set import.Sound_Play, 1
	.include "games/tbs/src/cartridge/overlay.inc"
