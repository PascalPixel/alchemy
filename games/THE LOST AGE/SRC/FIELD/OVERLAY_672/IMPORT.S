@ Import veneers after the OVERLAY_672 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_672 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Runtime_AllocateHeapBlockFar
	.thumb_func
Runtime_AllocateHeapBlockFar:
	overlay_veneer Func_08000140
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	overlay_veneer Runtime_ReleaseHeapBlockFar
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
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
	.global Func_02001d28
	.thumb_func
Func_02001d28:
	overlay_veneer Func_080200c0
	.global Func_02001d30
	.thumb_func
Func_02001d30:
	overlay_veneer Object_Destroy
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02001d40
	.thumb_func
Func_02001d40:
	overlay_veneer Func_08020120
	.global Func_02001d48
	.thumb_func
Func_02001d48:
	overlay_veneer Object_SetPosition
	.global Func_02001d50
	.thumb_func
Func_02001d50:
	overlay_veneer Object_CommitPosition
	.global Func_02001d58
	.thumb_func
Func_02001d58:
	overlay_veneer Func_08020178
	.global Func_02001d60
	.thumb_func
Func_02001d60:
	overlay_veneer Func_080201b0
	.global Func_02001d68
	.thumb_func
Func_02001d68:
	overlay_veneer Func_080201e8
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Func_02001d78
	.thumb_func
Func_02001d78:
	overlay_veneer Func_08020240
	.global Func_02001d80
	.thumb_func
Func_02001d80:
	overlay_veneer Func_08020248
	.global Func_02001d88
	.thumb_func
Func_02001d88:
	overlay_veneer Func_08038210
	.global Func_02001d90
	.thumb_func
Func_02001d90:
	overlay_veneer Func_08038248
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02001da0
	.thumb_func
Func_02001da0:
	overlay_veneer Func_080c8018
	.global Func_02001da8
	.thumb_func
Func_02001da8:
	overlay_veneer Func_080c8020
	.global PartyInventory_GiveItem
	.thumb_func
PartyInventory_GiveItem:
	overlay_veneer PartyInventory_GiveItemFar
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
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
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
	.global Func_02001e10
	.thumb_func
Func_02001e10:
	overlay_veneer Func_080c80f8
	.global Func_02001e18
	.thumb_func
Func_02001e18:
	overlay_veneer Func_080c8100
	.global Func_02001e20
	.thumb_func
Func_02001e20:
	overlay_veneer Func_080c8110
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global ObjectMotion_Launch
	.thumb_func
ObjectMotion_Launch:
	overlay_veneer ObjectMotion_LaunchFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02001e48
	.thumb_func
Func_02001e48:
	overlay_veneer Func_080c8180
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_02001e58
	.thumb_func
Func_02001e58:
	overlay_veneer Func_080c8198
	.global Func_02001e60
	.thumb_func
Func_02001e60:
	overlay_veneer Func_080c81a0
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_02001e70
	.thumb_func
Func_02001e70:
	overlay_veneer Func_080c81d8
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02001e80
	.thumb_func
Func_02001e80:
	overlay_veneer Func_080c8208
	.global Func_02001e88
	.thumb_func
Func_02001e88:
	overlay_veneer Func_080c8210
	.global Func_02001e90
	.thumb_func
Func_02001e90:
	overlay_veneer Func_080c8218
	.global Func_02001e98
	.thumb_func
Func_02001e98:
	overlay_veneer Func_080c8230
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_02001ea8
	.thumb_func
Func_02001ea8:
	overlay_veneer Func_080c8240
	.global Func_02001eb0
	.thumb_func
Func_02001eb0:
	overlay_veneer Func_080c8278
	.global Func_02001eb8
	.thumb_func
Func_02001eb8:
	overlay_veneer Func_080c8378
	.global Func_02001ec0
	.thumb_func
Func_02001ec0:
	overlay_veneer Func_080c8380
	.global Func_02001ec8
	.thumb_func
Func_02001ec8:
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
	.global Func_02001ee8
	.thumb_func
Func_02001ee8:
	overlay_veneer Func_080c84e0
	.global Func_02001ef0
	.thumb_func
Func_02001ef0:
	overlay_veneer Func_080c8570
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_02001f00
	.thumb_func
Func_02001f00:
	overlay_veneer Func_080c8778
	.global Func_02001f08
	.thumb_func
Func_02001f08:
	overlay_veneer Func_080c8930
	.global Func_02001f10
	.thumb_func
Func_02001f10:
	overlay_veneer Audio_PlayCue
