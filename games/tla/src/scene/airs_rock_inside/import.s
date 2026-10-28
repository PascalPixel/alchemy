.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
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
	.global Heap_Alloc
	.thumb_func
Heap_Alloc:
	overlay_veneer Heap_AllocFar
	.global Heap_Release
	.thumb_func
Heap_Release:
	overlay_veneer Sys_FreeFar
	.global Func_02005d24
	.thumb_func
Func_02005d24:
	overlay_veneer Resource_DecodeType01Far
	.global Func_02005d2c
	.thumb_func
Func_02005d2c:
	overlay_veneer DebugText_ClearFar
	.global Func_02005d34
	.thumb_func
Func_02005d34:
	overlay_veneer Window_LoadGraphicsFar
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
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
	.global Func_02005d5c
	.thumb_func
Func_02005d5c:
	overlay_veneer Actor_SetMode
	.global Actor_SetAnimationSpeed
	.thumb_func
Actor_SetAnimationSpeed:
	overlay_veneer Actor_SetAnimationSpeedFar
	.global Func_02005d6c
	.thumb_func
Func_02005d6c:
	overlay_veneer Actor_StartScriptFar
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Func_080200c0
	.global Func_02005d7c
	.thumb_func
Func_02005d7c:
	overlay_veneer Actor_Destroy
	.global Func_02005d84
	.thumb_func
Func_02005d84:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02005d8c
	.thumb_func
Func_02005d8c:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02005d94
	.thumb_func
Func_02005d94:
	overlay_veneer Actor_CommitPosition
	.global Func_02005d9c
	.thumb_func
Func_02005d9c:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02005dac
	.thumb_func
Func_02005dac:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02005db4
	.thumb_func
Func_02005db4:
	overlay_veneer Func_080201f0
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02005dcc
	.thumb_func
Func_02005dcc:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02005dd4
	.thumb_func
Func_02005dd4:
	overlay_veneer Func_08020230
	.global Map_EnableUpdateCallback
	.thumb_func
Map_EnableUpdateCallback:
	overlay_veneer Map_EnableUpdateCallbackFar
	.global Map_DisableUpdateCallback
	.thumb_func
Map_DisableUpdateCallback:
	overlay_veneer Map_DisableUpdateCallbackFar
	.global Func_02005dec
	.thumb_func
Func_02005dec:
	overlay_veneer Func_080202f8
	.global Func_02005df4
	.thumb_func
Func_02005df4:
	overlay_veneer Func_08020360
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global Func_02005e04
	.thumb_func
Func_02005e04:
	overlay_veneer Func_08038348
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02005e14
	.thumb_func
Func_02005e14:
	overlay_veneer Event_StartFar
	.global Func_02005e1c
	.thumb_func
Func_02005e1c:
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
	.global Func_02005e6c
	.thumb_func
Func_02005e6c:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Func_02005e74
	.thumb_func
Func_02005e74:
	overlay_veneer Actor_MoveToObjectFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Func_02005e84
	.thumb_func
Func_02005e84:
	overlay_veneer Actor_JumpDownLedgeFar
	.global Actor_TurnToFaceEachOther
	.thumb_func
Actor_TurnToFaceEachOther:
	overlay_veneer Actor_TurnToFaceEachOtherFar
	.global Func_02005e94
	.thumb_func
Func_02005e94:
	overlay_veneer Actor_ConfigureSpriteColorFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_02005ea4
	.thumb_func
Func_02005ea4:
	overlay_veneer Event_SetMessageIdFar
	.global Func_02005eac
	.thumb_func
Func_02005eac:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_02005ec4
	.thumb_func
Func_02005ec4:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Func_02005ed4
	.thumb_func
Func_02005ed4:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02005ee4
	.thumb_func
Func_02005ee4:
	overlay_veneer Func_080c8240
	.global Func_02005eec
	.thumb_func
Func_02005eec:
	overlay_veneer Func_080c8248
	.global Func_02005ef4
	.thumb_func
Func_02005ef4:
	overlay_veneer Func_080c8258
	.global Func_02005efc
	.thumb_func
Func_02005efc:
	overlay_veneer Event_SetExitRequestFar
	.global Func_02005f04
	.thumb_func
Func_02005f04:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_02005f0c
	.thumb_func
Func_02005f0c:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global Func_02005f14
	.thumb_func
Func_02005f14:
	overlay_veneer BattleEffect_StartBufferInterpolationFar
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Func_02005f2c
	.thumb_func
Func_02005f2c:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02005f34
	.thumb_func
Func_02005f34:
	overlay_veneer BattleParty_ApplyDrainFar
	.global Func_02005f3c
	.thumb_func
Func_02005f3c:
	overlay_veneer Party_PlaySavedMusicFar
	.global Actor_SetActionCallback
	.thumb_func
Actor_SetActionCallback:
	overlay_veneer Actor_SetActionCallbackFar
	.global Func_02005f4c
	.thumb_func
Func_02005f4c:
	overlay_veneer Func_080c8688
	.global Func_02005f54
	.thumb_func
Func_02005f54:
	overlay_veneer Func_080c86d0
	.global Func_02005f5c
	.thumb_func
Func_02005f5c:
	overlay_veneer Func_080c86d8
	.global Func_02005f64
	.thumb_func
Func_02005f64:
	overlay_veneer Func_080c8710
	.global Func_02005f6c
	.thumb_func
Func_02005f6c:
	overlay_veneer Func_080c8718
	.global Func_02005f74
	.thumb_func
Func_02005f74:
	overlay_veneer Func_080c8720
	.global Func_02005f7c
	.thumb_func
Func_02005f7c:
	overlay_veneer Func_080c8728
	.global Func_02005f84
	.thumb_func
Func_02005f84:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_02005f8c
	.thumb_func
Func_02005f8c:
	overlay_veneer Func_080c88c8
	.global Func_02005f94
	.thumb_func
Func_02005f94:
	overlay_veneer Sound_PlayFar
