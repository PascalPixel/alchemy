.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
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
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.global Math_Cosine
	.thumb_func
Math_Cosine:
	overlay_veneer Math_CosineFar
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global MemAlloc
	.thumb_func
MemAlloc:
	overlay_veneer Runtime_BumpAllocateFar
	.global MemFree
	.thumb_func
MemFree:
	overlay_veneer Sys_FreeFar
	.global Func_02003c7c
	.thumb_func
Func_02003c7c:
	overlay_veneer Resource_DecodeType01Far
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02003c94
	.thumb_func
Func_02003c94:
	overlay_veneer Runtime_PushSlotEntryFar
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
	.global Animation_ApplyChildArgument
	.thumb_func
Animation_ApplyChildArgument:
	overlay_veneer Animation_ApplyChildArgumentFar
	.global ResourceMetadata_Register
	.thumb_func
ResourceMetadata_Register:
	overlay_veneer ResourceMetadata_RegisterFar
	.global Func_02003cc4
	.thumb_func
Func_02003cc4:
	overlay_veneer Object_SetMode
	.global Func_02003ccc
	.thumb_func
Func_02003ccc:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Engine_ObjectCreate
	.thumb_func
Engine_ObjectCreate:
	overlay_veneer Func_080200c0
	.global Func_02003cdc
	.thumb_func
Func_02003cdc:
	overlay_veneer Map_ApplyWorkOriginAndSpanFar
	.global Func_02003ce4
	.thumb_func
Func_02003ce4:
	overlay_veneer Object_SetPosition
	.global Func_02003cec
	.thumb_func
Func_02003cec:
	overlay_veneer Object_CommitPosition
	.global Func_02003cf4
	.thumb_func
Func_02003cf4:
	overlay_veneer Map_CopyMetatileIndicesRectFar
	.global Func_02003cfc
	.thumb_func
Func_02003cfc:
	.ifdef TLA_EDITION_DE
	overlay_veneer Map_DisableBlendScriptFar
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Map_DisableBlendScriptFar
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Map_DisableBlendScriptFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Map_DisableBlendScriptFar
	.else
	overlay_veneer DisplayBlend_DisableRunScriptFar
	.endif
	.endif
	.endif
	.endif
	.global Func_02003d04
	.thumb_func
Func_02003d04:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_08020180
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_08020180
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_08020180
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_08020180
	.else
	overlay_veneer DisplayBlend_EnableRunScriptFar
	.endif
	.endif
	.endif
	.endif
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_08020190
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_08020190
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_08020190
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_08020190
	.else
	overlay_veneer Map_GetTerrainHeightFar
	.endif
	.endif
	.endif
	.endif
	.global Func_02003d14
	.thumb_func
Func_02003d14:
	.ifdef TLA_EDITION_DE
	overlay_veneer DisplayBlend_EnableRunScriptFar
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer DisplayBlend_EnableRunScriptFar
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer DisplayBlend_EnableRunScriptFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer DisplayBlend_EnableRunScriptFar
	.else
	overlay_veneer Map_CopyCellsRectFar
	.endif
	.endif
	.endif
	.endif
	.global Func_02003d1c
	.thumb_func
Func_02003d1c:
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
	.global Func_02003d24
	.thumb_func
Func_02003d24:
	.ifdef TLA_EDITION_DE
	overlay_veneer Map_GetTerrainHeightFar
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Map_GetTerrainHeightFar
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Map_GetTerrainHeightFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Map_GetTerrainHeightFar
	.else
	overlay_veneer Func_080201f0
	.endif
	.endif
	.endif
	.endif
	.global Func_02003d2c
	.thumb_func
Func_02003d2c:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02003d44
	.thumb_func
Func_02003d44:
	overlay_veneer Map_SetScreenShakeFar
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02003d54
	.thumb_func
Func_02003d54:
	.ifdef TLA_EDITION_DE
	overlay_veneer Runtime_CheckRadiusOverlapFar
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Runtime_CheckRadiusOverlapFar
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Runtime_CheckRadiusOverlapFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Runtime_CheckRadiusOverlapFar
	.else
	overlay_veneer Func_08020378
	.endif
	.endif
	.endif
	.endif
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Func_02003d64
	.thumb_func
Func_02003d64:
	overlay_veneer UiText_DrawQuantity
	.global PartyInventory_FindOwner
	.thumb_func
PartyInventory_FindOwner:
	overlay_veneer PartyInventory_FindOwnerFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02003d7c
	.thumb_func
Func_02003d7c:
	overlay_veneer EventRuntime_BeginFar
	.global Func_02003d84
	.thumb_func
Func_02003d84:
	overlay_veneer EventRuntime_EndFar
	.global UiText_DrawQuantityPairWithCue
	.thumb_func
