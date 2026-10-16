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
	.global Resource_ResetEntry
	.thumb_func
Resource_ResetEntry:
	overlay_veneer Resource_ResetEntryFar
	.global UnitGet
	.thumb_func
UnitGet:
	overlay_veneer UnitGetFar
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
	.global ResourceMetadata_ClearRecord
	.thumb_func
ResourceMetadata_ClearRecord:
	overlay_veneer ResourceMetadata_ClearRecordFar
	.global Func_02003118
	.thumb_func
Func_02003118:
	overlay_veneer Object_SetMode
	.global Func_02003120
	.thumb_func
Func_02003120:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02003128
	.thumb_func
Func_02003128:
	overlay_veneer Func_080200c0
	.global Func_02003130
	.thumb_func
Func_02003130:
	overlay_veneer Object_Destroy
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02003140
	.thumb_func
Func_02003140:
	overlay_veneer Object_SetPosition
	.global Func_02003148
	.thumb_func
Func_02003148:
	overlay_veneer Map_CopyMetatileIndicesRectFar
	.global Func_02003150
	.thumb_func
Func_02003150:
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
	.global Func_02003158
	.thumb_func
Func_02003158:
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
	.global Func_02003168
	.thumb_func
Func_02003168:
	.ifdef TLA_EDITION_DE
	overlay_veneer DisplayBlend_DisableRunScriptFar
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer DisplayBlend_DisableRunScriptFar
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer DisplayBlend_DisableRunScriptFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer DisplayBlend_DisableRunScriptFar
	.else
	overlay_veneer Func_080201d8
	.endif
	.endif
	.endif
	.endif
	.global Func_02003170
	.thumb_func
Func_02003170:
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
	.global Func_02003178
	.thumb_func
Func_02003178:
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
	.global Func_02003180
	.thumb_func
Func_02003180:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02003198
	.thumb_func
Func_02003198:
	overlay_veneer Runtime_SetWorkTripleIfNonNegativeFar
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_020031a8
	.thumb_func
Func_020031a8:
	overlay_veneer ObjectDispatch_SetSpritePriorityFar
	.ifndef TLA_EDITION_JA
	.global Func_020031b0
	.thumb_func
Func_020031b0:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_08020318
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_08020318
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_08020318
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_08020318
	.else
	overlay_veneer Runtime_CheckRadiusOverlapFar
	.endif
	.endif
	.endif
	.endif
	.endif
	.global Func_020031b8
	.thumb_func
Func_020031b8:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_08020388
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_08020388
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_08020388
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_08020388
	.else
	overlay_veneer Func_08020390
	.endif
	.endif
	.endif
	.endif
	.global Func_020031c0
	.thumb_func
Func_020031c0:
	overlay_veneer Func_08038210
	.global PartyInventory_FindOwner
	.thumb_func
PartyInventory_FindOwner:
	overlay_veneer PartyInventory_FindOwnerFar
	.global Party_CountActiveOwners
	.thumb_func
Party_CountActiveOwners:
	overlay_veneer Party_CountActiveOwnersFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_020031e0
	.thumb_func
Func_020031e0:
	overlay_veneer EventRuntime_BeginFar
	.global Func_020031e8
	.thumb_func
Func_020031e8:
	overlay_veneer EventRuntime_EndFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Func_020031f8
	.thumb_func
Func_020031f8:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_02003200
	.thumb_func
Func_02003200:
	overlay_veneer BattleFx_RunRisingObjectSequenceFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02003210
	.thumb_func
Func_02003210:
	overlay_veneer Func_080c8218
	.global Func_02003218
	.thumb_func
Func_02003218:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02003228
	.thumb_func
Func_02003228:
	overlay_veneer Func_080c8240
	.global Func_02003230
	.thumb_func
Func_02003230:
	overlay_veneer Func_080c8248
	.global Func_02003238
	.thumb_func
Func_02003238:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Func_02003240
	.thumb_func
Func_02003240:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8340
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8340
	.else
	overlay_veneer DisplayTransition_StartFar
	.endif
	.endif
	.global Func_02003248
	.thumb_func
Func_02003248:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8368
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8368
	.else
	overlay_veneer BattleFx_ApplyColorToTargetBufferFar
	.endif
	.endif
	.global Func_02003250
	.thumb_func
Func_02003250:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_InitializeBuffersFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_InitializeBuffersFar
	.else
	overlay_veneer BattleFx_ApplyColorToSourceBufferFar
	.endif
	.endif
	.global Func_02003258
	.thumb_func
Func_02003258:
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
	.global Func_02003270
	.thumb_func
Func_02003270:
	.ifdef TLA_EDITION_ES
	overlay_veneer Event_FinishScreenTransitionFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Event_FinishScreenTransitionFar
	.else
	overlay_veneer DisplayScroll_InitializeHBlankDmaFar
	.endif
	.endif
	.global Func_02003278
	.thumb_func
Func_02003278:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8470
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8470
	.else
	overlay_veneer Func_080c8480
	.endif
	.endif
	.global Func_02003280
	.thumb_func
Func_02003280:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.endif
	.endif
	.global Func_02003288
	.thumb_func
Func_02003288:
	.ifdef TLA_EDITION_ES
	overlay_veneer UpdateRisingParticleBurstFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer UpdateRisingParticleBurstFar
	.else
	overlay_veneer BattleParty_ApplyHealthDeltaFar
	.endif
	.endif
	.global Func_02003290
	.thumb_func
Func_02003290:
	.ifdef TLA_EDITION_ES
	overlay_veneer Encounter_GetEnemyGroupAtSlotFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Encounter_GetEnemyGroupAtSlotFar
	.else
	overlay_veneer Party_PlaySavedMusicFar
	.endif
	.endif
	.global Func_02003298
	.thumb_func
Func_02003298:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8768
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8768
	.else
	overlay_veneer EventRuntime_GetControlledOwnerFar
	.endif
	.endif
	.global Func_020032a0
	.thumb_func
Func_020032a0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c88c8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c88c8
	.else
	overlay_veneer Func_080c88d8
	.endif
	.endif
	.global Func_020032a8
	.thumb_func
Func_020032a8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c88d8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c88d8
	.else
	overlay_veneer Func_080c88e8
	.endif
	.endif
	.global Func_020032b0
	.thumb_func
Func_020032b0:
	.ifdef TLA_EDITION_ES
	overlay_veneer ObjectEffect_BeginContextEffect25Far
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer ObjectEffect_BeginContextEffect25Far
	.else
	overlay_veneer Func_080c8918
	.endif
	.endif
	.global Func_020032b8
	.thumb_func
Func_020032b8:
	overlay_veneer Audio_PlayCue
	.global AudioCommand_WaitForCompletion
	.thumb_func
AudioCommand_WaitForCompletion:
	overlay_veneer AudioCommand_WaitForCompletionFar
