@ Import veneers after the OVERLAY_6A8 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6A8 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_MathDivide
	.thumb_func
Engine_MathDivide:
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
	overlay_veneer Func_080001a8
	.global Resource_ResetEntry
	.thumb_func
Resource_ResetEntry:
	overlay_veneer Resource_ResetEntryFar
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
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
	.global ObjectDispatch_ApplyValueToChildren
	.thumb_func
ObjectDispatch_ApplyValueToChildren:
	overlay_veneer ObjectDispatch_ApplyValueToChildrenFar
	.global Func_020043d4
	.thumb_func
Func_020043d4:
	overlay_veneer Object_SetCallback
	.global Func_020043dc
	.thumb_func
Func_020043dc:
	overlay_veneer Func_080200c0
	.global Func_020043e4
	.thumb_func
Func_020043e4:
	overlay_veneer Func_080200c8
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
	overlay_veneer Func_08020178
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
	overlay_veneer Func_080201c8
	.global Func_0200442c
	.thumb_func
Func_0200442c:
	overlay_veneer Func_080201e8
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02004444
	.thumb_func
Func_02004444:
	overlay_veneer Func_08020228
	.global Func_0200444c
	.thumb_func
Func_0200444c:
	overlay_veneer Func_080202f8
	.global Func_02004454
	.thumb_func
Func_02004454:
	overlay_veneer Func_08020348
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02004464
	.thumb_func
Func_02004464:
	overlay_veneer Func_080c8018
	.global Func_0200446c
	.thumb_func
Func_0200446c:
	overlay_veneer Func_080c8020
	.global Inventory_PromptAndSetObjectMode
	.thumb_func
Inventory_PromptAndSetObjectMode:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
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
	overlay_veneer Func_080c80f8
	.global Func_020044d4
	.thumb_func
Func_020044d4:
	overlay_veneer Func_080c8100
	.global Func_020044dc
	.thumb_func
Func_020044dc:
	overlay_veneer Func_080c8110
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
	overlay_veneer Func_080c8180
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
	overlay_veneer Func_080c81a0
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
	overlay_veneer Func_080c81d8
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
	overlay_veneer Func_080c8230
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
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
	overlay_veneer Func_080c8278
	.global Func_02004594
	.thumb_func
Func_02004594:
	overlay_veneer Func_080c82f8
	.global Func_0200459c
	.thumb_func
Func_0200459c:
	overlay_veneer Func_080c8378
	.global Func_020045a4
	.thumb_func
Func_020045a4:
	overlay_veneer Func_080c8390
	.global Event_SetStatus1c6
	.thumb_func
Event_SetStatus1c6:
	overlay_veneer Event_SetStatus1c6Far
	.global Event_ClearStatus1c6
	.thumb_func
Event_ClearStatus1c6:
	overlay_veneer Event_ClearStatus1c6Far
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global Object_SetWideSprite
	.thumb_func
Object_SetWideSprite:
	overlay_veneer Object_SetWideSpriteFar
	.global Func_020045cc
	.thumb_func
Func_020045cc:
	overlay_veneer Func_080c8428
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
	overlay_veneer Func_080c84d8
	.global Func_020045ec
	.thumb_func
Func_020045ec:
	overlay_veneer Func_080c84e0
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
	overlay_veneer Func_080c87e0
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
