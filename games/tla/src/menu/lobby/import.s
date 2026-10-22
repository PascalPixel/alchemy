.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global __modsi3
	.thumb_func
__modsi3:
	overlay_veneer IwramSignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global Scheduler_RemoveCallbackFar
	.thumb_func
Scheduler_RemoveCallbackFar:
	overlay_veneer Func_080000d8
	.global Runtime_AllocateBlock
	.thumb_func
Runtime_AllocateBlock:
	overlay_veneer Runtime_AllocateBlockFar
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	overlay_veneer Runtime_ReleaseHeapBlockFar
	.global Runtime_BumpAllocateAlternatePool
	.thumb_func
Runtime_BumpAllocateAlternatePool:
	overlay_veneer Runtime_BumpAllocateAlternatePoolFar
	.global MemFree
	.thumb_func
MemFree:
	overlay_veneer Sys_FreeFar
	.global Func_02001914
	.thumb_func
Func_02001914:
	overlay_veneer SerialRuntime_InitializeFar
	.global Func_0200191c
	.thumb_func
Func_0200191c:
	overlay_veneer SerialRuntime_RemoveIrqHandlersFar
	.global Sound_LoadPresetParameters
	.thumb_func
Sound_LoadPresetParameters:
	overlay_veneer Sound_LoadPresetParametersFar
	.global Func_0200192c
	.thumb_func
Func_0200192c:
	overlay_veneer Func_08000380
	.global SerialRuntime_BeginReceive
	.thumb_func
SerialRuntime_BeginReceive:
	overlay_veneer Party_CheckFar
	.global Func_0200193c
	.thumb_func
Func_0200193c:
	overlay_veneer SerialRuntime_GetActiveTransfersFar
	.global Func_02001944
	.thumb_func
Func_02001944:
	overlay_veneer Scheduler_SetTaskFlagsFar
	.global Unit_Get
	.thumb_func
Unit_Get:
	overlay_veneer Unit_GetFar
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	overlay_veneer GameFlag_ClearBitFar
	.global GameFlag_GetByte
	.thumb_func
GameFlag_GetByte:
	overlay_veneer GameFlag_GetByteFar
	.global GameFlag_SetByte
	.thumb_func
GameFlag_SetByte:
	overlay_veneer GameFlag_SetByteFar
	.global Func_0200197c
	.thumb_func
Func_0200197c:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02001984
	.thumb_func
Func_02001984:
	overlay_veneer Map_CopyMetatileIndicesRectFar
	.global Func_0200198c
	.thumb_func
Func_0200198c:
	.ifdef TLA_EDITION_DE
	overlay_veneer MapLayer_ClearField22Far
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer MapLayer_ClearField22Far
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer MapLayer_ClearField22Far
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer MapLayer_ClearField22Far
	.else
	overlay_veneer Func_08020198
	.endif
	.endif
	.endif
	.endif
	.global Func_02001994
	.thumb_func
Func_02001994:
	.ifdef TLA_EDITION_DE
	overlay_veneer MapLayer_SetField22Far
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer MapLayer_SetField22Far
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer MapLayer_SetField22Far
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer MapLayer_SetField22Far
	.else
	overlay_veneer Func_080201a0
	.endif
	.endif
	.endif
	.endif
	.global Func_0200199c
	.thumb_func
Func_0200199c:
	.ifdef TLA_EDITION_DE
	overlay_veneer DisplayBlend_StartScriptFar
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer DisplayBlend_StartScriptFar
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer DisplayBlend_StartScriptFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer DisplayBlend_StartScriptFar
	.else
	overlay_veneer Map_CopyCellAttributeRectFar
	.endif
	.endif
	.endif
	.endif
	.global UiWindow_Create
	.thumb_func
UiWindow_Create:
	overlay_veneer UiWindow_CreateFar
	.global UiWork_Finalize
	.thumb_func
UiWork_Finalize:
	overlay_veneer UiWork_FinalizeFar
	.global Ui_AdjustValueWithoutLimit
	.thumb_func
