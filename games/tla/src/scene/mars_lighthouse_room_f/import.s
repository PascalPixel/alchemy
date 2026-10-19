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
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global ArcTan2
	.thumb_func
ArcTan2:
	overlay_veneer ArcTan2Far
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
	.global Func_02005420
	.thumb_func
Func_02005420:
	overlay_veneer Resource_DecodeType01Far
	.global Resource_ResetEntry
	.thumb_func
Resource_ResetEntry:
	overlay_veneer Resource_ResetEntryFar
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02005440
	.thumb_func
Func_02005440:
	overlay_veneer Runtime_PushSlotEntryFar
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
	.global GameFlag_GetByte
	.thumb_func
GameFlag_GetByte:
	overlay_veneer GameFlag_GetByteFar
	.global GameFlag_SetByte
	.thumb_func
GameFlag_SetByte:
	overlay_veneer GameFlag_SetByteFar
	.global ResourceMetadata_ClearRecord
	.thumb_func
ResourceMetadata_ClearRecord:
	overlay_veneer ResourceMetadata_ClearRecordFar
	.global Func_02005480
	.thumb_func
Func_02005480:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_ApplyValueToChildren
	.thumb_func
ObjectDispatch_ApplyValueToChildren:
	overlay_veneer ObjectDispatch_ApplyValueToChildrenFar
	.global Func_02005490
	.thumb_func
Func_02005490:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02005498
	.thumb_func
Func_02005498:
	overlay_veneer Func_080200c0
	.global Func_020054a0
	.thumb_func
Func_020054a0:
	overlay_veneer Object_Destroy
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_020054b0
	.thumb_func
Func_020054b0:
	overlay_veneer Map_ApplyWorkOriginAndSpanFar
	.global Func_020054b8
	.thumb_func
Func_020054b8:
	overlay_veneer Object_SetPosition
	.global Func_020054c0
	.thumb_func
Func_020054c0:
	overlay_veneer Object_CommitPosition
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
	.global Func_020054d0
	.thumb_func
Func_020054d0:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_08020198
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_08020198
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_08020198
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_08020198
	.else
	overlay_veneer GetMapCellCollisionFar
	.endif
	.endif
	.endif
	.endif
	.global Func_020054d8
	.thumb_func
Func_020054d8:
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
	.global Func_020054e0
	.thumb_func
Func_020054e0:
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
	.global Func_020054e8
	.thumb_func
Func_020054e8:
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
	.global Func_020054f0
	.thumb_func
Func_020054f0:
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
	.global Func_020054f8
	.thumb_func
Func_020054f8:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02005510
	.thumb_func
Func_02005510:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02005518
	.thumb_func
Func_02005518:
	overlay_veneer Func_08020230
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02005528
	.thumb_func
Func_02005528:
	overlay_veneer ObjectDispatch_SetSpritePriorityFar
	.global Func_02005530
	.thumb_func
Func_02005530:
	overlay_veneer Func_080202f8
	.global Func_02005538
	.thumb_func
Func_02005538:
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
	.global Func_02005540
	.thumb_func
Func_02005540:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_08020330
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_08020330
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_08020330
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_08020330
	.else
	overlay_veneer Func_08020360
	.endif
	.endif
	.endif
	.endif
	.global Func_02005548
	.thumb_func
Func_02005548:
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
	.global Party_CountActiveOwners
	.thumb_func
Party_CountActiveOwners:
	overlay_veneer Party_CountActiveOwnersFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02005560
	.thumb_func
Func_02005560:
	overlay_veneer EventRuntime_BeginFar
	.global Func_02005568
	.thumb_func
Func_02005568:
	overlay_veneer EventRuntime_EndFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02005598
	.thumb_func
Func_02005598:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_020055a8
	.thumb_func
Func_020055a8:
	overlay_veneer BattleFx_RunRisingObjectSequenceFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_020055b8
	.thumb_func
Func_020055b8:
	overlay_veneer Func_080c8208
	.global Func_020055c0
	.thumb_func
Func_020055c0:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Func_020055d0
	.thumb_func
Func_020055d0:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_020055e0
	.thumb_func
Func_020055e0:
	overlay_veneer Func_080c8240
	.global Func_020055e8
	.thumb_func
Func_020055e8:
	overlay_veneer Func_080c8258
	.global Func_020055f0
	.thumb_func
Func_020055f0:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Func_020055f8
	.thumb_func
