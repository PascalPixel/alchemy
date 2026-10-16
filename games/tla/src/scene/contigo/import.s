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
	.global __umodsi3
	.thumb_func
__umodsi3:
	overlay_veneer IwramUnsignedRemainder
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
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.global Math_Cosine
	.thumb_func
Math_Cosine:
	overlay_veneer Math_CosineFar
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
	.global Func_02003fdc
	.thumb_func
Func_02003fdc:
	overlay_veneer Object_SetMode
	.global Func_02003fe4
	.thumb_func
Func_02003fe4:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02003fec
	.thumb_func
Func_02003fec:
	overlay_veneer Func_080200c0
	.global Func_02003ff4
	.thumb_func
Func_02003ff4:
	overlay_veneer Object_Destroy
	.global Func_02003ffc
	.thumb_func
Func_02003ffc:
	overlay_veneer Map_ApplyWorkOriginAndSpanFar
	.global Func_02004004
	.thumb_func
Func_02004004:
	overlay_veneer Map_CopyMetatileIndicesRectFar
	.global Func_0200400c
	.thumb_func
Func_0200400c:
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
	.global Func_0200401c
	.thumb_func
Func_0200401c:
	overlay_veneer Runtime_SetWorkTripleIfNonNegativeFar
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_0200402c
	.thumb_func
Func_0200402c:
	overlay_veneer ObjectDispatch_SetSpritePriorityFar
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Func_0200403c
	.thumb_func
Func_0200403c:
	overlay_veneer UiText_DrawQuantity
	.global Func_02004044
	.thumb_func
Func_02004044:
	overlay_veneer Func_080ad240
	.global Func_0200404c
	.thumb_func
Func_0200404c:
	overlay_veneer Party_GetAverageLevelFar
	.global PartyInventory_CountItem
	.thumb_func
PartyInventory_CountItem:
	overlay_veneer PartyInventory_CountItemFar
	.global PartyInventory_CountFreeSlots
	.thumb_func
PartyInventory_CountFreeSlots:
	overlay_veneer PartyInventory_CountFreeSlotsFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_0200406c
	.thumb_func
Func_0200406c:
	overlay_veneer EventRuntime_BeginFar
	.global Func_02004074
	.thumb_func
Func_02004074:
	overlay_veneer EventRuntime_EndFar
	.global PartyInventory_GiveItem
	.thumb_func
PartyInventory_GiveItem:
	overlay_veneer PartyInventory_GiveItemFar
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
	.global ObjectMotion_EnableActionAndResetMotion
	.thumb_func
ObjectMotion_EnableActionAndResetMotion:
	overlay_veneer ObjectMotion_EnableActionAndResetMotionFar
	.global Object_SetActionCallbackAndRefreshById
	.thumb_func
Object_SetActionCallbackAndRefreshById:
	overlay_veneer Object_SetActionCallbackAndRefreshByIdFar
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
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
	.global Func_020040d4
	.thumb_func
Func_020040d4:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_020040dc
	.thumb_func
Func_020040dc:
	overlay_veneer ObjectMotion_SetHorizontalPositionAndAngleWithTerrainFar
	.global Func_020040e4
	.thumb_func
Func_020040e4:
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
	.global Object_LinkPair
	.thumb_func
Object_LinkPair:
	overlay_veneer Object_LinkPairFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_0200412c
	.thumb_func
Func_0200412c:
	overlay_veneer EventRuntime_SetMessageFar
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_0200413c
	.thumb_func
Func_0200413c:
	overlay_veneer Func_080c8198
	.global Func_02004144
	.thumb_func
Func_02004144:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global Func_0200414c
	.thumb_func
Func_0200414c:
	overlay_veneer Func_080c81a8
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_0200415c
	.thumb_func
Func_0200415c:
	overlay_veneer ObjectMotion_ArmCallbackAndRefreshFar
	.global Func_02004164
	.thumb_func
Func_02004164:
	overlay_veneer Battle_ShowPairedUnitWorkAndWaitFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02004174
	.thumb_func
Func_02004174:
	overlay_veneer Func_080c8210
	.global Func_0200417c
	.thumb_func
Func_0200417c:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Func_0200418c
	.thumb_func
Func_0200418c:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_0200419c
	.thumb_func
Func_0200419c:
	overlay_veneer Func_080c8240
	.global Func_020041a4
	.thumb_func
Func_020041a4:
	overlay_veneer Func_080c8260
	.global Func_020041ac
	.thumb_func
Func_020041ac:
	overlay_veneer Event_SetDestinationFar
	.global Func_020041b4
	.thumb_func
Func_020041b4:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Party_SetFields1eeAnd1f0
	.thumb_func
Party_SetFields1eeAnd1f0:
	overlay_veneer Party_SetFields1eeAnd1f0Far
	.global Field_SetDestination
	.thumb_func
