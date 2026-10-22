.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
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
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.global Math_Cosine
	.thumb_func
Math_Cosine:
	overlay_veneer Math_CosineFar
	.global Runtime_AllocateHeapBlockFar
	.thumb_func
Runtime_AllocateHeapBlockFar:
	overlay_veneer Func_08000140
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	overlay_veneer Runtime_ReleaseHeapBlockFar
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
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
	.global Func_02005130
	.thumb_func
Func_02005130:
	overlay_veneer Object_SetMode
	.global Func_02005138
	.thumb_func
Func_02005138:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02005140
	.thumb_func
Func_02005140:
	overlay_veneer Func_080200c0
	.global Func_02005148
	.thumb_func
Func_02005148:
	overlay_veneer Object_Destroy
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02005158
	.thumb_func
Func_02005158:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02005160
	.thumb_func
Func_02005160:
	overlay_veneer Object_SetPosition
	.global Func_02005168
	.thumb_func
Func_02005168:
	overlay_veneer Object_CommitPosition
	.global Func_02005170
	.thumb_func
Func_02005170:
	overlay_veneer Map_CopyMetatileIndicesRectFar
	.global Func_02005178
	.thumb_func
Func_02005178:
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
	.global Func_02005180
	.thumb_func
Func_02005180:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02005198
	.thumb_func
Func_02005198:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_020051a0
	.thumb_func
Func_020051a0:
	overlay_veneer Func_08020230
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Func_020051b0
	.thumb_func
Func_020051b0:
	overlay_veneer Func_08038248
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_020051c0
	.thumb_func
Func_020051c0:
	overlay_veneer EventRuntime_BeginFar
	.global Func_020051c8
	.thumb_func
Func_020051c8:
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
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02005230
	.thumb_func
Func_02005230:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_02005238
	.thumb_func
Func_02005238:
	overlay_veneer ObjectMotion_SetHorizontalPositionAndAngleWithTerrainFar
	.global Func_02005240
	.thumb_func
Func_02005240:
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
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Func_02005270
	.thumb_func
Func_02005270:
	overlay_veneer ObjectGroup_ConfigureChildValueFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02005280
	.thumb_func
Func_02005280:
	overlay_veneer EventRuntime_SetMessageFar
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_02005290
	.thumb_func
Func_02005290:
	overlay_veneer Func_080c8198
	.global Func_02005298
	.thumb_func
Func_02005298:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global Func_020052a0
	.thumb_func
Func_020052a0:
	overlay_veneer Func_080c81a8
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_020052b0
	.thumb_func
Func_020052b0:
	overlay_veneer ObjectMotion_ArmCallbackAndRefreshFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_020052c0
	.thumb_func
Func_020052c0:
	overlay_veneer Func_080c8208
	.global Func_020052c8
	.thumb_func
Func_020052c8:
	overlay_veneer Func_080c8210
	.global Func_020052d0
	.thumb_func
Func_020052d0:
	overlay_veneer Func_080c8218
	.global Func_020052d8
	.thumb_func
Func_020052d8:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_020052e8
	.thumb_func
Func_020052e8:
	overlay_veneer Func_080c8240
	.global Func_020052f0
	.thumb_func
Func_020052f0:
	overlay_veneer Func_080c8258
	.global Func_020052f8
	.thumb_func
Func_020052f8:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Func_02005300
	.thumb_func
Func_02005300:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Func_02005308
	.thumb_func
Func_02005308:
	overlay_veneer Func_080c8288
	.global Party_SetFields1eeAnd1f0
	.thumb_func
Party_SetFields1eeAnd1f0:
	overlay_veneer Party_SetFields1eeAnd1f0Far
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
	.global Func_02005330
	.thumb_func
Func_02005330:
	.ifdef TLA_EDITION_ES
	overlay_veneer DisplayScroll_DisarmHBlankDmaFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer DisplayScroll_DisarmHBlankDmaFar
	.else
	overlay_veneer Func_080c83d8
	.endif
	.endif
	.global Func_02005338
	.thumb_func
Func_02005338:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8470
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8470
	.else
	overlay_veneer Func_080c8480
	.endif
	.endif
	.global Field_DispatchTypeHandler
	.thumb_func
Field_DispatchTypeHandler:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8488
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8488
	.else
	overlay_veneer Field_DispatchTypeHandlerFar
	.endif
	.endif
	.global Func_02005348
	.thumb_func
Func_02005348:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8490
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8490
	.else
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.endif
	.endif
	.global Func_02005350
	.thumb_func
Func_02005350:
	.ifdef TLA_EDITION_ES
	overlay_veneer Field_DispatchTypeHandlerFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Field_DispatchTypeHandlerFar
	.else
	overlay_veneer BattleEffect_SetupObjectPairFar
	.endif
	.endif
	.global Func_02005358
	.thumb_func
Func_02005358:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.else
	overlay_veneer EffectRuntime_StopCurrentObjectFar
	.endif
	.endif
	.global Func_02005360
	.thumb_func
Func_02005360:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_SetupObjectPairFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_SetupObjectPairFar
	.else
	overlay_veneer BattleEffect_CleanupSceneObjectsFar
	.endif
	.endif
	.global Func_02005368
	.thumb_func
Func_02005368:
	.ifdef TLA_EDITION_ES
	overlay_veneer EffectRuntime_StopCurrentObjectFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer EffectRuntime_StopCurrentObjectFar
	.else
	overlay_veneer EventObject_InitializeFar
	.endif
	.endif
	.global Func_02005370
	.thumb_func
Func_02005370:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.endif
	.endif
	.global Func_02005378
	.thumb_func
Func_02005378:
	overlay_veneer Motion_LaunchFromFocusedObjectFar
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_02005388
	.thumb_func
Func_02005388:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8678
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8678
	.else
	overlay_veneer Func_080c8688
	.endif
	.endif
	.global Func_02005390
	.thumb_func
Func_02005390:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8698
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8698
	.else
	overlay_veneer Func_080c86a8
	.endif
	.endif
	.global Func_02005398
	.thumb_func
Func_02005398:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86c0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86c0
	.else
	overlay_veneer Func_080c86d0
	.endif
	.endif
	.global Func_020053a0
	.thumb_func
Func_020053a0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86c8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86c8
	.else
	overlay_veneer Func_080c86d8
	.endif
	.endif
	.global Func_020053a8
	.thumb_func
Func_020053a8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86d8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86d8
	.else
	overlay_veneer Func_080c86e8
	.endif
	.endif
	.global Func_020053b0
	.thumb_func
Func_020053b0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8700
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8700
	.else
	overlay_veneer Func_080c8710
	.endif
	.endif
	.global Func_020053b8
	.thumb_func
Func_020053b8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8708
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8708
	.else
	overlay_veneer Func_080c8718
	.endif
	.endif
	.global Func_020053c0
	.thumb_func
Func_020053c0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8710
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8710
	.else
	overlay_veneer Func_080c8720
	.endif
	.endif
	.global Func_020053c8
	.thumb_func
Func_020053c8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8718
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8718
	.else
	overlay_veneer Func_080c8728
	.endif
	.endif
	.global Func_020053d0
	.thumb_func
Func_020053d0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8750
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8750
	.else
	overlay_veneer Func_080c8760
	.endif
	.endif
	.global Func_020053d8
	.thumb_func
Func_020053d8:
	overlay_veneer Audio_PlayCue
