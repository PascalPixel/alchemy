.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
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
	.global Func_02003c7c
	.thumb_func
Func_02003c7c:
	overlay_veneer Resource_DecodeType01Far
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02003c94
	.thumb_func
Func_02003c94:
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
	.global Func_02003cc4
	.thumb_func
Func_02003cc4:
	overlay_veneer Actor_SetMode
	.global Func_02003ccc
	.thumb_func
Func_02003ccc:
	overlay_veneer Actor_StartScriptFar
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Func_080200c0
	.global Func_02003cdc
	.thumb_func
Func_02003cdc:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02003ce4
	.thumb_func
Func_02003ce4:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02003cec
	.thumb_func
Func_02003cec:
	overlay_veneer Actor_CommitPosition
	.global Func_02003cf4
	.thumb_func
Func_02003cf4:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Func_02003cfc
	.thumb_func
Func_02003cfc:
	overlay_veneer DisplayBlend_DisableRunScriptFar
	.global Func_02003d04
	.thumb_func
Func_02003d04:
	overlay_veneer DisplayBlend_EnableRunScriptFar
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02003d14
	.thumb_func
Func_02003d14:
	overlay_veneer Map_CopyCellsRectFar
	.global Func_02003d1c
	.thumb_func
Func_02003d1c:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02003d24
	.thumb_func
Func_02003d24:
	overlay_veneer Func_080201f0
	.global Func_02003d2c
	.thumb_func
Func_02003d2c:
	overlay_veneer Func_08020210
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02003d44
	.thumb_func
Func_02003d44:
	overlay_veneer Map_SetScreenShakeFar
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02003d54
	.thumb_func
Func_02003d54:
	overlay_veneer Func_08020378
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global Func_02003d64
	.thumb_func
Func_02003d64:
	overlay_veneer MessageArgument_PushFar
	.global PartyInventory_FindOwner
	.thumb_func
PartyInventory_FindOwner:
	overlay_veneer PartyInventory_FindOwnerFar
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_02003d7c
	.thumb_func
Func_02003d7c:
	overlay_veneer Event_StartFar
	.global Func_02003d84
	.thumb_func
Func_02003d84:
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
	.global Actor_OffsetPositionAndResetMotion
	.thumb_func
Actor_OffsetPositionAndResetMotion:
	overlay_veneer Actor_OffsetPositionAndResetMotionFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_02003ddc
	.thumb_func
Func_02003ddc:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Func_02003de4
	.thumb_func
Func_02003de4:
	overlay_veneer Actor_MoveToObjectFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Func_02003df4
	.thumb_func
Func_02003df4:
	overlay_veneer Actor_JumpDownLedgeFar
	.global Actor_TurnToFaceEachOther
	.thumb_func
Actor_TurnToFaceEachOther:
	overlay_veneer Actor_TurnToFaceEachOtherFar
	.global Func_02003e04
	.thumb_func
Func_02003e04:
	overlay_veneer Actor_ConfigureSpriteColorFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_02003e14
	.thumb_func
Func_02003e14:
	overlay_veneer Event_SetMessageIdFar
	.global Func_02003e1c
	.thumb_func
Func_02003e1c:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_02003e34
	.thumb_func
Func_02003e34:
	overlay_veneer Func_080c8210
	.global Func_02003e3c
	.thumb_func
Func_02003e3c:
	overlay_veneer Func_080c8218
	.global Func_02003e44
	.thumb_func
Func_02003e44:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02003e54
	.thumb_func
Func_02003e54:
	overlay_veneer Func_080c8240
	.global Func_02003e5c
	.thumb_func
Func_02003e5c:
	overlay_veneer Event_SetExitRequestFar
	.global Func_02003e64
	.thumb_func
Func_02003e64:
	overlay_veneer Func_080c8288
	.global Func_02003e6c
	.thumb_func
Func_02003e6c:
	overlay_veneer Func_080c82e0
	.global Func_02003e74
	.thumb_func
Func_02003e74:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_02003e7c
	.thumb_func
Func_02003e7c:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global Func_02003e84
	.thumb_func
Func_02003e84:
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
	.global Func_02003ea4
	.thumb_func
Func_02003ea4:
	overlay_veneer Func_080c8480
	.global Func_02003eac
	.thumb_func
Func_02003eac:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02003eb4
	.thumb_func
Func_02003eb4:
	overlay_veneer BattleParty_ApplyDrainFar
	.global Func_02003ebc
	.thumb_func
Func_02003ebc:
	overlay_veneer Party_PlaySavedMusicFar
	.global Actor_CommitPositionAndActivate
	.thumb_func
Actor_CommitPositionAndActivate:
	overlay_veneer Actor_CommitPositionAndActivateFar
	.global Func_02003ecc
	.thumb_func
Func_02003ecc:
	overlay_veneer Func_080c8688
	.global Func_02003ed4
	.thumb_func
Func_02003ed4:
	overlay_veneer Event_NoOpFar
	.global Func_02003edc
	.thumb_func
Func_02003edc:
	overlay_veneer Func_080c86d0
	.global Func_02003ee4
	.thumb_func
Func_02003ee4:
	overlay_veneer Func_080c86d8
	.global Func_02003eec
	.thumb_func
Func_02003eec:
	overlay_veneer Func_080c8860
	.global Func_02003ef4
	.thumb_func
Func_02003ef4:
	overlay_veneer Sound_PlayFar
	.global Sound_WaitForBgmRestore
	.thumb_func
Sound_WaitForBgmRestore:
	overlay_veneer Sound_WaitForBgmRestoreFar
