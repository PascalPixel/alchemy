@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
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
	.set import.Object_SelectAnimation, 1
	.set import.Object_Initialize, 1
	.set import.Object_CreateField, 1
	.set import.Object_Release, 1
	.set import.MapGrid_CenterOnFollowedPoint, 1
	.set import.Object_WaitForValue16, 1
	.set import.Object_SetMoveTarget, 1
	.set import.Object_WaitForMove, 1
	.set import.MapGrid_PlayMetatileCopySequence, 1
	.set import.MapGrid_CopyMetatileIndicesRect, 1
	.set import.BlendScript_Suspend, 1
	.set import.MapGrid_CopyMetatileCellsRect, 1
	.set import.MapGrid_CopyCellAttributeRect, 1
	.set import.MapGrid_CheckCollision, 1
	.set import.Object_SetSpriteFlags, 1
	.set import.Object_SetSpriteBlendMode, 1
	.set import.MapGrid_SetScreenShake, 1
	.set import.MapGrid_WaitForShakeToSettle, 1
	.set import.MapGrid_SetMetatileBehaviorAt, 1
	.set import.FieldMessage_Show, 1
	.set import.GameFlag_Get, 1
	.set import.GameFlag_Set, 1
	.set import.GameFlag_Clear, 1
	.set import.FieldScript_WaitFrames, 1
	.set import.FieldScript_Start, 1
	.set import.FieldScript_End, 1
	.set import.Scene_AssignViewFlags, 1
	.set import.ObjectEvent_GetById, 1
	.set import.ObjectMotion_SetSpeedParameters, 1
	.set import.ObjectMotion_EnableActionAndSetCallback, 1
	.set import.ObjectEvent_RefreshSelectorById, 1
	.set import.ObjectMotion_ResetAndSetPosition, 1
	.set import.ObjectMotion_SetPositionAndCommit, 1
	.set import.ObjectMotion_SetPositionAndReset, 1
	.set import.ObjectMotion_OffsetPositionAndResetMotion, 1
	.set import.ObjectMotion_CommitCurrentPositionAndActivate, 1
	.set import.Import_ObjectEventSetPosition, 1
	.set import.Import_ObjectEventSetAnimation, 1
	.set import.ObjectEvent_SetActionById, 1
	.set import.ObjectMotion_SetAnimationAndWait, 1
	.set import.ObjectMotion_SetVarCbAndRefresh, 1
	.set import.ObjectEvent_JumpDownLedge, 1
	.set import.ObjectMotion_SetAngleToward, 1
	.set import.ObjectEvent_LinkPair, 1
	.set import.ObjectEvent_ConfigureSpriteColor, 1
	.set import.ObjectEvent_SetSpriteColor, 1
	.set import.FieldScript_SetMessageId, 1
	.set import.FieldScript_ShowMessageAndWait, 1
	.set import.FieldScript_ShowMessageThenWait, 1
	.set import.ObjectMotion_ArmCallback, 1
	.set import.ObjectMotion_SetActionVariant, 1
	.set import.FieldEffect_SpawnLinked, 1
	.set import.FieldEffect_ConfigureLinkedObject, 1
	.set import.FieldCamera_SetSpeedParameters, 1
	.set import.FieldCamera_MoveWithinMapBounds, 1
	.set import.FieldCamera_WaitForArrival, 1
	.set import.ObjectEvent_PlaceCurrentWithinCameraBounds, 1
	.set import.FieldCamera_GetScriptObject, 1
	.set import.Import_ScriptRequestWarp, 1
	.set import.PaletteFade_SetTarget, 1
	.set import.PaletteFade_SetSource, 1
	.set import.PaletteFade_StartInterpolation, 1
	.set import.FieldScript_StartScreenTransition, 1
	.set import.FieldScript_FinishScreenTransition, 1
	.set import.FieldScript_WaitTransitionFrames, 1
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
	.set import.ObjectMotion_CommitPositionAndActivate, 1
	.set import.Scene_QueueMusic, 1
	.set import.Scene_PlayQueuedMusic, 1
	.set import.Sound_PlaySong, 1
	.include "games/tbs/src/system/overlay.inc"
