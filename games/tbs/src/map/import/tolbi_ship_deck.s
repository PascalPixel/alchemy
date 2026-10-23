@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.set overlay_imports, 1
	.set import.Math_RemainderUnsigned, 1
	.set import.Engine_TaskWait, 1
	.set import.Task_Add, 1
	.set import.Engine_RandomNext, 1
	.set import.Engine_MathSin, 1
	.set import.Engine_MathCos, 1
	.set import.Engine_MapGridRedraw, 1
	.set import.Engine_ObjectSetPosition, 1
	.set import.Engine_MapGridCopyCellsTo, 1
	.set import.Engine_MapGridCopyCellAttributes, 1
	.set import.Engine_ObjectEventSetSpriteFlags, 1
	.set import.Engine_WorkSetValuesIfNonNegative, 1
	.set import.ObjectSystem_SuspendProjectionAndShowSprites, 1
	.set import.ObjectSystem_ResumeProjectionAndHideLayers, 1
	.set import.UiText_ShowCenteredMessage, 1
	.set import.Engine_GameFlagIsSet, 1
	.set import.Engine_GameFlagSet, 1
	.set import.Engine_GameFlagClear, 1
	.set import.Engine_ScriptWait, 1
	.set import.Engine_ScriptStart, 1
	.set import.Engine_ScriptEnd, 1
	.set import.FieldScript_CallWithLastActiveObjectId, 1
	.set import.Engine_ScriptChooseYesNo, 1
	.set import.ObjectEvent_GetById, 1
	.set import.Engine_ObjectEventDestroy, 1
	.set import.Engine_ObjectEventSetSpeed, 1
	.set import.Engine_ObjectEventEnableActionCallback, 1
	.set import.ObjectEvent_RefreshSelectorById, 1
	.set import.Engine_ObjectEventStop, 1
	.set import.ObjectEvent_SetActionCallbackAndRefreshById, 1
	.set import.Engine_ObjectEventSetDestination, 1
	.set import.Engine_ObjectMotionSetPositionAndCommit, 1
	.set import.Engine_ObjectEventWalkTo, 1
	.set import.Engine_ObjectEventWalkToAndWait, 1
	.set import.ObjectMotion_CenterOnColumn, 1
	.ifdef TBS_EDITION_DE
	.set import.Engine_ObjectEventWaitForMove, 1
	.endif
	.ifdef TBS_EDITION_ES
	.set import.Engine_ObjectEventWaitForMove, 1
	.endif
	.ifdef TBS_EDITION_FR
	.set import.Engine_ObjectEventWaitForMove, 1
	.endif
	.ifdef TBS_EDITION_IT
	.set import.Engine_ObjectEventWaitForMove, 1
	.endif
	.set import.Engine_ObjectEventSetPosition, 1
	.set import.Engine_ObjectEventSetAnimation, 1
	.set import.Engine_ObjectEventSetAnimationAndWait, 1
	.set import.Engine_ObjectEventJump, 1
	.set import.Engine_ObjectEventStartRepeatedMotion, 1
	.set import.Engine_ObjectEventRunRepeatedMotion, 1
	.set import.Engine_ObjectEventFaceObjectEvent, 1
	.set import.Engine_ObjectEventSetChildValue, 1
	.set import.Engine_ScriptSetMessage, 1
	.set import.Engine_ScriptOpenMessage, 1
	.set import.Engine_ScriptShowMessage, 1
	.set import.Engine_ScriptShowMessageAndWait, 1
	.set import.Engine_ScriptAskYesNo, 1
	.set import.Engine_ObjectEventFaceDirection, 1
	.set import.Ui_SetMessageVoiceFromObjectEvent, 1
	.set import.Engine_ObjectEventSetSpritePriority, 1
	.set import.Engine_ObjectEventShowEmote, 1
	.set import.Engine_ObjectEventSetAttachedEffect, 1
	.set import.ObjectVisual_CopyAttributes, 1
	.set import.Engine_CameraFollowObjectEvent, 1
	.set import.Engine_CameraSetSpeed, 1
	.set import.Engine_CameraMoveTo, 1
	.set import.Engine_CameraWaitForMove, 1
	.set import.Engine_ScriptRequestWarp, 1
	.set import.Field_RequestBattleFromEncounterTable, 1
	.set import.FieldScript_SetNextDestination, 1
	.set import.Engine_ScriptOpenScreen, 1
	.set import.Engine_ScriptCloseScreen, 1
	.set import.Engine_ScriptWaitForScreen, 1
	.set import.Field_EndBusyAndReveal, 1
	.set import.Engine_AudioPlayCue, 1
	.include "games/tbs/src/system/overlay.inc"
