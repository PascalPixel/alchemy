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
	.global Task_RemoveFar
	.thumb_func
Task_RemoveFar:
	overlay_veneer Func_080000d8
	.global Func_02001bf4
	.thumb_func
Func_02001bf4:
	overlay_veneer Task_SuspendAllFar
	.global Func_02001bfc
	.thumb_func
Func_02001bfc:
	overlay_veneer Task_ResumeAllFar
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global Func_02001c0c
	.thumb_func
Func_02001c0c:
	overlay_veneer Irq_SetHandlerFar
	.global HeapSlot_AllocEwram
	.thumb_func
HeapSlot_AllocEwram:
	overlay_veneer HeapSlot_AllocEwramFar
	.global HeapSlot_Release
	.thumb_func
HeapSlot_Release:
	overlay_veneer HeapSlot_ReleaseFar
	.global Func_02001c24
	.thumb_func
Func_02001c24:
	overlay_veneer Resource_DecodeByteLzInRamFar
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02001c3c
	.thumb_func
Func_02001c3c:
	overlay_veneer Oam_AddSpriteAtDepthFar
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
	.global ResourceMetadata_Register
	.thumb_func
ResourceMetadata_Register:
	overlay_veneer ResourceMetadata_RegisterFar
	.global Func_02001c6c
	.thumb_func
Func_02001c6c:
	overlay_veneer Actor_SetMode
	.global Func_02001c74
	.thumb_func
Func_02001c74:
	overlay_veneer Actor_StartScriptFar
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Func_080200c0
	.global Func_02001c84
	.thumb_func
Func_02001c84:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02001c8c
	.thumb_func
Func_02001c8c:
	overlay_veneer Actor_CommitPosition
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02001c9c
	.thumb_func
Func_02001c9c:
	overlay_veneer Func_08020210
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_02001cc4
	.thumb_func
Func_02001cc4:
	overlay_veneer Event_StartFar
	.global Func_02001ccc
	.thumb_func
Func_02001ccc:
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
	.global Actor_CenterOnColumn
	.thumb_func
Actor_CenterOnColumn:
	overlay_veneer Actor_CenterOnColumnFar
	.global Actor_OffsetPositionAndResetMotion
	.thumb_func
Actor_OffsetPositionAndResetMotion:
	overlay_veneer Actor_OffsetPositionAndResetMotionFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_02001d04
	.thumb_func
Func_02001d04:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
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
	.global Func_02001d2c
	.thumb_func
Func_02001d2c:
	overlay_veneer Actor_JumpDownLedgeFar
	.global Actor_SetAngleToward
	.thumb_func
Actor_SetAngleToward:
	overlay_veneer Actor_SetAngleTowardFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_02001d44
	.thumb_func
Func_02001d44:
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_02001d54
	.thumb_func
Func_02001d54:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_02001d6c
	.thumb_func
Func_02001d6c:
	overlay_veneer Func_080c8210
	.global Func_02001d74
	.thumb_func
Func_02001d74:
	overlay_veneer Func_080c8218
	.global Func_02001d7c
	.thumb_func
Func_02001d7c:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02001d8c
	.thumb_func
Func_02001d8c:
	overlay_veneer Func_080c8240
	.global Func_02001d94
	.thumb_func
Func_02001d94:
	overlay_veneer Event_SetExitRequestFar
	.global Func_02001d9c
	.thumb_func
Func_02001d9c:
	overlay_veneer Func_080c82e0
	.global Func_02001da4
	.thumb_func
Func_02001da4:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_02001dac
	.thumb_func
Func_02001dac:
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
	.global Func_02001dcc
	.thumb_func
Func_02001dcc:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02001dd4
	.thumb_func
Func_02001dd4:
	overlay_veneer Func_080c8580
	.global Actor_CommitPositionAndActivate
	.thumb_func
Actor_CommitPositionAndActivate:
	overlay_veneer Actor_CommitPositionAndActivateFar
	.global Func_02001de4
	.thumb_func
Func_02001de4:
	overlay_veneer ObjectTable_SnapshotFar
	.global Func_02001dec
	.thumb_func
Func_02001dec:
	overlay_veneer Func_080c8688
	.global Func_02001df4
	.thumb_func
Func_02001df4:
	overlay_veneer Event_NoOpFar
	.global Func_02001dfc
	.thumb_func
Func_02001dfc:
	overlay_veneer Func_080c86a8
	.global Func_02001e04
	.thumb_func
Func_02001e04:
	overlay_veneer Func_080c86e8
	.global Func_02001e0c
	.thumb_func
Func_02001e0c:
	overlay_veneer Func_080c8760
	.global Func_02001e14
	.thumb_func
Func_02001e14:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_02001e1c
	.thumb_func
Func_02001e1c:
	overlay_veneer Func_08108008
	.global Func_02001e24
	.thumb_func
Func_02001e24:
	overlay_veneer Func_08108018
	.global Func_02001e2c
	.thumb_func
Func_02001e2c:
	overlay_veneer Sound_PlayFar
