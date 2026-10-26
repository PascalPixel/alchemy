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
	.global __umodsi3
	.thumb_func
__umodsi3:
	overlay_veneer IwramUnsignedRemainder
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
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.global Math_Cosine
	.thumb_func
Math_Cosine:
	overlay_veneer Math_CosineFar
	.global Runtime_AllocateHeapBlockFar
	.thumb_func
Runtime_AllocateHeapBlockFar:
	overlay_veneer Func_08000140
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
	.global Func_020027b8
	.thumb_func
Func_020027b8:
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
	.global Func_020027d8
	.thumb_func
Func_020027d8:
	overlay_veneer Runtime_PushSlotEntryFar
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
	.global GameFlag_GetByte
	.thumb_func
GameFlag_GetByte:
	overlay_veneer GameFlag_GetByteFar
	.global GameFlag_SetByte
	.thumb_func
GameFlag_SetByte:
	overlay_veneer GameFlag_SetByteFar
	.global Func_02002810
	.thumb_func
Func_02002810:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_SetAnimationSpeed
	.thumb_func
ObjectDispatch_SetAnimationSpeed:
	overlay_veneer ObjectDispatch_SetAnimationSpeedFar
	.global Func_02002820
	.thumb_func
Func_02002820:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02002828
	.thumb_func
Func_02002828:
	overlay_veneer Func_080200c0
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02002838
	.thumb_func
Func_02002838:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02002840
	.thumb_func
Func_02002840:
	overlay_veneer Object_SetPosition
	.global Func_02002848
	.thumb_func
Func_02002848:
	overlay_veneer Object_CommitPosition
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02002858
	.thumb_func
Func_02002858:
	overlay_veneer Map_CopyCellsRectFar
	.global Func_02002860
	.thumb_func
Func_02002860:
	overlay_veneer Func_080201f0
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Func_02002870
	.thumb_func
Func_02002870:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02002878
	.thumb_func
Func_02002878:
	overlay_veneer Func_08020230
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02002888
	.thumb_func
Func_02002888:
	overlay_veneer Func_08020360
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Func_02002898
	.thumb_func
Func_02002898:
	overlay_veneer Func_08038210
	.global Func_020028a0
	.thumb_func
Func_020028a0:
	overlay_veneer Func_080ad208
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_020028b0
	.thumb_func
Func_020028b0:
	overlay_veneer EventRuntime_StartFar
	.global Func_020028b8
	.thumb_func
Func_020028b8:
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
	.global ObjectMotion_EnableActionAndSetCallback
	.thumb_func
ObjectMotion_EnableActionAndSetCallback:
	overlay_veneer ObjectMotion_EnableActionAndSetCallbackFar
	.global Object_RefreshSelectorById
	.thumb_func
Object_RefreshSelectorById:
	overlay_veneer Object_RefreshSelectorByIdFar
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global Func_020028f8
	.thumb_func
Func_020028f8:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_02002900
	.thumb_func
Func_02002900:
	overlay_veneer ObjectMotion_SetHorizontalPositionAndAngleWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global Func_02002918
	.thumb_func
Func_02002918:
	overlay_veneer ObjectGroup_ConfigureChildValueFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02002928
	.thumb_func
Func_02002928:
	overlay_veneer EventRuntime_SetMessageFar
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_02002938
	.thumb_func
Func_02002938:
	overlay_veneer Func_080c8198
	.global Func_02002940
	.thumb_func
Func_02002940:
	overlay_veneer ObjectMotion_ArmCallbackAndRefreshFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02002950
	.thumb_func
Func_02002950:
	overlay_veneer Func_080c8210
	.global Func_02002958
	.thumb_func
Func_02002958:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02002970
	.thumb_func
Func_02002970:
	overlay_veneer Func_080c8258
	.global Func_02002978
	.thumb_func
Func_02002978:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_02002980
	.thumb_func
Func_02002980:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global Func_02002988
	.thumb_func
Func_02002988:
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
	.global Func_020029a8
	.thumb_func
Func_020029a8:
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global Func_020029b0
	.thumb_func
Func_020029b0:
	overlay_veneer Camera_WorldToScreenFar
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Object_SetActionCallback
	.thumb_func
Object_SetActionCallback:
	overlay_veneer Object_SetActionCallbackFar
	.global Func_020029c8
	.thumb_func
Func_020029c8:
	overlay_veneer Func_080c8718
	.global Func_020029d0
	.thumb_func
Func_020029d0:
	overlay_veneer Func_080c8720
	.global Func_020029d8
	.thumb_func
Func_020029d8:
	overlay_veneer Func_080c8728
	.global Func_020029e0
	.thumb_func
Func_020029e0:
	overlay_veneer Func_080c87c0
	.global Func_020029e8
	.thumb_func
Func_020029e8:
	overlay_veneer Func_080c87c8
	.global Func_020029f0
	.thumb_func
Func_020029f0:
	overlay_veneer Func_080c87e8
	.global Func_020029f8
	.thumb_func
Func_020029f8:
	overlay_veneer Func_080c87f0
	.global Func_02002a00
	.thumb_func
Func_02002a00:
	overlay_veneer BattleEffect_StartArcEffectFar
	.global Func_02002a08
	.thumb_func
Func_02002a08:
	overlay_veneer BattleEffect_EndArcEffectFar
	.global Func_02002a10
	.thumb_func
Func_02002a10:
	overlay_veneer BattleEffect_WaitForArcGroupsFar
	.global Func_02002a18
	.thumb_func
Func_02002a18:
	overlay_veneer BattleEffect_SetObjectTickModeFar
	.global Func_02002a20
	.thumb_func
Func_02002a20:
	overlay_veneer Func_080c8830
	.global Func_02002a28
	.thumb_func
Func_02002a28:
	overlay_veneer Func_080c8840
	.global Func_02002a30
	.thumb_func
Func_02002a30:
	overlay_veneer Func_080c8848
	.global AudioCommand_WaitForStateByteClear
	.thumb_func
AudioCommand_WaitForStateByteClear:
	overlay_veneer AudioCommand_WaitForStateByteClearFar
	.global Func_02002a40
	.thumb_func
Func_02002a40:
	overlay_veneer Audio_PlayCue
