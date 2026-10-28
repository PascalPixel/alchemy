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
	.global Runtime_AllocateBlock
	.thumb_func
Runtime_AllocateBlock:
	overlay_veneer Runtime_AllocateBlockFar
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	overlay_veneer Runtime_ReleaseHeapBlockFar
	.global MemAlloc
	.thumb_func
MemAlloc:
	overlay_veneer Runtime_BumpAllocateFar
	.global MemFree
	.thumb_func
MemFree:
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
	overlay_veneer Object_SetMode
	.global Func_02002918
	.thumb_func
Func_02002918:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Engine_ObjectCreate
	.thumb_func
Engine_ObjectCreate:
	overlay_veneer Func_080200c0
	.global Func_02002928
	.thumb_func
Func_02002928:
	overlay_veneer Object_Destroy
	.global Func_02002930
	.thumb_func
Func_02002930:
	overlay_veneer Object_SetPosition
	.global Func_02002938
	.thumb_func
Func_02002938:
	overlay_veneer Object_CommitPosition
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
	.global Func_02002998
	.thumb_func
Func_02002998:
	overlay_veneer EventRuntime_StartFar
	.global Func_020029a0
	.thumb_func
Func_020029a0:
	overlay_veneer EventRuntime_EndFar
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
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_020029d8
	.thumb_func
Func_020029d8:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
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
	overlay_veneer EventRuntime_SetCountdownFar
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
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
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
	overlay_veneer EventRuntime_GetControlledOwnerFar
	.global Func_02002a70
	.thumb_func
Func_02002a70:
	overlay_veneer Sound_PlayFar