UiText_DrawQuantityPairWithCue:
	overlay_veneer UiText_DrawQuantityPairWithCueFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_EnableActionAndSetCallback
	.thumb_func
ObjectMotion_EnableActionAndSetCallback:
	overlay_veneer ObjectMotion_EnableActionAndSetCallbackFar
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02003ddc
	.thumb_func
Func_02003ddc:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_02003de4
	.thumb_func
Func_02003de4:
	overlay_veneer ObjectMotion_MoveToObjectFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_02003df4
	.thumb_func
Func_02003df4:
	overlay_veneer BattleFx_RunRisingObjectSequenceFar
	.global Object_LinkPair
	.thumb_func
Object_LinkPair:
	overlay_veneer Object_LinkPairFar
	.global Func_02003e04
	.thumb_func
Func_02003e04:
	overlay_veneer ObjectGroup_ConfigureChildValueFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02003e14
	.thumb_func
Func_02003e14:
	overlay_veneer EventRuntime_SetMessageFar
	.global Func_02003e1c
	.thumb_func
Func_02003e1c:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02003e34
	.thumb_func
Func_02003e34:
	overlay_veneer Func_080c8210
	.global Func_02003e3c
	.thumb_func
Func_02003e3c:
	overlay_veneer Func_080c8218
	.global Func_02003e44
	.thumb_func
Func_02003e44:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02003e54
	.thumb_func
Func_02003e54:
	overlay_veneer Func_080c8240
	.global Func_02003e5c
	.thumb_func
Func_02003e5c:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Func_02003e64
	.thumb_func
Func_02003e64:
	overlay_veneer Func_080c8288
	.global Func_02003e6c
	.thumb_func
Func_02003e6c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c82d0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c82d0
	.else
	overlay_veneer Func_080c82e0
	.endif
	.endif
	.global Func_02003e74
	.thumb_func
Func_02003e74:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8368
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8368
	.else
	overlay_veneer BattleFx_ApplyColorToTargetBufferFar
	.endif
	.endif
	.global Func_02003e7c
	.thumb_func
Func_02003e7c:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_InitializeBuffersFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_InitializeBuffersFar
	.else
	overlay_veneer BattleFx_ApplyColorToSourceBufferFar
	.endif
	.endif
	.global Func_02003e84
	.thumb_func
Func_02003e84:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_ApplyColorToSourceBufferFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_ApplyColorToSourceBufferFar
	.else
	overlay_veneer BattleFx_StartBufferInterpolationFar
	.endif
	.endif
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
	.global Event_FinishScreenTransition
	.thumb_func
Event_FinishScreenTransition:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_SetPrimaryBufferValueFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_SetPrimaryBufferValueFar
	.else
	overlay_veneer Event_FinishScreenTransitionFar
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
	.global Func_02003ea4
	.thumb_func
Func_02003ea4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8470
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8470
	.else
	overlay_veneer Func_080c8480
	.endif
	.endif
	.global Func_02003eac
	.thumb_func
Func_02003eac:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.endif
	.endif
	.global Func_02003eb4
	.thumb_func
Func_02003eb4:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_SnapScaleToFullFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_SnapScaleToFullFar
	.else
	overlay_veneer BattleParty_ApplyDrainFar
	.endif
	.endif
	.global Func_02003ebc
	.thumb_func
Func_02003ebc:
	.ifdef TLA_EDITION_ES
	overlay_veneer Encounter_GetEnemyGroupAtSlotFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Encounter_GetEnemyGroupAtSlotFar
	.else
	overlay_veneer Party_PlaySavedMusicFar
	.endif
	.endif
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Func_02003ecc
	.thumb_func
Func_02003ecc:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8678
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8678
	.else
	overlay_veneer Func_080c8688
	.endif
	.endif
	.global Func_02003ed4
	.thumb_func
Func_02003ed4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Object_SetActionCallbackFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Object_SetActionCallbackFar
	.else
	overlay_veneer Event_NoOpFar
	.endif
	.endif
	.global Func_02003edc
	.thumb_func
Func_02003edc:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86c0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86c0
	.else
	overlay_veneer Func_080c86d0
	.endif
	.endif
	.global Func_02003ee4
	.thumb_func
Func_02003ee4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86c8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86c8
	.else
	overlay_veneer Func_080c86d8
	.endif
	.endif
	.global Func_02003eec
	.thumb_func
Func_02003eec:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8850
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8850
	.else
	overlay_veneer Func_080c8860
	.endif
	.endif
	.global Func_02003ef4
	.thumb_func
Func_02003ef4:
	overlay_veneer Audio_PlayCue
	.global AudioCommand_WaitForCompletion
	.thumb_func
AudioCommand_WaitForCompletion:
	overlay_veneer AudioCommand_WaitForCompletionFar
