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
	.global Runtime_BumpAllocateAlternatePool
	.thumb_func
Runtime_BumpAllocateAlternatePool:
	overlay_veneer Runtime_BumpAllocateAlternatePoolFar
	.global MemFree
	.thumb_func
MemFree:
	overlay_veneer Sys_FreeFar
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
	.global ResourceMetadata_Register
	.thumb_func
ResourceMetadata_Register:
	overlay_veneer ResourceMetadata_RegisterFar
	.global Func_02004d9c
	.thumb_func
Func_02004d9c:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_SetAnimationSpeed
	.thumb_func
ObjectDispatch_SetAnimationSpeed:
	overlay_veneer ObjectDispatch_SetAnimationSpeedFar
	.global Func_02004dac
	.thumb_func
Func_02004dac:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Engine_ObjectCreate
	.thumb_func
Engine_ObjectCreate:
	overlay_veneer Func_080200c0
	.global Func_02004dbc
	.thumb_func
Func_02004dbc:
	overlay_veneer Object_Destroy
	.global Func_02004dc4
	.thumb_func
Func_02004dc4:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02004dcc
	.thumb_func
Func_02004dcc:
	overlay_veneer Object_SetPosition
	.global Func_02004dd4
	.thumb_func
Func_02004dd4:
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
	.global Func_02004ddc
	.thumb_func
Func_02004ddc:
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
	.global Func_02004de4
	.thumb_func
Func_02004de4:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_08020188
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_08020188
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_08020188
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_08020188
	.else
	overlay_veneer DisplayBlend_StartScriptFar
	.endif
	.endif
	.endif
	.endif
	.global Func_02004dec
	.thumb_func
Func_02004dec:
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
	.global Func_02004df4
	.thumb_func
Func_02004df4:
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
	.global Func_02004dfc
	.thumb_func
Func_02004dfc:
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
	.global Func_02004e04
	.thumb_func
Func_02004e04:
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
	.global Func_02004e0c
	.thumb_func
Func_02004e0c:
	.ifdef TLA_EDITION_DE
	overlay_veneer GetMapCellCollisionFar
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer GetMapCellCollisionFar
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer GetMapCellCollisionFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer GetMapCellCollisionFar
	.else
	overlay_veneer Func_080201f8
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
	.global Func_02004e24
	.thumb_func
Func_02004e24:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02004e2c
	.thumb_func
Func_02004e2c:
	overlay_veneer Func_08020230
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02004e3c
	.thumb_func
Func_02004e3c:
	overlay_veneer ObjectDispatch_SetSpritePriorityFar
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global PartyInventory_FindOwner
	.thumb_func
PartyInventory_FindOwner:
	overlay_veneer PartyInventory_FindOwnerFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02004e5c
	.thumb_func
Func_02004e5c:
	overlay_veneer EventRuntime_BeginFar
	.global Func_02004e64
	.thumb_func
Func_02004e64:
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
	.global Object_RefreshSelectorById
	.thumb_func
Object_RefreshSelectorById:
	overlay_veneer Object_RefreshSelectorByIdFar
	.global ObjectMotion_EnableActionAndResetMotion
	.thumb_func
ObjectMotion_EnableActionAndResetMotion:
	overlay_veneer ObjectMotion_EnableActionAndResetMotionFar
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02004ebc
	.thumb_func
Func_02004ebc:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_02004ec4
	.thumb_func
Func_02004ec4:
	overlay_veneer ObjectMotion_MoveToObjectFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global ObjectMotion_Launch
	.thumb_func
ObjectMotion_Launch:
	overlay_veneer ObjectMotion_LaunchFar
	.global ObjectMotion_SetVariantCallback
	.thumb_func
ObjectMotion_SetVariantCallback:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global Func_02004eec
	.thumb_func
Func_02004eec:
	overlay_veneer BattleEffect_RunRisingObjectSequenceFar
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Func_02004efc
	.thumb_func
Func_02004efc:
	overlay_veneer ObjectGroup_ConfigureChildValueFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02004f0c
	.thumb_func
Func_02004f0c:
	overlay_veneer EventRuntime_SetMessageFar
	.global Func_02004f14
	.thumb_func
Func_02004f14:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02004f2c
	.thumb_func
