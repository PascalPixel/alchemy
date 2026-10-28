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
	overlay_veneer Event_StartFar
	.global Func_020021cc
	.thumb_func
Func_020021cc:
	overlay_veneer Event_EndFar
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
	.global Object_TurnToFaceEachOther
	.thumb_func
Object_TurnToFaceEachOther:
	overlay_veneer Object_LinkPairFar
	.global Func_02002244
	.thumb_func
Func_02002244:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_02002254
	.thumb_func
Func_02002254:
	overlay_veneer Event_ShowMessageAndWaitFar
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
	overlay_veneer Field_DispatchTypeHandlerFar
	.global Func_02002294
	.thumb_func
Func_02002294:
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.global Func_0200229c
	.thumb_func
Func_0200229c:
	overlay_veneer BattleEffect_SetupObjectPairFar
	.global Func_020022a4
	.thumb_func
Func_020022a4:
	overlay_veneer EffectRuntime_StopCurrentObjectFar
	.global Func_020022ac
	.thumb_func
Func_020022ac:
	overlay_veneer BattleEffect_CleanupSceneObjectsFar
	.global Func_020022b4
	.thumb_func
Func_020022b4:
	overlay_veneer EventObject_InitializeFar
	.global Func_020022bc
	.thumb_func
Func_020022bc:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_020022c4
	.thumb_func
Func_020022c4:
	overlay_veneer Func_080c8580
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
	overlay_veneer Party_GetLeaderActorFar
	.global Func_020022e4
	.thumb_func
Func_020022e4:
	overlay_veneer BattleEffect_StartArcEffectFar
	.global Func_020022ec
	.thumb_func
Func_020022ec:
	overlay_veneer BattleEffect_EndArcEffectFar
	.global Func_020022f4
	.thumb_func
Func_020022f4:
	overlay_veneer BattleEffect_WaitForArcGroupsFar
	.global Func_020022fc
	.thumb_func
Func_020022fc:
	overlay_veneer BattleEffect_SetObjectUpdateModeFar
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
	overlay_veneer Sound_PlayFar
