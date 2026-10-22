@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.__divsi3, 1
	.set import.Engine_TaskWait, 1
	.set import.Scheduler_AddOrUpdateCallback, 1
	.set import.Engine_RandomNext, 1
	.set import.Engine_MathSin, 1
	.set import.Engine_MathCos, 1
	.ifndef TBS_EDITION_JA
	.set import.Vector_AddPolarOffset, 1
	.endif
	.set import.Object_SetMode, 1
	.set import.Engine_ObjectSetScript, 1
	.set import.Engine_ObjectCreate, 1
	.set import.Engine_ObjectDispatchRelease, 1
	.set import.Engine_MapRedraw, 1
	.set import.ObjectDispatch_WaitForValue16, 1
	.set import.Engine_ObjectSetPosition, 1
	.set import.FieldObject_WaitForMove, 1
	.set import.Engine_MapAnimateCells, 1
	.set import.Engine_MapCopyCellsTo, 1
	.set import.DisplayBlend_SuspendScript, 1
	.set import.Engine_MapCopyCells, 1
	.set import.Engine_MapCopyCellAttributes, 1
	.set import.Object_CheckMovementCollision, 1
	.set import.Engine_ActorSetSpriteFlags, 1
	.set import.Engine_ObjectSetBlendMode, 1
	.set import.Engine_WorkSetValuesIfNonNegative, 1
	.set import.Engine_MapRenderWaitForValues, 1
	.set import.SetMapCellCollision, 1
	.set import.Engine_MessageShowCentered, 1
	.set import.Engine_GameFlagIsSet, 1
	.set import.Engine_GameFlagSet, 1
	.set import.Engine_GameFlagClear, 1
	.set import.Engine_EventWait, 1
	.set import.Engine_EventBegin, 1
	.set import.Engine_EventEnd, 1
	.set import.FieldScene_PrepareActors, 1
	.set import.Object_GetById, 1
	.set import.Engine_ActorSetSpeed, 1
	.set import.Engine_ActorEnableActionCallback, 1
	.set import.Object_RefreshSelectorById, 1
	.set import.Engine_ActorSetDestination, 1
	.set import.Engine_ActorMoveToAndWait, 1
	.set import.Engine_ActorWalkToAndWait, 1
	.set import.Engine_ActorSetDestinationOffset, 1
	.set import.Engine_ActorWaitForMove, 1
	.set import.Engine_ActorSetPosition, 1
	.set import.Engine_ActorSetAnimation, 1
	.set import.Object_SetActionById, 1
	.set import.Engine_ActorSetAnimationAndWait, 1
	.set import.Engine_ActorRunRepeatedMotion, 1
	.set import.Actor_JumpDownLedge, 1
	.set import.Engine_ActorFaceActor, 1
	.set import.Engine_ActorFaceEachOther, 1
	.set import.Engine_ActorSetChildValue, 1
	.set import.ObjectGroup_SetChildValue, 1
	.set import.Engine_EventSetMessage, 1
	.set import.Engine_EventShowMessage, 1
	.set import.Engine_EventShowMessageAndWait, 1
	.set import.Engine_ActorFaceDirection, 1
	.set import.Engine_ActorSetSpritePriority, 1
	.set import.Engine_ActorShowEmote, 1
	.set import.Engine_ActorSetAttachedEffect, 1
	.set import.Engine_CameraSetSpeed, 1
	.set import.Engine_CameraMoveTo, 1
	.set import.Engine_CameraWaitForMove, 1
	.set import.Object_PlaceCurrentWithinCameraBounds, 1
	.set import.Camera_GetEventObject, 1
	.set import.Engine_EventRequestExit, 1
	.set import.Engine_ColorBufferApplyTarget, 1
	.set import.Engine_ColorBufferApplySource, 1
	.set import.Engine_ColorBufferInterpolate, 1
	.set import.Engine_EventOpenScreen, 1
	.set import.Engine_EventCloseScreen, 1
	.set import.Engine_EventWaitForScreen, 1
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
	.set import.Engine_ActorWalkByAndWait, 1
	.set import.FieldEffect_SetQueuedSoundAndPlay, 1
	.set import.FieldEffect_PlayQueuedSound, 1
	.set import.Engine_AudioPlayCue, 1
	.include "games/tbs/src/system/overlay.inc"
