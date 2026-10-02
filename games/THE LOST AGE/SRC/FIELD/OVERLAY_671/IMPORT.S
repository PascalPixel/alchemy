@ Import veneers after the OVERLAY_671 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_671 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global __modsi3
	.thumb_func
__modsi3:
	overlay_veneer IwramSignedRemainder
	.global __udivsi3
	.thumb_func
__udivsi3:
	overlay_veneer IwramUnsignedDivide
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
	.global Runtime_BumpAllocate
	.thumb_func
Runtime_BumpAllocate:
	overlay_veneer Runtime_BumpAllocateFar
	.global Runtime_BumpAllocateAlternatePool
	.thumb_func
Runtime_BumpAllocateAlternatePool:
	overlay_veneer Runtime_BumpAllocateAlternatePoolFar
	.global Sys_Free
	.thumb_func
Sys_Free:
	overlay_veneer Sys_FreeFar
	.global Func_020055cc
	.thumb_func
Func_020055cc:
	overlay_veneer Func_080001a8
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global AffineMatrix_BuildForEffect
	.thumb_func
AffineMatrix_BuildForEffect:
	overlay_veneer AffineMatrix_BuildForEffectFar
	.global Func_020055ec
	.thumb_func
Func_020055ec:
	overlay_veneer Func_080001e8
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
	.global Owner_GetState
	.thumb_func
Owner_GetState:
	overlay_veneer Owner_GetStateFar
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
	.global Func_0200561c
	.thumb_func
Func_0200561c:
	overlay_veneer Func_08020038
	.global Func_02005624
	.thumb_func
Func_02005624:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_ApplyValueToChildren
	.thumb_func
ObjectDispatch_ApplyValueToChildren:
	overlay_veneer ObjectDispatch_ApplyValueToChildrenFar
	.global Func_02005634
	.thumb_func
Func_02005634:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_0200563c
	.thumb_func
Func_0200563c:
	overlay_veneer Func_080200c0
	.global Func_02005644
	.thumb_func
Func_02005644:
	overlay_veneer Object_Destroy
	.global Func_0200564c
	.thumb_func
Func_0200564c:
	overlay_veneer Func_08020120
	.global Func_02005654
	.thumb_func
Func_02005654:
	overlay_veneer Object_ResetMotion
	.global Func_0200565c
	.thumb_func
Func_0200565c:
	overlay_veneer Object_SetPosition
	.global Func_02005664
	.thumb_func
Func_02005664:
	overlay_veneer Object_CommitPosition
	.global Func_0200566c
	.thumb_func
Func_0200566c:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_080201b0
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080201b0
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_080201b0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080201b0
	.else
	overlay_veneer Func_080201e0
	.endif
	.endif
	.endif
	.endif
	.global Func_02005674
	.thumb_func
Func_02005674:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_080201b8
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080201b8
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_080201b8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080201b8
	.else
	overlay_veneer Func_080201e8
	.endif
	.endif
	.endif
	.endif
	.global Func_0200567c
	.thumb_func
Func_0200567c:
	.ifdef TLA_EDITION_DE
	overlay_veneer Map_GetTerrainHeightFar
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Map_GetTerrainHeightFar
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Map_GetTerrainHeightFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Map_GetTerrainHeightFar
	.else
	overlay_veneer Func_080201f0
	.endif
	.endif
	.endif
	.endif
	.global Func_02005684
	.thumb_func
Func_02005684:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Func_02005694
	.thumb_func
Func_02005694:
	overlay_veneer Func_08020228
	.global Func_0200569c
	.thumb_func
Func_0200569c:
	overlay_veneer Func_08020230
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_020056b4
	.thumb_func
Func_020056b4:
	overlay_veneer Func_080c8018
	.global Func_020056bc
	.thumb_func
Func_020056bc:
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
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global Func_02005714
	.thumb_func
Func_02005714:
	overlay_veneer Func_080c80f8
	.global Func_0200571c
	.thumb_func
Func_0200571c:
	overlay_veneer Func_080c8100
	.global Func_02005724
	.thumb_func
Func_02005724:
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
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02005754
	.thumb_func
Func_02005754:
	overlay_veneer Func_080c8180
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_02005764
	.thumb_func
Func_02005764:
	overlay_veneer Func_080c81a0
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_02005774
	.thumb_func
Func_02005774:
	overlay_veneer Func_080c81d8
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02005784
	.thumb_func
Func_02005784:
	overlay_veneer Func_080c8208
	.global Func_0200578c
	.thumb_func
Func_0200578c:
	overlay_veneer Func_080c8210
	.global Func_02005794
	.thumb_func
Func_02005794:
	overlay_veneer Func_080c8218
	.global Object_AttachWorkTargetToObject
	.thumb_func
Object_AttachWorkTargetToObject:
	overlay_veneer Object_AttachWorkTargetToObjectFar
	.global Func_020057a4
	.thumb_func
Func_020057a4:
	overlay_veneer Func_080c8230
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_020057b4
	.thumb_func
Func_020057b4:
	overlay_veneer Func_080c8240
	.global Func_020057bc
	.thumb_func
Func_020057bc:
	overlay_veneer Func_080c8258
	.global Func_020057c4
	.thumb_func
Func_020057c4:
	overlay_veneer Func_080c8278
	.global Func_020057cc
	.thumb_func
Func_020057cc:
	overlay_veneer Func_080c8288
	.global Func_020057d4
	.thumb_func
Func_020057d4:
	overlay_veneer Func_080c82f8
	.global Func_020057dc
	.thumb_func
Func_020057dc:
	overlay_veneer Func_080c8378
	.global Func_020057e4
	.thumb_func
Func_020057e4:
	overlay_veneer Func_080c8380
	.global Func_020057ec
	.thumb_func
Func_020057ec:
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
	.ifdef TLA_EDITION_ES
	overlay_veneer Event_SetStatus1c6Far
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Event_SetStatus1c6Far
	.else
	overlay_veneer Event_WaitValue1c8FramesFar
	.endif
	.endif
	.global Func_0200580c
	.thumb_func
Func_0200580c:
	overlay_veneer Func_080c84e0
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_0200581c
	.thumb_func
Func_0200581c:
	overlay_veneer Func_080c8628
	.global Func_02005824
	.thumb_func
Func_02005824:
	overlay_veneer Func_080c86a8
	.global Func_0200582c
	.thumb_func
Func_0200582c:
	overlay_veneer Func_080c86b8
	.global Func_02005834
	.thumb_func
Func_02005834:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c86d0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c86d0
	.else
	overlay_veneer Func_080c86e0
	.endif
	.endif
	.global Func_0200583c
	.thumb_func
Func_0200583c:
	overlay_veneer Func_080c86e8
	.global Func_02005844
	.thumb_func
Func_02005844:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8700
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8700
	.else
	overlay_veneer Func_080c8710
	.endif
	.endif
	.global Func_0200584c
	.thumb_func
Func_0200584c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8768
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8768
	.else
	overlay_veneer Func_080c8778
	.endif
	.endif
	.global Func_02005854
	.thumb_func
Func_02005854:
	overlay_veneer Audio_PlayCue