Field_SetDestination:
	overlay_veneer Field_SetDestinationFar
	.global Func_020041cc
	.thumb_func
Func_020041cc:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8368
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8368
	.else
	overlay_veneer BattleFx_ApplyColorToTargetBufferFar
	.endif
	.endif
	.global Func_020041d4
	.thumb_func
Func_020041d4:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_InitializeBuffersFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_InitializeBuffersFar
	.else
	overlay_veneer BattleFx_ApplyColorToSourceBufferFar
	.endif
	.endif
	.global Func_020041dc
	.thumb_func
Func_020041dc:
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
	.global Func_020041fc
	.thumb_func
Func_020041fc:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_PlayCueAndStartEmitterOnTargetFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_PlayCueAndStartEmitterOnTargetFar
	.else
	overlay_veneer Func_080c83e0
	.endif
	.endif
	.global Func_02004204
	.thumb_func
Func_02004204:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.endif
	.endif
	.global Func_0200420c
	.thumb_func
Func_0200420c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Encounter_GetEnemyGroupAtSlotFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Encounter_GetEnemyGroupAtSlotFar
	.else
	overlay_veneer Party_PlaySavedMusicFar
	.endif
	.endif
	.global Func_02004214
	.thumb_func
Func_02004214:
	.ifdef TLA_EDITION_ES
	overlay_veneer Party_PlaySavedMusicFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Party_PlaySavedMusicFar
	.else
	overlay_veneer Func_080c8580
	.endif
	.endif
	.global Func_0200421c
	.thumb_func
Func_0200421c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Camera_WorldToScreenFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Camera_WorldToScreenFar
	.else
	overlay_veneer Func_080c85c0
	.endif
	.endif
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_0200422c
	.thumb_func
Func_0200422c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Object_BuildRecordSnapshotFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Object_BuildRecordSnapshotFar
	.else
	overlay_veneer ObjectTable_SnapshotFar
	.endif
	.endif
	.global Object_Spawn
	.thumb_func
Object_Spawn:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8748
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8748
	.else
	overlay_veneer Object_SpawnFar
	.endif
	.endif
	.global Func_0200423c
	.thumb_func
Func_0200423c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8750
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8750
	.else
	overlay_veneer Func_080c8760
	.endif
	.endif
	.global Func_02004244
	.thumb_func
Func_02004244:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87b0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87b0
	.else
	overlay_veneer Func_080c87c0
	.endif
	.endif
	.global Func_0200424c
	.thumb_func
Func_0200424c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87b8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87b8
	.else
	overlay_veneer Func_080c87c8
	.endif
	.endif
	.global Func_02004254
	.thumb_func
Func_02004254:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_StepVector2Far
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_StepVector2Far
	.else
	overlay_veneer Func_080c87e8
	.endif
	.endif
	.global Func_0200425c
	.thumb_func
Func_0200425c:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_StepVector3Far
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_StepVector3Far
	.else
	overlay_veneer Func_080c87f0
	.endif
	.endif
	.global Func_02004264
	.thumb_func
Func_02004264:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87f8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87f8
	.else
	overlay_veneer BattleFx_StartArcEffectFar
	.endif
	.endif
	.global Func_0200426c
	.thumb_func
Func_0200426c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Field_EndActorSpriteEffectFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Field_EndActorSpriteEffectFar
	.else
	overlay_veneer BattleFx_EndArcEffectFar
	.endif
	.endif
	.global Func_02004274
	.thumb_func
Func_02004274:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_StartArcEffectFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_StartArcEffectFar
	.else
	overlay_veneer BattleFx_WaitForArcGroupsFar
	.endif
	.endif
	.global Func_0200427c
	.thumb_func
Func_0200427c:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_EndArcEffectFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_EndArcEffectFar
	.else
	overlay_veneer BattleFx_SetObjectTickModeFar
	.endif
	.endif
	.global Func_02004284
	.thumb_func
Func_02004284:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c88c8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c88c8
	.else
	overlay_veneer Func_080c88d8
	.endif
	.endif
	.global Func_0200428c
	.thumb_func
Func_0200428c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c88d8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c88d8
	.else
	overlay_veneer Func_080c88e8
	.endif
	.endif
	.global Func_02004294
	.thumb_func
Func_02004294:
	overlay_veneer Func_08108008
	.global Func_0200429c
	.thumb_func
Func_0200429c:
	overlay_veneer Func_08108010
	.global Func_020042a4
	.thumb_func
Func_020042a4:
	overlay_veneer Func_08108018
	.global AudioCommand_WaitForStateByteClear
	.thumb_func
AudioCommand_WaitForStateByteClear:
	overlay_veneer AudioCommand_WaitForStateByteClearFar
	.global Func_020042b4
	.thumb_func
Func_020042b4:
	overlay_veneer Audio_PlayCue
