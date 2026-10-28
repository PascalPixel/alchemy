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
	.global wait
	.thumb_func
wait:
	overlay_veneer waitFar
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
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
	.global Heap_AllocEwram
	.thumb_func
Heap_AllocEwram:
	overlay_veneer Heap_AllocEwramFar
	.global Heap_Release
	.thumb_func
Heap_Release:
	overlay_veneer Sys_FreeFar
	.global Unit_Get
	.thumb_func
Unit_Get:
	overlay_veneer Unit_GetFar
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
	overlay_veneer Actor_SetMode
	.global Actor_SetAnimationSpeed
	.thumb_func
Actor_SetAnimationSpeed:
	overlay_veneer Actor_SetAnimationSpeedFar
	.global Func_02004dac
	.thumb_func
Func_02004dac:
	overlay_veneer Actor_StartScriptFar
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Func_080200c0
	.global Func_02004dbc
	.thumb_func
Func_02004dbc:
	overlay_veneer Actor_Destroy
	.global Func_02004dc4
	.thumb_func
Func_02004dc4:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02004dcc
	.thumb_func
Func_02004dcc:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02004dd4
	.thumb_func
Func_02004dd4:
	overlay_veneer DisplayBlend_DisableRunScriptFar
	.global Func_02004ddc
	.thumb_func
Func_02004ddc:
	overlay_veneer DisplayBlend_EnableRunScriptFar
	.global Func_02004de4
	.thumb_func
Func_02004de4:
	overlay_veneer DisplayBlend_StartScriptFar
	.global Func_02004dec
	.thumb_func
Func_02004dec:
	overlay_veneer GetMapCellCollisionFar
	.global Func_02004df4
	.thumb_func
Func_02004df4:
	overlay_veneer Map_CopyCellsRectFar
	.global Func_02004dfc
	.thumb_func
Func_02004dfc:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02004e04
	.thumb_func
Func_02004e04:
	overlay_veneer Func_080201f0
	.global Func_02004e0c
	.thumb_func
Func_02004e0c:
	overlay_veneer Func_080201f8
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
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
	overlay_veneer Actor_SetSpritePriorityDirectFar
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global PartyInventory_FindOwner
	.thumb_func
PartyInventory_FindOwner:
	overlay_veneer PartyInventory_FindOwnerFar
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_02004e5c
	.thumb_func
Func_02004e5c:
	overlay_veneer Event_StartFar
	.global Func_02004e64
	.thumb_func
Func_02004e64:
	overlay_veneer Event_EndFar
	.global Party_LearnAbility
	.thumb_func
Party_LearnAbility:
	overlay_veneer Party_LearnAbilityFar
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
	.global Actor_RefreshSelectorById
	.thumb_func
Actor_RefreshSelectorById:
	overlay_veneer Actor_RefreshSelectorByIdFar
	.global Actor_Stop
	.thumb_func
Actor_Stop:
	overlay_veneer Actor_StopFar
	.global Actor_ResetAndSetPosition
	.thumb_func
Actor_ResetAndSetPosition:
	overlay_veneer Actor_ResetAndSetPositionFar
	.global Actor_ResetAndSetPositionInMode2
	.thumb_func
Actor_ResetAndSetPositionInMode2:
	overlay_veneer Actor_ResetAndSetPositionInMode2Far
	.global Actor_OffsetPositionAndResetMotion
	.thumb_func
Actor_OffsetPositionAndResetMotion:
	overlay_veneer Actor_OffsetPositionAndResetMotionFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_02004ebc
	.thumb_func
Func_02004ebc:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Func_02004ec4
	.thumb_func
Func_02004ec4:
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
	.global Actor_StartRepeatedMotion
	.thumb_func
Actor_StartRepeatedMotion:
	overlay_veneer Actor_StartRepeatedMotionFar
	.global Func_02004eec
	.thumb_func
Func_02004eec:
	overlay_veneer Actor_JumpDownLedgeFar
	.global Actor_SetAngleToward
	.thumb_func
Actor_SetAngleToward:
	overlay_veneer Actor_SetAngleTowardFar
	.global Func_02004efc
	.thumb_func
Func_02004efc:
	overlay_veneer Actor_ConfigureSpriteColorFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_02004f0c
	.thumb_func
Func_02004f0c:
	overlay_veneer Event_SetMessageIdFar
	.global Func_02004f14
	.thumb_func
Func_02004f14:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
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
	overlay_veneer Event_SetExitRequestFar
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
	overlay_veneer Func_080c8368
	.global Func_02004f94
	.thumb_func
Func_02004f94:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_02004f9c
	.thumb_func
Func_02004f9c:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global Func_02004fa4
	.thumb_func
Func_02004fa4:
	overlay_veneer BattleEffect_StartBufferInterpolationFar
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
	.global Func_02004fc4
	.thumb_func
Func_02004fc4:
	overlay_veneer Func_080c8480
	.global Func_02004fcc
	.thumb_func
Func_02004fcc:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02004fd4
	.thumb_func
Func_02004fd4:
	overlay_veneer Party_PlaySavedMusicFar
	.global Func_02004fdc
	.thumb_func
Func_02004fdc:
	overlay_veneer ObjectTable_SnapshotFar
	.global Func_02004fe4
	.thumb_func
Func_02004fe4:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_02004fec
	.thumb_func
Func_02004fec:
	overlay_veneer Func_080c8780
	.global Func_02004ff4
	.thumb_func
Func_02004ff4:
	overlay_veneer Func_080c88d0
	.global Actor_ResetTargetsAndVelocity
	.thumb_func
Actor_ResetTargetsAndVelocity:
	overlay_veneer Actor_ResetTargetsAndVelocityFar
	.ifndef TLA_EDITION_JA
	.global Func_02005004
	.thumb_func
Func_02005004:
	overlay_veneer Func_080c8940
	.endif
	.global Func_0200500c
	.thumb_func
Func_0200500c:
	overlay_veneer Func_080c8948
	.global Func_02005014
	.thumb_func
Func_02005014:
	overlay_veneer Sound_PlayFar