Ui_AdjustValueWithoutLimit:
	overlay_veneer Ui_AdjustValueWithoutLimitFar
	.global UiText_OpenMessageWindow
	.thumb_func
UiText_OpenMessageWindow:
	overlay_veneer UiText_OpenMessageWindowFar
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global RenderOutput_PrepareForRedraw
	.thumb_func
RenderOutput_PrepareForRedraw:
	overlay_veneer RenderOutput_PrepareForRedrawFar
	.global UiText_DrawNumber
	.thumb_func
UiText_DrawNumber:
	overlay_veneer UiText_DrawNumberFar
	.global UiWork_ClearValueNameTables
	.thumb_func
UiWork_ClearValueNameTables:
	overlay_veneer UiWork_ClearValueNameTablesFar
	.global Func_020019e4
	.thumb_func
Func_020019e4:
	overlay_veneer UiText_DrawQuantity
	.global Func_020019ec
	.thumb_func
Func_020019ec:
	overlay_veneer Func_080382e0
	.global Djinn_GetRecoveryTable
	.thumb_func
Djinn_GetRecoveryTable:
	overlay_veneer Djinn_GetRecoveryTableFar
	.global Party_CountActiveOwners
	.thumb_func
Party_CountActiveOwners:
	overlay_veneer Party_CountActiveOwnersFar
	.global Func_02001a04
	.thumb_func
Func_02001a04:
	overlay_veneer Func_080ad208
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02001a14
	.thumb_func
Func_02001a14:
	overlay_veneer EventRuntime_BeginFar
	.global Func_02001a1c
	.thumb_func
Func_02001a1c:
	overlay_veneer EventRuntime_EndFar
	.global Event_AskYesNoWithActorAnimation
	.thumb_func
Event_AskYesNoWithActorAnimation:
	overlay_veneer Event_AskYesNoWithActorAnimationFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global ObjectMotion_SetVariantCallback
	.thumb_func
ObjectMotion_SetVariantCallback:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Func_02001a64
	.thumb_func
Func_02001a64:
	overlay_veneer EventRuntime_SetMessageFar
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_02001a74
	.thumb_func
Func_02001a74:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global Func_02001a7c
	.thumb_func
Func_02001a7c:
	overlay_veneer Func_080c8210
	.global Func_02001a84
	.thumb_func
Func_02001a84:
	overlay_veneer Event_SetDestinationFar
	.global Func_02001a8c
	.thumb_func
Func_02001a8c:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Party_SetFields1eeAnd1f0
	.thumb_func
Party_SetFields1eeAnd1f0:
	overlay_veneer Party_SetFields1eeAnd1f0Far
	.global Party_SetFields1f2And1f4
	.thumb_func
Party_SetFields1f2And1f4:
	overlay_veneer Party_SetFields1f2And1f4Far
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattlePalette_StopBlendAndReleaseBufferFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattlePalette_StopBlendAndReleaseBufferFar
	.else
	overlay_veneer Event_StartScreenTransitionFar
	.endif
	.endif
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	.ifdef TLA_EDITION_ES
	overlay_veneer Event_DelayEffectFramesFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Event_DelayEffectFramesFar
	.else
	overlay_veneer Event_WaitTransitionFramesFar
	.endif
	.endif
	.global Func_02001ab4
	.thumb_func
Func_02001ab4:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.endif
	.endif
	.global Func_02001abc
	.thumb_func
Func_02001abc:
	overlay_veneer Func_08108010
	.global DjinnRecovery_TickTwenty
	.thumb_func
DjinnRecovery_TickTwenty:
	overlay_veneer DjinnRecovery_TickTwentyFar
	.global DebugParty_LoadPreset
	.thumb_func
DebugParty_LoadPreset:
	overlay_veneer DebugParty_LoadPresetFar
	.global Func_02001ad4
	.thumb_func
Func_02001ad4:
	overlay_veneer Audio_PlayCue
	.global Func_02001adc
	.thumb_func
Func_02001adc:
	overlay_veneer SongFlag_TestFar
