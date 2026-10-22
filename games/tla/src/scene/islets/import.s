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
	.global Scheduler_RemoveCallbackFar
	.thumb_func
Scheduler_RemoveCallbackFar:
	overlay_veneer Func_080000d8
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global Runtime_AllocateBlock
	.thumb_func
Runtime_AllocateBlock:
	overlay_veneer Runtime_AllocateBlockFar
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
	.global Func_02003de4
	.thumb_func
Func_02003de4:
	overlay_veneer Object_SetMode
	.global Func_02003dec
	.thumb_func
Func_02003dec:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02003df4
	.thumb_func
Func_02003df4:
	overlay_veneer Func_080200c0
	.global Func_02003dfc
	.thumb_func
Func_02003dfc:
	overlay_veneer Object_Destroy
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02003e0c
	.thumb_func
Func_02003e0c:
	overlay_veneer Object_SetPosition
	.global Func_02003e14
	.thumb_func
Func_02003e14:
	overlay_veneer Object_CommitPosition
	.global Func_02003e1c
	.thumb_func
Func_02003e1c:
	overlay_veneer Map_PlayMetatileCopySequenceFar
	.global Func_02003e24
	.thumb_func
Func_02003e24:
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
	.global Func_02003e2c
	.thumb_func
Func_02003e2c:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Func_02003e3c
	.thumb_func
Func_02003e3c:
	overlay_veneer Func_08020300
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global PartyInventory_FindOwner
	.thumb_func
PartyInventory_FindOwner:
	overlay_veneer PartyInventory_FindOwnerFar
	.global PartyInventory_Remove
	.thumb_func
PartyInventory_Remove:
	overlay_veneer PartyInventory_RemoveFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02003e64
	.thumb_func
Func_02003e64:
	overlay_veneer EventRuntime_StartFar
	.global Func_02003e6c
	.thumb_func
Func_02003e6c:
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
	.global Func_02003edc
	.thumb_func
Func_02003edc:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_02003ee4
	.thumb_func
Func_02003ee4:
	overlay_veneer ObjectMotion_SetHorizontalPositionAndAngleWithTerrainFar
	.global Func_02003eec
	.thumb_func
Func_02003eec:
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
	.global Func_02003f0c
	.thumb_func
Func_02003f0c:
	overlay_veneer BattleEffect_RunRisingObjectSequenceFar
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02003f24
	.thumb_func
Func_02003f24:
	overlay_veneer EventRuntime_SetMessageFar
	.global Func_02003f2c
	.thumb_func
Func_02003f2c:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_02003f3c
	.thumb_func
Func_02003f3c:
	overlay_veneer ObjectMotion_ArmCallbackAndRefreshFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02003f4c
	.thumb_func
Func_02003f4c:
	overlay_veneer Func_080c8208
	.global Func_02003f54
	.thumb_func
Func_02003f54:
	overlay_veneer Func_080c8210
	.global Func_02003f5c
	.thumb_func
Func_02003f5c:
	overlay_veneer Func_080c8218
	.global Func_02003f64
	.thumb_func
Func_02003f64:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02003f74
	.thumb_func
Func_02003f74:
	overlay_veneer Func_080c8240
	.global Func_02003f7c
	.thumb_func
Func_02003f7c:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Func_02003f84
	.thumb_func
Func_02003f84:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c82d0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c82d0
	.else
	overlay_veneer Func_080c82e0
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
	.global Func_02003fa4
	.thumb_func
Func_02003fa4:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.endif
	.endif
	.global Func_02003fac
	.thumb_func
Func_02003fac:
	.ifdef TLA_EDITION_ES
	overlay_veneer EventRuntime_ResolveAllPendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer EventRuntime_ResolveAllPendingActionsFar
	.else
	overlay_veneer Func_080c84e8
	.endif
	.endif
	.global Func_02003fb4
	.thumb_func
Func_02003fb4:
	.ifdef TLA_EDITION_ES
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.else
	overlay_veneer Func_080c84f0
	.endif
	.endif
	.global Func_02003fbc
	.thumb_func
Func_02003fbc:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c84e8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c84e8
	.else
	overlay_veneer Func_080c84f8
	.endif
	.endif
	.global Func_02003fc4
	.thumb_func
Func_02003fc4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c84f0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c84f0
	.else
	overlay_veneer Func_080c8500
	.endif
	.endif
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_02003fd4
	.thumb_func
Func_02003fd4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Object_SetActionCallbackFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Object_SetActionCallbackFar
	.else
	overlay_veneer Event_NoOpFar
	.endif
	.endif
	.global Func_02003fdc
	.thumb_func
Func_02003fdc:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8728
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8728
	.else
	overlay_veneer Func_080c8738
	.endif
	.endif
	.global Func_02003fe4
	.thumb_func
Func_02003fe4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8738
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8738
	.else
	overlay_veneer Func_080c8748
	.endif
	.endif
	.ifndef TLA_EDITION_JA
	.global Func_02003fec
	.thumb_func
Func_02003fec:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8750
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8750
	.else
	overlay_veneer Func_080c8760
	.endif
	.endif
	.endif
	.global Func_02003ff4
	.thumb_func
Func_02003ff4:
	overlay_veneer Func_08108010
	.global Func_02003ffc
	.thumb_func
Func_02003ffc:
	overlay_veneer Audio_PlayCue
