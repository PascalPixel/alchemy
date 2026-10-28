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
	.global wait
	.thumb_func
wait:
	overlay_veneer waitFar
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
	.global Task_RemoveFar
	.thumb_func
Task_RemoveFar:
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
	.global HeapSlot_AllocFar
	.thumb_func
HeapSlot_AllocFar:
	overlay_veneer Func_08000140
	.global HeapSlot_Release
	.thumb_func
HeapSlot_Release:
	overlay_veneer HeapSlot_ReleaseFar
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
	overlay_veneer Actor_SetMode
	.global Func_02005138
	.thumb_func
Func_02005138:
	overlay_veneer Actor_StartScriptFar
	.global Func_02005140
	.thumb_func
Func_02005140:
	overlay_veneer Func_080200c0
	.global Func_02005148
	.thumb_func
Func_02005148:
	overlay_veneer Actor_Destroy
	.global Actor_SetPositionAndResetMotion
	.thumb_func
Actor_SetPositionAndResetMotion:
	overlay_veneer Actor_SetPositionAndResetMotionFar
	.global Func_02005158
	.thumb_func
Func_02005158:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02005160
	.thumb_func
Func_02005160:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02005168
	.thumb_func
Func_02005168:
	overlay_veneer Actor_CommitPosition
	.global Func_02005170
	.thumb_func
Func_02005170:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Func_02005178
	.thumb_func
Func_02005178:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02005180
	.thumb_func
Func_02005180:
	overlay_veneer Func_08020210
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
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
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global Func_020051b0
	.thumb_func
Func_020051b0:
	overlay_veneer Func_08038248
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_020051c0
	.thumb_func
Func_020051c0:
	overlay_veneer Event_StartFar
	.global Func_020051c8
	.thumb_func
Func_020051c8:
	overlay_veneer Event_EndFar
	.global PartyInventory_GiveItem
	.thumb_func
PartyInventory_GiveItem:
	overlay_veneer PartyInventory_GiveItemFar
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
	.global Actor_SetMovementTypeById
	.thumb_func
Actor_SetMovementTypeById:
	overlay_veneer Actor_SetMovementTypeByIdFar
	.global Actor_Stop
	.thumb_func
Actor_Stop:
	overlay_veneer Actor_StopFar
	.global Actor_SetMovementTypeAndWaitById
	.thumb_func
Actor_SetMovementTypeAndWaitById:
	overlay_veneer Actor_SetMovementTypeAndWaitByIdFar
	.global Actor_ResetAndSetPosition
	.thumb_func
Actor_ResetAndSetPosition:
	overlay_veneer Actor_ResetAndSetPositionFar
	.global Actor_SetPositionAndCommit
	.thumb_func
Actor_SetPositionAndCommit:
	overlay_veneer Actor_SetPositionAndCommitFar
	.global Actor_ResetAndSetPositionInMode2
	.thumb_func
Actor_ResetAndSetPositionInMode2:
	overlay_veneer Actor_ResetAndSetPositionInMode2Far
	.global Actor_SetPositionAndReset
	.thumb_func
Actor_SetPositionAndReset:
	overlay_veneer Actor_SetPositionAndResetFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_02005230
	.thumb_func
Func_02005230:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Func_02005238
	.thumb_func
Func_02005238:
	overlay_veneer Actor_SetHorizontalPositionAndAngleWithTerrainFar
	.global Func_02005240
	.thumb_func
Func_02005240:
	overlay_veneer Actor_MoveToObjectFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global Actor_Jump
	.thumb_func
Actor_Jump:
	overlay_veneer Actor_JumpFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Actor_SetAngleToward
	.thumb_func
Actor_SetAngleToward:
	overlay_veneer Actor_SetAngleTowardFar
	.global Func_02005270
	.thumb_func
Func_02005270:
	overlay_veneer Actor_ConfigureSpriteColorFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_02005280
	.thumb_func
Func_02005280:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_02005290
	.thumb_func
Func_02005290:
	overlay_veneer Func_080c8198
	.global Func_02005298
	.thumb_func
Func_02005298:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Func_020052a0
	.thumb_func
Func_020052a0:
	overlay_veneer Func_080c81a8
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Func_020052b0
	.thumb_func
Func_020052b0:
	overlay_veneer Actor_ArmCallbackAndRefreshFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
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
	overlay_veneer Event_SetExitRequestFar
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
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_FinishScreenTransition
	.thumb_func
Event_FinishScreenTransition:
	overlay_veneer Event_FinishScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Func_02005330
	.thumb_func
Func_02005330:
	overlay_veneer Func_080c83d8
	.global Func_02005338
	.thumb_func
Func_02005338:
	overlay_veneer Func_080c8480
	.global Field_DispatchTypeHandler
	.thumb_func
Field_DispatchTypeHandler:
	overlay_veneer Field_DispatchTypeHandlerFar
	.global Func_02005348
	.thumb_func
Func_02005348:
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.global Func_02005350
	.thumb_func
Func_02005350:
	overlay_veneer BattleEffect_SetupObjectPairFar
	.global Func_02005358
	.thumb_func
Func_02005358:
	overlay_veneer BattleEffect_StopMainActorFar
	.global Func_02005360
	.thumb_func
Func_02005360:
	overlay_veneer BattleEffect_CleanupSceneObjectsFar
	.global Func_02005368
	.thumb_func
Func_02005368:
	overlay_veneer EventObject_InitializeFar
	.global Func_02005370
	.thumb_func
Func_02005370:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02005378
	.thumb_func
Func_02005378:
	overlay_veneer Motion_LaunchFromFocusedObjectFar
	.global Actor_WatchAndSetScript
	.thumb_func
Actor_WatchAndSetScript:
	overlay_veneer Actor_WatchAndSetScriptFar
	.global Func_02005388
	.thumb_func
Func_02005388:
	overlay_veneer Func_080c8688
	.global Func_02005390
	.thumb_func
Func_02005390:
	overlay_veneer Func_080c86a8
	.global Func_02005398
	.thumb_func
Func_02005398:
	overlay_veneer Func_080c86d0
	.global Func_020053a0
	.thumb_func
Func_020053a0:
	overlay_veneer Func_080c86d8
	.global Func_020053a8
	.thumb_func
Func_020053a8:
	overlay_veneer Func_080c86e8
	.global Func_020053b0
	.thumb_func
Func_020053b0:
	overlay_veneer Func_080c8710
	.global Func_020053b8
	.thumb_func
Func_020053b8:
	overlay_veneer Func_080c8718
	.global Func_020053c0
	.thumb_func
Func_020053c0:
	overlay_veneer Func_080c8720
	.global Func_020053c8
	.thumb_func
Func_020053c8:
	overlay_veneer Func_080c8728
	.global Func_020053d0
	.thumb_func
Func_020053d0:
	overlay_veneer Func_080c8760
	.global Func_020053d8
	.thumb_func
Func_020053d8:
	overlay_veneer Sound_PlayFar
