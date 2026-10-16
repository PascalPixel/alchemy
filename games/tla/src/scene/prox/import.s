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
	.global ArcTan2
	.thumb_func
ArcTan2:
	overlay_veneer ArcTan2Far
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global Runtime_AllocateHeapBlockFar
	.thumb_func
Runtime_AllocateHeapBlockFar:
	overlay_veneer Func_08000140
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	overlay_veneer Runtime_ReleaseHeapBlockFar
	.global Func_02004384
	.thumb_func
Func_02004384:
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
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
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
	.global Func_020043c4
	.thumb_func
Func_020043c4:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_ApplyValueToChildren
	.thumb_func
ObjectDispatch_ApplyValueToChildren:
	overlay_veneer ObjectDispatch_ApplyValueToChildrenFar
	.global Func_020043d4
	.thumb_func
Func_020043d4:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_020043dc
	.thumb_func
Func_020043dc:
	overlay_veneer Func_080200c0
	.global Func_020043e4
	.thumb_func
Func_020043e4:
	overlay_veneer Object_Destroy
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_020043f4
	.thumb_func
Func_020043f4:
	overlay_veneer Object_SetPosition
	.global Func_020043fc
	.thumb_func
Func_020043fc:
	overlay_veneer Object_CommitPosition
	.global Func_02004404
	.thumb_func
Func_02004404:
	overlay_veneer Map_CopyMetatileIndicesRectFar
	.global Func_0200440c
	.thumb_func
Func_0200440c:
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
	.global Func_02004414
	.thumb_func
Func_02004414:
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
	.global Func_02004424
	.thumb_func
Func_02004424:
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
	.global Func_0200442c
	.thumb_func
Func_0200442c:
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
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02004444
	.thumb_func
Func_02004444:
	overlay_veneer Runtime_SetWorkTripleIfNonNegativeFar
	.global Func_0200444c
	.thumb_func
Func_0200444c:
	overlay_veneer Func_080202f8
	.global Func_02004454
	.thumb_func
Func_02004454:
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
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02004464
	.thumb_func
Func_02004464:
	overlay_veneer EventRuntime_BeginFar
	.global Func_0200446c
	.thumb_func
Func_0200446c:
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
	.global ObjectMotion_EnableActionAndSetCallback
	.thumb_func
ObjectMotion_EnableActionAndSetCallback:
	overlay_veneer ObjectMotion_EnableActionAndSetCallbackFar
	.global Object_RefreshSelectorById
	.thumb_func
Object_RefreshSelectorById:
	overlay_veneer Object_RefreshSelectorByIdFar
	.global ObjectMotion_EnableActionAndResetMotion
	.thumb_func
ObjectMotion_EnableActionAndResetMotion:
	overlay_veneer ObjectMotion_EnableActionAndResetMotionFar
	.global Object_SetActionCallbackAndRefreshById
	.thumb_func
Object_SetActionCallbackAndRefreshById:
	overlay_veneer Object_SetActionCallbackAndRefreshByIdFar
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
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
	.global Func_020044cc
	.thumb_func
Func_020044cc:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_020044d4
	.thumb_func
Func_020044d4:
	overlay_veneer ObjectMotion_SetHorizontalPositionAndAngleWithTerrainFar
	.global Func_020044dc
	.thumb_func
Func_020044dc:
	overlay_veneer ObjectMotion_MoveToObjectFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global ObjectMotion_WaitForAnimationChange
	.thumb_func
ObjectMotion_WaitForAnimationChange:
	overlay_veneer ObjectMotion_WaitForAnimationChangeFar
	.global ObjectMotion_Launch
	.thumb_func
ObjectMotion_Launch:
	overlay_veneer ObjectMotion_LaunchFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_0200451c
	.thumb_func
Func_0200451c:
	overlay_veneer EventRuntime_SetMessageFar
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_0200452c
	.thumb_func
