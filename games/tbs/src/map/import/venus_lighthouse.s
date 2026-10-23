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
	.set import.ObjectEvent_SetMode, 1
	.set import.Engine_ObjectSetScript, 1
	.set import.Engine_ObjectCreate, 1
	.set import.Engine_ObjectDispatchRelease, 1
	.set import.Engine_MapGridRedraw, 1
	.set import.ObjectDispatch_WaitForValue16, 1
	.set import.Engine_ObjectSetPosition, 1
	.set import.FieldObject_WaitForMove, 1
	.set import.Engine_MapGridAnimateCells, 1
	.set import.Engine_MapGridCopyCellsTo, 1
	.set import.DisplayBlend_SuspendScript, 1
	.set import.Engine_MapGridCopyCells, 1
	.set import.Engine_MapGridCopyCellAttributes, 1
	.set import.Object_CheckMovementCollision, 1
	.set import.Engine_ObjectEventSetSpriteFlags, 1
	.set import.Engine_ObjectSetBlendMode, 1
	.set import.Engine_WorkSetValuesIfNonNegative, 1
	.set import.Engine_MapGridRenderWaitForValues, 1
	.set import.MapGrid_SetMetatileBehaviorAt, 1
	.set import.Engine_MessageShowCentered, 1
	.set import.Engine_GameFlagIsSet, 1
	.set import.Engine_GameFlagSet, 1
	.set import.Engine_GameFlagClear, 1
	.set import.Engine_ScriptWait, 1
	.set import.Engine_ScriptStart, 1
	.set import.Engine_ScriptEnd, 1
	.set import.Map_PrepareObjectEvents, 1
	.set import.ObjectEvent_GetById, 1
	.set import.Engine_ObjectEventSetSpeed, 1
	.set import.Engine_ObjectEventEnableActionCallback, 1
	.set import.ObjectEvent_RefreshSelectorById, 1
	.set import.Engine_ObjectEventSetDestination, 1
	.set import.Engine_ObjectEventMoveToAndWait, 1
	.set import.Engine_ObjectEventWalkToAndWait, 1
	.set import.Engine_ObjectEventSetDestinationOffset, 1
	.set import.Engine_ObjectEventWaitForMove, 1
	.set import.Engine_ObjectEventSetPosition, 1
	.set import.Engine_ObjectEventSetAnimation, 1
	.set import.ObjectEvent_SetActionById, 1
	.set import.Engine_ObjectEventSetAnimationAndWait, 1
	.set import.Engine_ObjectEventRunRepeatedMotion, 1
	.set import.ObjectEvent_JumpDownLedge, 1
	.set import.Engine_ObjectEventFaceObjectEvent, 1
	.set import.Engine_ObjectEventFaceEachOther, 1
	.set import.Engine_ObjectEventSetChildValue, 1
	.set import.ObjectGroup_SetChildValue, 1
	.set import.Engine_ScriptSetMessage, 1
	.set import.Engine_ScriptShowMessage, 1
	.set import.Engine_ScriptShowMessageAndWait, 1
	.set import.Engine_ObjectEventFaceDirection, 1
	.set import.Engine_ObjectEventSetSpritePriority, 1
	.set import.Engine_ObjectEventShowEmote, 1
	.set import.Engine_ObjectEventSetAttachedEffect, 1
	.set import.Engine_CameraSetSpeed, 1
	.set import.Engine_CameraMoveTo, 1
	.set import.Engine_CameraWaitForMove, 1
	.set import.ObjectEvent_PlaceCurrentWithinCameraBounds, 1
	.set import.Camera_GetScriptObject, 1
	.set import.Engine_ScriptRequestWarp, 1
	.set import.Engine_ColorBufferApplyTarget, 1
	.set import.Engine_ColorBufferApplySource, 1
	.set import.Engine_ColorBufferInterpolate, 1
	.set import.Engine_ScriptOpenScreen, 1
	.set import.Engine_ScriptCloseScreen, 1
	.set import.Engine_ScriptWaitForScreen, 1
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
	.set import.Engine_ObjectEventWalkByAndWait, 1
	.set import.FieldEffect_SetQueuedSoundAndPlay, 1
	.set import.FieldEffect_PlayQueuedSound, 1
	.set import.Engine_AudioPlayCue, 1
	.include "games/tbs/src/system/overlay.inc"