Func_02004f2c:
	overlay_veneer Func_080c8210
	.global Func_02004f34
	.thumb_func
Func_02004f34:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Func_02004f44
	.thumb_func
Func_02004f44:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02004f54
	.thumb_func
Func_02004f54:
	overlay_veneer Func_080c8240
	.global Func_02004f5c
	.thumb_func
Func_02004f5c:
	overlay_veneer Func_080c8248
	.global Func_02004f64
	.thumb_func
Func_02004f64:
	overlay_veneer Event_SetDestinationFar
	.global Func_02004f6c
	.thumb_func
Func_02004f6c:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Func_02004f74
	.thumb_func
Func_02004f74:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Party_SetFields1eeAnd1f0
	.thumb_func
Party_SetFields1eeAnd1f0:
	overlay_veneer Party_SetFields1eeAnd1f0Far
	.global Party_SetFields1f2And1f4
	.thumb_func
Party_SetFields1f2And1f4:
	overlay_veneer Party_SetFields1f2And1f4Far
	.global Func_02004f8c
	.thumb_func
Func_02004f8c:
	.ifdef TLA_EDITION_ES
	overlay_veneer DisplayTransition_FinishFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer DisplayTransition_FinishFar
	.else
	overlay_veneer Func_080c8368
	.endif
	.endif
	.global Func_02004f94
	.thumb_func
Func_02004f94:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8368
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8368
	.else
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.endif
	.endif
	.global Func_02004f9c
	.thumb_func
Func_02004f9c:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_InitializeBuffersFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_InitializeBuffersFar
	.else
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.endif
	.endif
	.global Func_02004fa4
	.thumb_func
Func_02004fa4:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.else
	overlay_veneer BattleEffect_StartBufferInterpolationFar
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
	overlay_veneer BattleEffect_SetPrimaryBufferValueFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_SetPrimaryBufferValueFar
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
	.global Func_02004fc4
	.thumb_func
Func_02004fc4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8470
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8470
	.else
	overlay_veneer Func_080c8480
	.endif
	.endif
	.global Func_02004fcc
	.thumb_func
Func_02004fcc:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.endif
	.endif
	.global Func_02004fd4
	.thumb_func
Func_02004fd4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Encounter_GetEnemyGroupAtSlotFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Encounter_GetEnemyGroupAtSlotFar
	.else
	overlay_veneer Party_PlaySavedMusicFar
	.endif
	.endif
	.global Func_02004fdc
	.thumb_func
Func_02004fdc:
	.ifdef TLA_EDITION_ES
	overlay_veneer Object_BuildRecordSnapshotFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Object_BuildRecordSnapshotFar
	.else
	overlay_veneer ObjectTable_SnapshotFar
	.endif
	.endif
	.global Func_02004fe4
	.thumb_func
Func_02004fe4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8768
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8768
	.else
	overlay_veneer EventRuntime_GetControlledOwnerFar
	.endif
	.endif
	.global Func_02004fec
	.thumb_func
Func_02004fec:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8770
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8770
	.else
	overlay_veneer Func_080c8780
	.endif
	.endif
	.global Func_02004ff4
	.thumb_func
Func_02004ff4:
	.ifdef TLA_EDITION_ES
	overlay_veneer ObjectGroup_ApplyRandomChildValuesFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer ObjectGroup_ApplyRandomChildValuesFar
	.else
	overlay_veneer Func_080c88d0
	.endif
	.endif
	.global ObjectMotion_ResetTargetsAndVelocity
	.thumb_func
ObjectMotion_ResetTargetsAndVelocity:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c88e8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c88e8
	.else
	overlay_veneer ObjectMotion_ResetTargetsAndVelocityFar
	.endif
	.endif
	.ifndef TLA_EDITION_JA
	.global Func_02005004
	.thumb_func
Func_02005004:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8930
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8930
	.else
	overlay_veneer Func_080c8940
	.endif
	.endif
	.endif
	.global Func_0200500c
	.thumb_func
Func_0200500c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8938
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8938
	.else
	overlay_veneer Func_080c8948
	.endif
	.endif
	.global Func_02005014
	.thumb_func
Func_02005014:
	overlay_veneer Audio_PlayCue
