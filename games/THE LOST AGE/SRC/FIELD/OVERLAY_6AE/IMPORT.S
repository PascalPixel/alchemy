@ Import veneers after the OVERLAY_6AE overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6AE overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_MathDivide
	.thumb_func
Engine_MathDivide:
	overlay_veneer IwramSignedDivide
	.global Engine_MathModulo
	.thumb_func
Engine_MathModulo:
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
	.global Func_0200556c
	.thumb_func
Func_0200556c:
	overlay_veneer Object_SetCallback
	.global Func_02005574
	.thumb_func
Func_02005574:
	overlay_veneer Func_080200c0
	.global Func_0200557c
	.thumb_func
Func_0200557c:
	overlay_veneer Func_080200c8
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_0200558c
	.thumb_func
Func_0200558c:
	overlay_veneer Func_08020120
	.global Func_02005594
	.thumb_func
Func_02005594:
	overlay_veneer Object_SetPosition
	.global Func_0200559c
	.thumb_func
Func_0200559c:
	overlay_veneer Object_CommitPosition
	.global Func_020055a4
	.thumb_func
Func_020055a4:
	overlay_veneer Func_08020178
	.global Func_020055ac
	.thumb_func
Func_020055ac:
	overlay_veneer Func_080201e8
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_020055c4
	.thumb_func
Func_020055c4:
	overlay_veneer Func_08020228
	.global Func_020055cc
	.thumb_func
Func_020055cc:
	overlay_veneer Func_08020230
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_020055dc
	.thumb_func
Func_020055dc:
	overlay_veneer Func_08020390
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Func_020055ec
	.thumb_func
Func_020055ec:
	overlay_veneer Func_08038248
	.global PartyInventory_Remove
	.thumb_func
PartyInventory_Remove:
	overlay_veneer PartyInventory_RemoveFar
	.global Func_020055fc
	.thumb_func
Func_020055fc:
	overlay_veneer Func_080ad208
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_0200560c
	.thumb_func
Func_0200560c:
	overlay_veneer Func_080c8018
	.global Func_02005614
	.thumb_func
Func_02005614:
	overlay_veneer Func_080c8020
	.global Event_PrepareObjectAndApplyValue
	.thumb_func
Event_PrepareObjectAndApplyValue:
	overlay_veneer Event_PrepareObjectAndApplyValueFar
	.global Party_RemoveOwnerRestored
	.thumb_func
Party_RemoveOwnerRestored:
	overlay_veneer Party_RemoveOwnerRestoredFar
	.global Inventory_PromptAndSetObjectMode
	.thumb_func
Inventory_PromptAndSetObjectMode:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Func_0200563c
	.thumb_func
Func_0200563c:
	overlay_veneer Func_080c8090
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_EnableActionAndSetCallback
	.thumb_func
ObjectMotion_EnableActionAndSetCallback:
	overlay_veneer ObjectMotion_EnableActionAndSetCallbackFar
	.global ObjectMotion_EnableActionAndResetMotion
	.thumb_func
ObjectMotion_EnableActionAndResetMotion:
	overlay_veneer ObjectMotion_EnableActionAndResetMotionFar
	.global Object_SetActionCallbackAndRefreshById
	.thumb_func
Object_SetActionCallbackAndRefreshById:
	overlay_veneer Object_SetActionCallbackAndRefreshByIdFar
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_0200567c
	.thumb_func
Func_0200567c:
	overlay_veneer Func_080c80f8
	.global Func_02005684
	.thumb_func
Func_02005684:
	overlay_veneer Func_080c8100
	.global Func_0200568c
	.thumb_func
Func_0200568c:
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
	.global ObjectMotion_SetVariantCallback
	.thumb_func
ObjectMotion_SetVariantCallback:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Func_020056cc
	.thumb_func
Func_020056cc:
	overlay_veneer Func_080c8168
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_020056dc
	.thumb_func
Func_020056dc:
	overlay_veneer Func_080c8180
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_020056ec
	.thumb_func
Func_020056ec:
	overlay_veneer Func_080c8198
	.global Func_020056f4
	.thumb_func
Func_020056f4:
	overlay_veneer Func_080c81a0
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_02005704
	.thumb_func
Func_02005704:
	overlay_veneer Func_080c81d8
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02005714
	.thumb_func
Func_02005714:
	overlay_veneer Func_080c8210
	.global Func_0200571c
	.thumb_func
Func_0200571c:
	overlay_veneer Func_080c8218
	.global Func_02005724
	.thumb_func
Func_02005724:
	overlay_veneer Func_080c8230
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_02005734
	.thumb_func
Func_02005734:
	overlay_veneer Func_080c8240
	.global Func_0200573c
	.thumb_func
Func_0200573c:
	overlay_veneer Func_080c8258
	.global Func_02005744
	.thumb_func
Func_02005744:
	overlay_veneer Func_080c8278
	.global Func_0200574c
	.thumb_func
Func_0200574c:
	overlay_veneer Func_080c8280
	.global Party_SetFields1eeAnd1f0
	.thumb_func
Party_SetFields1eeAnd1f0:
	overlay_veneer Party_SetFields1eeAnd1f0Far
	.global Func_0200575c
	.thumb_func
Func_0200575c:
	overlay_veneer Func_080c8378
	.global Func_02005764
	.thumb_func
Func_02005764:
	overlay_veneer Func_080c8380
	.global Func_0200576c
	.thumb_func
Func_0200576c:
	overlay_veneer Func_080c8390
	.global Event_SetStatus1c6
	.thumb_func
Event_SetStatus1c6:
	overlay_veneer Event_SetStatus1c6Far
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global Func_02005784
	.thumb_func
Func_02005784:
	overlay_veneer Func_080c84e0
	.global Func_0200578c
	.thumb_func
Func_0200578c:
	overlay_veneer Func_080c84e8
	.global Func_02005794
	.thumb_func
Func_02005794:
	overlay_veneer Func_080c84f0
	.global Func_0200579c
	.thumb_func
Func_0200579c:
	overlay_veneer Func_080c85c0
	.global Func_020057a4
	.thumb_func
Func_020057a4:
	overlay_veneer Func_080c87c0
	.global Func_020057ac
	.thumb_func
Func_020057ac:
	overlay_veneer Func_080c87c8
	.global Func_020057b4
	.thumb_func
Func_020057b4:
	overlay_veneer Func_080c87e8
	.global Func_020057bc
	.thumb_func
Func_020057bc:
	overlay_veneer Func_080c87f0
	.global Func_020057c4
	.thumb_func
Func_020057c4:
	overlay_veneer Func_080c88d8
	.global Func_020057cc
	.thumb_func
Func_020057cc:
	overlay_veneer Func_080c88e8
	.global Func_020057d4
	.thumb_func
Func_020057d4:
	overlay_veneer Func_080c8918
	.global Func_020057dc
	.thumb_func
Func_020057dc:
	overlay_veneer Audio_PlayCue
