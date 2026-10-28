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
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
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
	overlay_veneer Runtime_SetIrqHandlerFar
	.global Runtime_AllocateBlock
	.thumb_func
Runtime_AllocateBlock:
	overlay_veneer Runtime_AllocateBlockFar
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	overlay_veneer Runtime_ReleaseHeapBlockFar
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
	overlay_veneer Object_SetMode
	.global Func_02001c74
	.thumb_func
Func_02001c74:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Engine_ObjectCreate
	.thumb_func
Engine_ObjectCreate:
	overlay_veneer Func_080200c0
	.global Func_02001c84
	.thumb_func
Func_02001c84:
	overlay_veneer Object_SetPosition
	.global Func_02001c8c
	.thumb_func
Func_02001c8c:
	overlay_veneer Object_CommitPosition
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02001c9c
	.thumb_func
Func_02001c9c:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02001cc4
	.thumb_func
Func_02001cc4:
	overlay_veneer EventRuntime_StartFar
	.global Func_02001ccc
	.thumb_func
Func_02001ccc:
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
	.global Func_02001d04
	.thumb_func
Func_02001d04:
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
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Func_02001d2c
	.thumb_func
Func_02001d2c:
	overlay_veneer Object_JumpDownLedgeFar
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02001d44
	.thumb_func
Func_02001d44:
	overlay_veneer EventRuntime_SetMessageFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_02001d54
	.thumb_func
Func_02001d54:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
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
	overlay_veneer EventRuntime_SetCountdownFar
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
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global Func_02001dd4
	.thumb_func
Func_02001dd4:
	overlay_veneer Func_080c8580
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
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
	overlay_veneer EventRuntime_GetControlledOwnerFar
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
