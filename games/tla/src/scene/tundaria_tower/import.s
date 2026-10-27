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
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
	.global Scheduler_RemoveCallbackFar
	.thumb_func
Scheduler_RemoveCallbackFar:
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
	.global Runtime_AllocateHeapBlockFar
	.thumb_func
Runtime_AllocateHeapBlockFar:
	overlay_veneer Func_08000140
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	overlay_veneer Runtime_ReleaseHeapBlockFar
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
	overlay_veneer Object_SetMode
	.global ObjectDispatch_SetAnimationSpeed
	.thumb_func
ObjectDispatch_SetAnimationSpeed:
	overlay_veneer ObjectDispatch_SetAnimationSpeedFar
	.global Func_02002f90
	.thumb_func
Func_02002f90:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02002f98
	.thumb_func
Func_02002f98:
	overlay_veneer Func_080200c0
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02002fa8
	.thumb_func
Func_02002fa8:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02002fb0
	.thumb_func
Func_02002fb0:
	overlay_veneer Object_SetPosition
	.global Func_02002fb8
	.thumb_func
Func_02002fb8:
	overlay_veneer Object_CommitPosition
	.global Func_02002fc0
	.thumb_func
Func_02002fc0:
	overlay_veneer Map_CopyMetatileIndicesRectFar
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
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
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
	overlay_veneer Runtime_CheckRadiusOverlapFar
	.global Func_02003030
	.thumb_func
Func_02003030:
	overlay_veneer Func_08020360
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02003048
	.thumb_func
Func_02003048:
	overlay_veneer EventRuntime_StartFar
	.global Func_02003050
	.thumb_func
Func_02003050:
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
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02003088
	.thumb_func
Func_02003088:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_02003090
	.thumb_func
Func_02003090:
	overlay_veneer ObjectMotion_MoveToObjectFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Object_SetActionById
	.thumb_func
Object_SetActionById:
	overlay_veneer Object_SetActionByIdFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
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
	overlay_veneer EventRuntime_SetCountdownFar
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
	overlay_veneer EventRuntime_PrepareCurrentObjectFar
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
	overlay_veneer EventRuntime_ResolveAllPendingActionsFar
	.global Func_02003148
	.thumb_func
Func_02003148:
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global Func_02003150
	.thumb_func
Func_02003150:
	overlay_veneer Func_080c8518
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
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
	overlay_veneer Audio_PlayCue
