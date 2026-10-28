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
	.global HeapSlot_AllocFar
	.thumb_func
HeapSlot_AllocFar:
	overlay_veneer Func_08000140
	.global HeapSlot_Release
	.thumb_func
HeapSlot_Release:
	overlay_veneer HeapSlot_ReleaseFar
	.global Func_02003a14
	.thumb_func
Func_02003a14:
	overlay_veneer Resource_DecodeType01Far
	.global Resource_ResetEntry
	.thumb_func
Resource_ResetEntry:
	overlay_veneer Resource_ResetEntryFar
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
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
	.global Func_02003a54
	.thumb_func
Func_02003a54:
	overlay_veneer Actor_SetMode
	.global Func_02003a5c
	.thumb_func
Func_02003a5c:
	overlay_veneer Actor_StartScriptFar
	.global Import_ActorCreate
	.thumb_func
Import_ActorCreate:
	overlay_veneer Func_080200c0
	.global Func_02003a6c
	.thumb_func
Func_02003a6c:
	overlay_veneer Actor_Destroy
	.global Actor_SetPositionAndResetMotion
	.thumb_func
Actor_SetPositionAndResetMotion:
	overlay_veneer Actor_SetPositionAndResetMotionFar
	.global Func_02003a7c
	.thumb_func
Func_02003a7c:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02003a84
	.thumb_func
Func_02003a84:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02003a8c
	.thumb_func
Func_02003a8c:
	overlay_veneer Actor_CommitPosition
	.global Func_02003a94
	.thumb_func
Func_02003a94:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02003aa4
	.thumb_func
Func_02003aa4:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_02003aac
	.thumb_func
Func_02003aac:
	overlay_veneer Func_08020210
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02003ac4
	.thumb_func
Func_02003ac4:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02003acc
	.thumb_func
Func_02003acc:
	overlay_veneer Func_08020230
	.global Func_02003ad4
	.thumb_func
Func_02003ad4:
	overlay_veneer SetMapCellCollisionFar
	.global Func_02003adc
	.thumb_func
Func_02003adc:
	overlay_veneer Func_080202f0
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02003aec
	.thumb_func
Func_02003aec:
	overlay_veneer Event_StartFar
	.global Func_02003af4
	.thumb_func
Func_02003af4:
	overlay_veneer Event_EndFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_ResetAndSetPositionInMode2
	.thumb_func
Actor_ResetAndSetPositionInMode2:
	overlay_veneer Actor_ResetAndSetPositionInMode2Far
	.global Func_02003b0c
	.thumb_func
Func_02003b0c:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Func_02003b14
	.thumb_func
Func_02003b14:
	overlay_veneer Actor_MoveToObjectFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_SetActionById
	.thumb_func
Actor_SetActionById:
	overlay_veneer Actor_SetActionByIdFar
	.global Actor_WaitForAnimationChange
	.thumb_func
Actor_WaitForAnimationChange:
	overlay_veneer Actor_WaitForAnimationChangeFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_02003b3c
	.thumb_func
Func_02003b3c:
	overlay_veneer Func_080c8208
	.global Func_02003b44
	.thumb_func
Func_02003b44:
	overlay_veneer Func_080c8218
	.global ObjectVisual_CopyAttributes
	.thumb_func
ObjectVisual_CopyAttributes:
	overlay_veneer ObjectVisual_CopyAttributesFar
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Func_02003b5c
	.thumb_func
Func_02003b5c:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02003b6c
	.thumb_func
Func_02003b6c:
	overlay_veneer Func_080c8240
	.global Func_02003b74
	.thumb_func
Func_02003b74:
	overlay_veneer Func_080c8250
	.global Func_02003b7c
	.thumb_func
Func_02003b7c:
	overlay_veneer Event_SetExitRequestFar
	.global Func_02003b84
	.thumb_func
Func_02003b84:
	overlay_veneer Func_080c8288
	.global Func_02003b8c
	.thumb_func
Func_02003b8c:
	overlay_veneer Func_080c82e0
	.global Event_StartScreenTransition
	.thumb_func
Event_StartScreenTransition:
	overlay_veneer Event_StartScreenTransitionFar
	.global Event_WaitTransitionFrames
	.thumb_func
Event_WaitTransitionFrames:
	overlay_veneer Event_WaitTransitionFramesFar
	.global Field_DispatchTypeHandler
	.thumb_func
Field_DispatchTypeHandler:
	overlay_veneer Field_DispatchTypeHandlerFar
	.global Func_02003bac
	.thumb_func
Func_02003bac:
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.global Func_02003bb4
	.thumb_func
Func_02003bb4:
	overlay_veneer BattleEffect_SetupObjectPairFar
	.global Func_02003bbc
	.thumb_func
Func_02003bbc:
	overlay_veneer BattleEffect_StopMainActorFar
	.global Func_02003bc4
	.thumb_func
Func_02003bc4:
	overlay_veneer BattleEffect_CleanupSceneObjectsFar
	.global Func_02003bcc
	.thumb_func
Func_02003bcc:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02003bd4
	.thumb_func
Func_02003bd4:
	overlay_veneer Func_080c8518
	.global Func_02003bdc
	.thumb_func
Func_02003bdc:
	overlay_veneer Event_NoOpFar
	.global Func_02003be4
	.thumb_func
Func_02003be4:
	overlay_veneer Func_080c86a8
	.global Func_02003bec
	.thumb_func
Func_02003bec:
	overlay_veneer Func_080c86d0
	.global Func_02003bf4
	.thumb_func
Func_02003bf4:
	overlay_veneer Func_080c86d8
	.global Func_02003bfc
	.thumb_func
Func_02003bfc:
	overlay_veneer Func_080c86e8
	.global Func_02003c04
	.thumb_func
Func_02003c04:
	overlay_veneer Func_080c8710
	.global Func_02003c0c
	.thumb_func
Func_02003c0c:
	overlay_veneer Func_080c8718
	.global Func_02003c14
	.thumb_func
Func_02003c14:
	overlay_veneer Func_080c8720
	.global Func_02003c1c
	.thumb_func
Func_02003c1c:
	overlay_veneer Func_080c8728
	.global Func_02003c24
	.thumb_func
Func_02003c24:
	overlay_veneer Func_080c8730
	.global Func_02003c2c
	.thumb_func
Func_02003c2c:
	overlay_veneer Func_080c8738
	.global Func_02003c34
	.thumb_func
Func_02003c34:
	overlay_veneer Func_080c87c0
	.global Func_02003c3c
	.thumb_func
Func_02003c3c:
	overlay_veneer Func_080c87d0
	.global Func_02003c44
	.thumb_func
Func_02003c44:
	overlay_veneer BattleEffect_StepVector3Far
	.global Func_02003c4c
	.thumb_func
Func_02003c4c:
	overlay_veneer Func_080c88c8
	.global Func_02003c54
	.thumb_func
Func_02003c54:
	overlay_veneer Sound_PlayFar
