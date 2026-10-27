.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global WaitTicks
	.thumb_func
WaitTicks:
	overlay_veneer WaitTicksFar
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
	.global Func_02004384
	.thumb_func
Func_02004384:
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
	.global Func_020043c4
	.thumb_func
Func_020043c4:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_SetAnimationSpeed
	.thumb_func
ObjectDispatch_SetAnimationSpeed:
	overlay_veneer ObjectDispatch_SetAnimationSpeedFar
	.global Func_020043d4
	.thumb_func
Func_020043d4:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_020043dc
	.thumb_func
Func_020043dc:
	overlay_veneer Func_080200c0
	.global Func_020043e4
	.thumb_func
Func_020043e4:
	overlay_veneer Object_Destroy
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_020043f4
	.thumb_func
Func_020043f4:
	overlay_veneer Object_SetPosition
	.global Func_020043fc
	.thumb_func
Func_020043fc:
	overlay_veneer Object_CommitPosition
	.global Func_02004404
	.thumb_func
Func_02004404:
	overlay_veneer Map_CopyMetatileIndicesRectFar
	.global Func_0200440c
	.thumb_func
Func_0200440c:
	overlay_veneer Func_08020198
	.global Func_02004414
	.thumb_func
Func_02004414:
	overlay_veneer Func_080201a0
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02004424
	.thumb_func
Func_02004424:
	overlay_veneer GetMapCellCollisionFar
	.global Func_0200442c
	.thumb_func
Func_0200442c:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02004444
	.thumb_func
Func_02004444:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_0200444c
	.thumb_func
Func_0200444c:
	overlay_veneer Func_080202f8
	.global Func_02004454
	.thumb_func
Func_02004454:
	overlay_veneer Runtime_CheckRadiusOverlapFar
	.global FieldEvent_WaitTicks
	.thumb_func
FieldEvent_WaitTicks:
	overlay_veneer FieldEvent_WaitTicksFar
	.global Func_02004464
	.thumb_func
Func_02004464:
	overlay_veneer EventRuntime_StartFar
	.global Func_0200446c
	.thumb_func
Func_0200446c:
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
	.global ObjectMotion_EnableActionAndResetMotion
	.thumb_func
ObjectMotion_EnableActionAndResetMotion:
	overlay_veneer ObjectMotion_EnableActionAndResetMotionFar
	.global Object_SetActionCallbackAndRefreshById
	.thumb_func
Object_SetActionCallbackAndRefreshById:
	overlay_veneer Object_SetActionCallbackAndRefreshByIdFar
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_020044cc
	.thumb_func
Func_020044cc:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_020044d4
	.thumb_func
Func_020044d4:
	overlay_veneer ObjectMotion_SetHorizontalPositionAndAngleWithTerrainFar
	.global Func_020044dc
	.thumb_func
Func_020044dc:
	overlay_veneer ObjectMotion_MoveToObjectFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global ObjectMotion_WaitForAnimationChange
	.thumb_func
ObjectMotion_WaitForAnimationChange:
	overlay_veneer ObjectMotion_WaitForAnimationChangeFar
	.global ObjectMotion_Launch
	.thumb_func
ObjectMotion_Launch:
	overlay_veneer ObjectMotion_LaunchFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_0200451c
	.thumb_func
Func_0200451c:
	overlay_veneer EventRuntime_SetMessageFar
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_0200452c
	.thumb_func
Func_0200452c:
	overlay_veneer Func_080c8198
	.global Func_02004534
	.thumb_func
Func_02004534:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global Func_0200453c
	.thumb_func
Func_0200453c:
	overlay_veneer Func_080c81a8
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_0200454c
	.thumb_func
Func_0200454c:
	overlay_veneer ObjectMotion_ArmCallbackAndRefreshFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_0200455c
	.thumb_func
Func_0200455c:
	overlay_veneer Func_080c8210
	.global Func_02004564
	.thumb_func
Func_02004564:
	overlay_veneer Func_080c8218
	.global Func_0200456c
	.thumb_func
Func_0200456c:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_0200457c
	.thumb_func
Func_0200457c:
	overlay_veneer Func_080c8240
	.global Func_02004584
	.thumb_func
Func_02004584:
	overlay_veneer Func_080c8258
	.global Func_0200458c
	.thumb_func
Func_0200458c:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Func_02004594
	.thumb_func
Func_02004594:
	overlay_veneer EventRuntime_PrepareCurrentObjectFar
	.global Func_0200459c
	.thumb_func
Func_0200459c:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_020045a4
	.thumb_func
Func_020045a4:
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
	.global Object_SetWideSprite
	.thumb_func
Object_SetWideSprite:
	overlay_veneer Object_SetWideSpriteFar
	.global Func_020045cc
	.thumb_func
Func_020045cc:
	overlay_veneer Object_ReturnHomeFar
	.global ObjectTable_Get
	.thumb_func
ObjectTable_Get:
	overlay_veneer ObjectTable_GetFar
	.global Func_020045dc
	.thumb_func
Func_020045dc:
	overlay_veneer Func_080c8480
	.global Func_020045e4
	.thumb_func
Func_020045e4:
	overlay_veneer EventRuntime_ResolveAllPendingActionsFar
	.global Func_020045ec
	.thumb_func
Func_020045ec:
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global Func_020045f4
	.thumb_func
Func_020045f4:
	overlay_veneer Func_080c8580
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_02004604
	.thumb_func
Func_02004604:
	overlay_veneer Func_080c87c0
	.global Func_0200460c
	.thumb_func
Func_0200460c:
	overlay_veneer Func_080c87d0
	.global Func_02004614
	.thumb_func
Func_02004614:
	overlay_veneer BattleEffect_StepVector3Far
	.global Func_0200461c
	.thumb_func
Func_0200461c:
	overlay_veneer Func_080c8918
	.global Func_02004624
	.thumb_func
Func_02004624:
	overlay_veneer Func_08108008
	.global Func_0200462c
	.thumb_func
Func_0200462c:
	overlay_veneer Func_08108010
	.global Func_02004634
	.thumb_func
Func_02004634:
	overlay_veneer Func_08108018
	.global Func_0200463c
	.thumb_func
Func_0200463c:
	overlay_veneer Audio_PlayCue
