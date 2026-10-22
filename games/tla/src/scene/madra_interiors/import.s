.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global Func_02001d20
	.thumb_func
Func_02001d20:
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
	.global Func_02001d28
	.thumb_func
Func_02001d28:
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
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Func_02001d38
	.thumb_func
Func_02001d38:
	overlay_veneer Func_08020300
	.global PartyInventory_Remove
	.thumb_func
PartyInventory_Remove:
	overlay_veneer PartyInventory_RemoveFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02001d50
	.thumb_func
Func_02001d50:
	overlay_veneer EventRuntime_StartFar
	.global Func_02001d58
	.thumb_func
Func_02001d58:
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
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
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
	.global Func_02001db8
	.thumb_func
Func_02001db8:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_02001dc0
	.thumb_func
Func_02001dc0:
	overlay_veneer ObjectMotion_SetHorizontalPositionAndAngleWithTerrainFar
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
	.global Func_02001e08
	.thumb_func
Func_02001e08:
	overlay_veneer ObjectGroup_ConfigureChildValueFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02001e18
	.thumb_func
Func_02001e18:
	overlay_veneer EventRuntime_SetMessageFar
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_02001e28
	.thumb_func
Func_02001e28:
	overlay_veneer Func_080c8198
	.global Func_02001e30
	.thumb_func
Func_02001e30:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_02001e40
	.thumb_func
Func_02001e40:
	overlay_veneer ObjectMotion_ArmCallbackAndRefreshFar
	.global Func_02001e48
	.thumb_func
Func_02001e48:
	overlay_veneer Func_080c8210
	.global Func_02001e50
	.thumb_func
Func_02001e50:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02001e68
	.thumb_func
Func_02001e68:
	overlay_veneer Func_080c8240
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
	.global Func_02001e80
	.thumb_func
Func_02001e80:
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
	.global Func_02001e90
	.thumb_func
Func_02001e90:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8490
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8490
	.else
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.endif
	.endif
	.global Func_02001e98
	.thumb_func
Func_02001e98:
	.ifdef TLA_EDITION_ES
	overlay_veneer Field_DispatchTypeHandlerFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Field_DispatchTypeHandlerFar
	.else
	overlay_veneer BattleEffect_SetupObjectPairFar
	.endif
	.endif
	.global Func_02001ea0
	.thumb_func
Func_02001ea0:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.else
	overlay_veneer EffectRuntime_StopCurrentObjectFar
	.endif
	.endif
	.global Func_02001ea8
	.thumb_func
Func_02001ea8:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_SetupObjectPairFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_SetupObjectPairFar
	.else
	overlay_veneer BattleEffect_CleanupSceneObjectsFar
	.endif
	.endif
	.global Func_02001eb0
	.thumb_func
Func_02001eb0:
	.ifdef TLA_EDITION_ES
	overlay_veneer EffectRuntime_StopCurrentObjectFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer EffectRuntime_StopCurrentObjectFar
	.else
	overlay_veneer EventObject_InitializeFar
	.endif
	.endif
	.global Func_02001eb8
	.thumb_func
Func_02001eb8:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.endif
	.endif
	.global Func_02001ec0
	.thumb_func
Func_02001ec0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Party_PlaySavedMusicFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Party_PlaySavedMusicFar
	.else
	overlay_veneer Func_080c8580
	.endif
	.endif
	.global Func_02001ec8
	.thumb_func
Func_02001ec8:
	overlay_veneer Motion_LaunchFromFocusedObjectFar
	.global ObjectMotion_OffsetPositionAndReset
	.thumb_func
ObjectMotion_OffsetPositionAndReset:
	overlay_veneer ObjectMotion_OffsetPositionAndResetFar
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_02001ee8
	.thumb_func
Func_02001ee8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8768
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8768
	.else
	overlay_veneer EventRuntime_GetControlledOwnerFar
	.endif
	.endif
	.global Func_02001ef0
	.thumb_func
Func_02001ef0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8798
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8798
	.else
	overlay_veneer Func_080c87a8
	.endif
	.endif
	.global Func_02001ef8
	.thumb_func
Func_02001ef8:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8920
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8920
	.else
	overlay_veneer Func_080c8930
	.endif
	.endif
	.global Func_02001f00
	.thumb_func
Func_02001f00:
	overlay_veneer Func_08108010
	.global Func_02001f08
	.thumb_func
Func_02001f08:
	overlay_veneer Func_08108018
	.global Func_02001f10
	.thumb_func
Func_02001f10:
	overlay_veneer Audio_PlayCue
