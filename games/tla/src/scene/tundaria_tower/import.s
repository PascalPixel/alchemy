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
	.global ArcTan2
	.thumb_func
ArcTan2:
	overlay_veneer ArcTan2Far
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
	.global Func_02002f40
	.thumb_func
Func_02002f40:
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
	.global Func_02002f80
	.thumb_func
Func_02002f80:
	overlay_veneer Actor_SetMode
	.global Actor_SetAnimationSpeed
	.thumb_func
Actor_SetAnimationSpeed:
	overlay_veneer Actor_SetAnimationSpeedFar
	.global Func_02002f90
	.thumb_func
Func_02002f90:
	overlay_veneer Actor_StartScriptFar
	.global Func_02002f98
	.thumb_func
Func_02002f98:
	overlay_veneer Func_080200c0
	.global Actor_SetPositionAndResetMotion
	.thumb_func
Actor_SetPositionAndResetMotion:
	overlay_veneer Actor_SetPositionAndResetMotionFar
	.global Func_02002fa8
	.thumb_func
Func_02002fa8:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02002fb0
	.thumb_func
Func_02002fb0:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02002fb8
	.thumb_func
Func_02002fb8:
	overlay_veneer Actor_CommitPosition
	.global Func_02002fc0
	.thumb_func
Func_02002fc0:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Func_02002fc8
	.thumb_func
Func_02002fc8:
	overlay_veneer DisplayBlend_DisableRunScriptFar
	.global Func_02002fd0
	.thumb_func
Func_02002fd0:
	overlay_veneer DisplayBlend_EnableRunScriptFar
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02002fe0
	.thumb_func
Func_02002fe0:
	overlay_veneer GetMapCellCollisionFar
	.global Func_02002fe8
	.thumb_func
Func_02002fe8:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02003000
	.thumb_func
Func_02003000:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02003008
	.thumb_func
Func_02003008:
	overlay_veneer Func_08020230
	.global Func_02003010
	.thumb_func
Func_02003010:
	overlay_veneer Func_080202f0
	.global Func_02003018
	.thumb_func
Func_02003018:
	overlay_veneer Func_080202f8
	.global Func_02003020
	.thumb_func
Func_02003020:
	overlay_veneer Func_08020300
	.global Func_02003028
	.thumb_func
Func_02003028:
	overlay_veneer Field_CheckSpheresOverlapFar
	.global Func_02003030
	.thumb_func
Func_02003030:
	overlay_veneer Func_08020360
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
	.global FieldEvent_Wait
	.thumb_func
FieldEvent_Wait:
	overlay_veneer FieldEvent_WaitFar
	.global Func_02003048
	.thumb_func
Func_02003048:
	overlay_veneer Event_StartFar
	.global Func_02003050
	.thumb_func
Func_02003050:
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
	.global Func_02003088
	.thumb_func
Func_02003088:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Func_02003090
	.thumb_func
Func_02003090:
	overlay_veneer Actor_MoveToObjectFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Actor_SetActionById
	.thumb_func
Actor_SetActionById:
	overlay_veneer Actor_SetActionByIdFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Func_020030c0
	.thumb_func
Func_020030c0:
	overlay_veneer Func_080c8208
	.global Func_020030c8
	.thumb_func
Func_020030c8:
	overlay_veneer Func_080c8218
	.global ObjectVisual_CopyAttributes
	.thumb_func
ObjectVisual_CopyAttributes:
	overlay_veneer ObjectVisual_CopyAttributesFar
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_020030e8
	.thumb_func
Func_020030e8:
	overlay_veneer Event_SetExitRequestFar
	.global Func_020030f0
	.thumb_func
Func_020030f0:
	overlay_veneer Func_080c8288
	.global Func_020030f8
	.thumb_func
Func_020030f8:
	overlay_veneer Func_080c82e0
	.global Func_02003100
	.thumb_func
Func_02003100:
	overlay_veneer Leader_StopMovingFar
	.global Func_02003108
	.thumb_func
Func_02003108:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_02003110
	.thumb_func
Func_02003110:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global Func_02003118
	.thumb_func
Func_02003118:
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
	.global ObjectTable_Get
	.thumb_func
ObjectTable_Get:
	overlay_veneer ObjectTable_GetFar
	.global Func_02003140
	.thumb_func
Func_02003140:
	overlay_veneer Event_ResolveAllPendingActionsFar
	.global Func_02003148
	.thumb_func
Func_02003148:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Func_02003150
	.thumb_func
Func_02003150:
	overlay_veneer Func_080c8518
	.global Actor_WatchAndSetScript
	.thumb_func
Actor_WatchAndSetScript:
	overlay_veneer Actor_WatchAndSetScriptFar
	.global Func_02003160
	.thumb_func
Func_02003160:
	overlay_veneer Func_080c8710
	.global Func_02003168
	.thumb_func
Func_02003168:
	overlay_veneer Func_080c8718
	.global Func_02003170
	.thumb_func
Func_02003170:
	overlay_veneer Func_080c8720
	.global Func_02003178
	.thumb_func
Func_02003178:
	overlay_veneer Func_080c8728
	.global Func_02003180
	.thumb_func
Func_02003180:
	overlay_veneer Func_080c8730
	.global Func_02003188
	.thumb_func
Func_02003188:
	overlay_veneer Func_080c8738
	.global Func_02003190
	.thumb_func
Func_02003190:
	overlay_veneer Func_080c87c0
	.global Func_02003198
	.thumb_func
Func_02003198:
	overlay_veneer Func_080c87d0
	.global Func_020031a0
	.thumb_func
Func_020031a0:
	overlay_veneer BattleEffect_StepVector3Far
	.global Func_020031a8
	.thumb_func
Func_020031a8:
	overlay_veneer Sound_PlayFar
