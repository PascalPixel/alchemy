.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
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
	.global ArcTan2
	.thumb_func
ArcTan2:
	overlay_veneer ArcTan2Far
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global Func_02001b7c
	.thumb_func
Func_02001b7c:
	overlay_veneer Object_SetMode
	.global Func_02001b84
	.thumb_func
Func_02001b84:
	overlay_veneer Map_PlayMetatileCopySequenceFar
	.global Func_02001b8c
	.thumb_func
Func_02001b8c:
	overlay_veneer Func_080201f0
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Animation_SetIndexAndInitializeObjects
	.thumb_func
Animation_SetIndexAndInitializeObjects:
	overlay_veneer Animation_SetIndexAndInitializeObjectsFar
	.global Func_02001ba4
	.thumb_func
Func_02001ba4:
	overlay_veneer ObjectDispatch_SetSpritePriorityFar
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Func_02001bb4
	.thumb_func
Func_02001bb4:
	overlay_veneer Func_080ad208
	.global Func_02001bbc
	.thumb_func
Func_02001bbc:
	overlay_veneer Party_FindOwnerWithActionFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02001bcc
	.thumb_func
Func_02001bcc:
	overlay_veneer EventRuntime_StartFar
	.global Func_02001bd4
	.thumb_func
Func_02001bd4:
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
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global ObjectMotion_CenterOnColumn
	.thumb_func
ObjectMotion_CenterOnColumn:
	overlay_veneer ObjectMotion_CenterOnColumnFar
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02001c2c
	.thumb_func
Func_02001c2c:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
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
	.global Func_02001c6c
	.thumb_func
Func_02001c6c:
	overlay_veneer EventRuntime_SetMessageFar
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_02001c7c
	.thumb_func
Func_02001c7c:
	overlay_veneer Func_080c8198
	.global Func_02001c84
	.thumb_func
Func_02001c84:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_02001c94
	.thumb_func
Func_02001c94:
	overlay_veneer ObjectMotion_ArmCallbackAndRefreshFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02001ca4
	.thumb_func
Func_02001ca4:
	overlay_veneer Func_080c8210
	.global Func_02001cac
	.thumb_func
Func_02001cac:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02001cc4
	.thumb_func
Func_02001cc4:
	overlay_veneer Func_080c8240
	.global Func_02001ccc
	.thumb_func
Func_02001ccc:
	overlay_veneer Event_SetDestinationFar
	.global Func_02001cd4
	.thumb_func
Func_02001cd4:
	overlay_veneer EventRuntime_SetCountdownFar
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
	.global Func_02001cf4
	.thumb_func
Func_02001cf4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8418
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8418
	.else
	overlay_veneer Object_ReturnHomeFar
	.endif
	.endif
	.global Func_02001cfc
	.thumb_func
Func_02001cfc:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8470
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8470
	.else
	overlay_veneer Func_080c8480
	.endif
	.endif
	.global Func_02001d04
	.thumb_func
Func_02001d04:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.endif
	.endif
	.global Func_02001d0c
	.thumb_func
Func_02001d0c:
	overlay_veneer Motion_LaunchFromFocusedObjectFar
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_02001d24
	.thumb_func
Func_02001d24:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8698
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8698
	.else
	overlay_veneer Func_080c86a8
	.endif
	.endif
	.global Func_02001d2c
	.thumb_func
Func_02001d2c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86a8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86a8
	.else
	overlay_veneer Func_080c86b8
	.endif
	.endif
	.global Func_02001d34
	.thumb_func
Func_02001d34:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86b0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86b0
	.else
	overlay_veneer Func_080c86c0
	.endif
	.endif
	.global Func_02001d3c
	.thumb_func
Func_02001d3c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86d8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86d8
	.else
	overlay_veneer Func_080c86e8
	.endif
	.endif
	.global Func_02001d44
	.thumb_func
Func_02001d44:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8750
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8750
	.else
	overlay_veneer Func_080c8760
	.endif
	.endif
	.global Func_02001d4c
	.thumb_func
Func_02001d4c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8770
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8770
	.else
	overlay_veneer Func_080c8780
	.endif
	.endif
	.global Func_02001d54
	.thumb_func
Func_02001d54:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_StepVector2Far
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_StepVector2Far
	.else
	overlay_veneer Func_080c87e8
	.endif
	.endif
	.global Func_02001d5c
	.thumb_func
Func_02001d5c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8828
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8828
	.else
	overlay_veneer Func_080c8838
	.endif
	.endif
	.global Func_02001d64
	.thumb_func
Func_02001d64:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8838
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8838
	.else
	overlay_veneer Func_080c8848
	.endif
	.endif
	.global Func_02001d6c
	.thumb_func
Func_02001d6c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8868
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8868
	.else
	overlay_veneer Func_080c8878
	.endif
	.endif
	.global Func_02001d74
	.thumb_func
Func_02001d74:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c88d0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c88d0
	.else
	overlay_veneer Func_080c88e0
	.endif
	.endif
	.global Func_02001d7c
	.thumb_func
Func_02001d7c:
	overlay_veneer Audio_PlayCue
