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
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global HeapSlot_AllocEwram
	.thumb_func
HeapSlot_AllocEwram:
	overlay_veneer HeapSlot_AllocEwramFar
	.global HeapSlot_Release
	.thumb_func
HeapSlot_Release:
	overlay_veneer HeapSlot_ReleaseFar
	.global Heap_Alloc
	.thumb_func
Heap_Alloc:
	overlay_veneer Heap_AllocFar
	.global Heap_Release
	.thumb_func
Heap_Release:
	overlay_veneer Sys_FreeFar
	.global Func_020028b0
	.thumb_func
Func_020028b0:
	overlay_veneer Resource_DecodeByteLzInRamFar
	.global Func_020028b8
	.thumb_func
Func_020028b8:
	overlay_veneer Resource_DecodeType01Far
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_020028d0
	.thumb_func
Func_020028d0:
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
	.global GameFlag_GetByte
	.thumb_func
GameFlag_GetByte:
	overlay_veneer GameFlag_GetByteFar
	.global GameFlag_SetByte
	.thumb_func
GameFlag_SetByte:
	overlay_veneer GameFlag_SetByteFar
	.global Animation_ApplyChildArgument
	.thumb_func
Animation_ApplyChildArgument:
	overlay_veneer Animation_ApplyChildArgumentFar
	.global ResourceMetadata_Register
	.thumb_func
ResourceMetadata_Register:
	overlay_veneer ResourceMetadata_RegisterFar
	.global Func_02002910
	.thumb_func
Func_02002910:
	overlay_veneer Actor_SetMode
	.global Func_02002918
	.thumb_func
Func_02002918:
	overlay_veneer Actor_StartScriptFar
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Func_080200c0
	.global Func_02002928
	.thumb_func
Func_02002928:
	overlay_veneer Actor_Destroy
	.global Func_02002930
	.thumb_func
Func_02002930:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02002938
	.thumb_func
Func_02002938:
	overlay_veneer Actor_CommitPosition
	.global Func_02002940
	.thumb_func
Func_02002940:
	overlay_veneer Func_08020198
	.global Func_02002948
	.thumb_func
Func_02002948:
	overlay_veneer Func_080201a0
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02002958
	.thumb_func
Func_02002958:
	overlay_veneer Map_CopyCellsRectFar
	.global Func_02002960
	.thumb_func
Func_02002960:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02002968
	.thumb_func
Func_02002968:
	overlay_veneer Func_080201f0
	.global Func_02002970
	.thumb_func
Func_02002970:
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
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02002998
	.thumb_func
Func_02002998:
	overlay_veneer Event_StartFar
	.global Func_020029a0
	.thumb_func
Func_020029a0:
	overlay_veneer Event_EndFar
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
	.global Actor_SetPositionAndCommit
	.thumb_func
Actor_SetPositionAndCommit:
	overlay_veneer Actor_SetPositionAndCommitFar
	.global Actor_OffsetPositionAndResetMotion
	.thumb_func
Actor_OffsetPositionAndResetMotion:
	overlay_veneer Actor_OffsetPositionAndResetMotionFar
	.global Actor_WaitForMoveById
	.thumb_func
Actor_WaitForMoveById:
	overlay_veneer Actor_WaitForMoveByIdFar
	.global Func_020029d8
	.thumb_func
Func_020029d8:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_020029f8
	.thumb_func
Func_020029f8:
	overlay_veneer Func_080c8218
	.global Func_02002a00
	.thumb_func
Func_02002a00:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02002a10
	.thumb_func
Func_02002a10:
	overlay_veneer Func_080c8240
	.global Func_02002a18
	.thumb_func
Func_02002a18:
	overlay_veneer Event_SetExitRequestFar
	.global Func_02002a20
	.thumb_func
Func_02002a20:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_02002a28
	.thumb_func
Func_02002a28:
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
	.global Func_02002a48
	.thumb_func
Func_02002a48:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Actor_CommitPositionAndActivate
	.thumb_func
Actor_CommitPositionAndActivate:
	overlay_veneer Actor_CommitPositionAndActivateFar
	.global Func_02002a58
	.thumb_func
Func_02002a58:
	overlay_veneer Func_080c8688
	.global Func_02002a60
	.thumb_func
Func_02002a60:
	overlay_veneer Event_NoOpFar
	.global Func_02002a68
	.thumb_func
Func_02002a68:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_02002a70
	.thumb_func
Func_02002a70:
	overlay_veneer Sound_PlayFar
