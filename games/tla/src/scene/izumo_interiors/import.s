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
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	overlay_veneer GameFlag_ClearBitFar
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
	.global Func_020021c4
	.thumb_func
Func_020021c4:
	overlay_veneer EventRuntime_BeginFar
	.global Func_020021cc
	.thumb_func
Func_020021cc:
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
	.global Func_02002214
	.thumb_func
Func_02002214:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global ObjectMotion_Launch
	.thumb_func
ObjectMotion_Launch:
	overlay_veneer ObjectMotion_LaunchFar
	.global ObjectMotion_SetVariantCallback
	.thumb_func
ObjectMotion_SetVariantCallback:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Object_LinkPair
	.thumb_func
Object_LinkPair:
	overlay_veneer Object_LinkPairFar
	.global Func_02002244
	.thumb_func
Func_02002244:
	overlay_veneer EventRuntime_SetMessageFar
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_02002254
	.thumb_func
Func_02002254:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_0200226c
	.thumb_func
Func_0200226c:
	overlay_veneer Func_080c8210
	.global Func_02002274
	.thumb_func
Func_02002274:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
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
	.global Func_02002294
	.thumb_func
Func_02002294:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8490
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8490
	.else
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.endif
	.endif
	.global Func_0200229c
	.thumb_func
Func_0200229c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Field_DispatchTypeHandlerFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Field_DispatchTypeHandlerFar
	.else
	overlay_veneer BattleEffect_SetupObjectPairFar
	.endif
	.endif
	.global Func_020022a4
	.thumb_func
Func_020022a4:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.else
	overlay_veneer EffectRuntime_StopCurrentObjectFar
	.endif
	.endif
	.global Func_020022ac
	.thumb_func
Func_020022ac:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_SetupObjectPairFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_SetupObjectPairFar
	.else
	overlay_veneer BattleEffect_CleanupSceneObjectsFar
	.endif
	.endif
	.global Func_020022b4
	.thumb_func
Func_020022b4:
	.ifdef TLA_EDITION_ES
	overlay_veneer EffectRuntime_StopCurrentObjectFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer EffectRuntime_StopCurrentObjectFar
	.else
	overlay_veneer EventObject_InitializeFar
	.endif
	.endif
	.global Func_020022bc
	.thumb_func
Func_020022bc:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ResolvePendingActionsFar
	.else
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.endif
	.endif
	.global Func_020022c4
	.thumb_func
Func_020022c4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Party_PlaySavedMusicFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Party_PlaySavedMusicFar
	.else
	overlay_veneer Func_080c8580
	.endif
	.endif
	.global Func_020022cc
	.thumb_func
Func_020022cc:
	overlay_veneer Motion_LaunchFromFocusedObjectFar
	.global ObjectMotion_OffsetPositionAndReset
	.thumb_func
ObjectMotion_OffsetPositionAndReset:
	overlay_veneer ObjectMotion_OffsetPositionAndResetFar
	.global Func_020022dc
	.thumb_func
Func_020022dc:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8768
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8768
	.else
	overlay_veneer EventRuntime_GetControlledOwnerFar
	.endif
	.endif
	.global Func_020022e4
	.thumb_func
Func_020022e4:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87f8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87f8
	.else
	overlay_veneer BattleEffect_StartArcEffectFar
	.endif
	.endif
	.global Func_020022ec
	.thumb_func
Func_020022ec:
	.ifdef TLA_EDITION_ES
	overlay_veneer Field_EndActorSpriteEffectFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Field_EndActorSpriteEffectFar
	.else
	overlay_veneer BattleEffect_EndArcEffectFar
	.endif
	.endif
	.global Func_020022f4
	.thumb_func
Func_020022f4:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_StartArcEffectFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_StartArcEffectFar
	.else
	overlay_veneer BattleEffect_WaitForArcGroupsFar
	.endif
	.endif
	.global Func_020022fc
	.thumb_func
Func_020022fc:
	.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_EndArcEffectFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_EndArcEffectFar
	.else
	overlay_veneer BattleEffect_SetObjectTickModeFar
	.endif
	.endif
	.global Func_02002304
	.thumb_func
Func_02002304:
	overlay_veneer Func_08108008
	.global Func_0200230c
	.thumb_func
Func_0200230c:
	overlay_veneer Func_08108010
	.global Func_02002314
	.thumb_func
Func_02002314:
	overlay_veneer Func_08108018
	.global Func_0200231c
	.thumb_func
Func_0200231c:
	overlay_veneer Audio_PlayCue
