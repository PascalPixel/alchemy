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
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
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
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetSpeedParameters
	.thumb_func
Actor_SetSpeedParameters:
	overlay_veneer Actor_SetSpeedParametersFar
	.global Actor_ResetAndSetPosition
	.thumb_func
Actor_ResetAndSetPosition:
	overlay_veneer Actor_ResetAndSetPositionFar
	.global Actor_ResetAndSetPositionInMode2
	.thumb_func
Actor_ResetAndSetPositionInMode2:
	overlay_veneer Actor_ResetAndSetPositionInMode2Far
	.global Actor_SetPositionAndReset
	.thumb_func
Actor_SetPositionAndReset:
	overlay_veneer Actor_SetPositionAndResetFar
	.global Actor_OffsetPositionAndResetMotion
	.thumb_func
Actor_OffsetPositionAndResetMotion:
	overlay_veneer Actor_OffsetPositionAndResetMotionFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_02002214
	.thumb_func
Func_02002214:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_Jump
	.thumb_func
Actor_Jump:
	overlay_veneer Actor_JumpFar
	.global Actor_StartRepeatedMotion
	.thumb_func
Actor_StartRepeatedMotion:
	overlay_veneer Actor_StartRepeatedMotionFar
	.global Actor_SetAngleToward
	.thumb_func
Actor_SetAngleToward:
	overlay_veneer Actor_SetAngleTowardFar
	.global Actor_TurnToFaceEachOther
	.thumb_func
Actor_TurnToFaceEachOther:
	overlay_veneer Actor_TurnToFaceEachOtherFar
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
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
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
	overlay_veneer BattleEffect_StopMainActorFar
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
	.global Actor_OffsetPositionAndReset
	.thumb_func
Actor_OffsetPositionAndReset:
	overlay_veneer Actor_OffsetPositionAndResetFar
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