Func_020055f8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Object_GetTriggerTileAheadOfCurrentFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Object_GetTriggerTileAheadOfCurrentFar
	.else
	overlay_veneer Func_080c82c0
	.endif
	.endif
	.global Func_02005600
	.thumb_func
Func_02005600:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c82d0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c82d0
	.else
	overlay_veneer Func_080c82e0
	.endif
	.endif
	.global Func_02005608
	.thumb_func
Func_02005608:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c82e8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c82e8
	.else
	overlay_veneer EventRuntime_PrepareCurrentObjectFar
	.endif
	.endif
	.global Func_02005610
	.thumb_func
Func_02005610:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8340
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8340
	.else
	overlay_veneer DisplayTransition_StartFar
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
	.global ObjectTable_Get
	.thumb_func
ObjectTable_Get:
	.ifdef TLA_EDITION_ES
	overlay_veneer Event_SpawnObjectTableFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Event_SpawnObjectTableFar
	.else
	overlay_veneer ObjectTable_GetFar
	.endif
	.endif
	.global Func_02005638
	.thumb_func
Func_02005638:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8470
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8470
	.else
	overlay_veneer Func_080c8480
	.endif
	.endif
	.global Func_02005640
	.thumb_func
Func_02005640:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_InitializeSharedSceneFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_InitializeSharedSceneFar
	.else
	overlay_veneer EventRuntime_ResolveAllPendingActionsFar
	.endif
	.endif
	.global Func_02005648
	.thumb_func
Func_02005648:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.endif
	.endif
	.global Func_02005650
	.thumb_func
Func_02005650:
	.ifdef TLA_EDITION_ES
	overlay_veneer UpdateRisingParticleBurstFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer UpdateRisingParticleBurstFar
	.else
	overlay_veneer BattleParty_ApplyHealthDeltaFar
	.endif
	.endif
	.global Func_02005658
	.thumb_func
Func_02005658:
	overlay_veneer Camera_WorldToScreenFar
	.global Func_02005660
	.thumb_func
Func_02005660:
	.ifdef TLA_EDITION_ES
	overlay_veneer Object_BuildRecordSnapshotFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Object_BuildRecordSnapshotFar
	.else
	overlay_veneer ObjectTable_SnapshotFar
	.endif
	.endif
	.global Func_02005668
	.thumb_func
Func_02005668:
	.ifdef TLA_EDITION_ES
	overlay_veneer Object_SetActionCallbackFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Object_SetActionCallbackFar
	.else
	overlay_veneer Event_NoOpFar
	.endif
	.endif
	.global Func_02005670
	.thumb_func
Func_02005670:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86c0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86c0
	.else
	overlay_veneer Func_080c86d0
	.endif
	.endif
	.global Func_02005678
	.thumb_func
Func_02005678:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86c8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86c8
	.else
	overlay_veneer Func_080c86d8
	.endif
	.endif
	.global Func_02005680
	.thumb_func
Func_02005680:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8728
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8728
	.else
	overlay_veneer Func_080c8738
	.endif
	.endif
	.global Func_02005688
	.thumb_func
Func_02005688:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8738
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8738
	.else
	overlay_veneer Func_080c8748
	.endif
	.endif
	.global Func_02005690
	.thumb_func
Func_02005690:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8768
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8768
	.else
	overlay_veneer EventRuntime_GetControlledOwnerFar
	.endif
	.endif
	.global Func_02005698
	.thumb_func
Func_02005698:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_StepVector2Far
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_StepVector2Far
	.else
	overlay_veneer Func_080c87e8
	.endif
	.endif
	.global Func_020056a0
	.thumb_func
Func_020056a0:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_SetObjectTickModeFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_SetObjectTickModeFar
	.else
	overlay_veneer Func_080c8830
	.endif
	.endif
	.global Func_020056a8
	.thumb_func
Func_020056a8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8838
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8838
	.else
	overlay_veneer Func_080c8848
	.endif
	.endif
	.global Func_020056b0
	.thumb_func
Func_020056b0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8870
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8870
	.else
	overlay_veneer Func_080c8880
	.endif
	.endif
	.global Func_020056b8
	.thumb_func
Func_020056b8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8878
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8878
	.else
	overlay_veneer Func_080c8888
	.endif
	.endif
	.global Func_020056c0
	.thumb_func
Func_020056c0:
	overlay_veneer Audio_PlayCue