Func_0200452c:
	overlay_veneer Func_080c8198
	.global Func_02004534
	.thumb_func
Func_02004534:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global Func_0200453c
	.thumb_func
Func_0200453c:
	overlay_veneer Func_080c81a8
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_0200454c
	.thumb_func
Func_0200454c:
	overlay_veneer ObjectMotion_ArmCallbackAndRefreshFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_0200455c
	.thumb_func
Func_0200455c:
	overlay_veneer Func_080c8210
	.global Func_02004564
	.thumb_func
Func_02004564:
	overlay_veneer Func_080c8218
	.global Func_0200456c
	.thumb_func
Func_0200456c:
	overlay_veneer ObjectTable_AllocateAndSetObjectSpeedFar
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_0200457c
	.thumb_func
Func_0200457c:
	overlay_veneer Func_080c8240
	.global Func_02004584
	.thumb_func
Func_02004584:
	overlay_veneer Func_080c8258
	.global Func_0200458c
	.thumb_func
Func_0200458c:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Func_02004594
	.thumb_func
Func_02004594:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c82e8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c82e8
	.else
	overlay_veneer EventRuntime_PrepareCurrentObjectFar
	.endif
	.endif
	.global Func_0200459c
	.thumb_func
Func_0200459c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8368
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8368
	.else
	overlay_veneer BattleFx_ApplyColorToTargetBufferFar
	.endif
	.endif
	.global Func_020045a4
	.thumb_func
Func_020045a4:
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
	.global Object_SetWideSprite
	.thumb_func
Object_SetWideSprite:
	.ifdef TLA_EDITION_ES
	overlay_veneer ObjectEvent_RemoveFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer ObjectEvent_RemoveFar
	.else
	overlay_veneer Object_SetWideSpriteFar
	.endif
	.endif
	.global Func_020045cc
	.thumb_func
Func_020045cc:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8418
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8418
	.else
	overlay_veneer Object_ReturnHomeFar
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
	.global Func_020045dc
	.thumb_func
Func_020045dc:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8470
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8470
	.else
	overlay_veneer Func_080c8480
	.endif
	.endif
	.global Func_020045e4
	.thumb_func
Func_020045e4:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_InitializeSharedSceneFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_InitializeSharedSceneFar
	.else
	overlay_veneer EventRuntime_ResolveAllPendingActionsFar
	.endif
	.endif
	.global Func_020045ec
	.thumb_func
Func_020045ec:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.endif
	.endif
	.global Func_020045f4
	.thumb_func
Func_020045f4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Party_PlaySavedMusicFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Party_PlaySavedMusicFar
	.else
	overlay_veneer Func_080c8580
	.endif
	.endif
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_02004604
	.thumb_func
Func_02004604:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87b0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87b0
	.else
	overlay_veneer Func_080c87c0
	.endif
	.endif
	.global Func_0200460c
	.thumb_func
Func_0200460c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87c0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87c0
	.else
	overlay_veneer Func_080c87d0
	.endif
	.endif
	.global Func_02004614
	.thumb_func
Func_02004614:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87d0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87d0
	.else
	overlay_veneer BattleFx_StepVector3Far
	.endif
	.endif
	.global Func_0200461c
	.thumb_func
Func_0200461c:
	.ifdef TLA_EDITION_ES
	overlay_veneer ObjectEffect_BeginContextEffect25Far
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer ObjectEffect_BeginContextEffect25Far
	.else
	overlay_veneer Func_080c8918
	.endif
	.endif
	.global Func_02004624
	.thumb_func
Func_02004624:
	overlay_veneer Func_08108008
	.global Func_0200462c
	.thumb_func
Func_0200462c:
	overlay_veneer Func_08108010
	.global Func_02004634
	.thumb_func
Func_02004634:
	overlay_veneer Func_08108018
	.global Func_0200463c
	.thumb_func
Func_0200463c:
	overlay_veneer Audio_PlayCue
