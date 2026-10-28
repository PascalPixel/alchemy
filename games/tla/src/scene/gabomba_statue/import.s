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
	.global Heap_Alloc
	.thumb_func
Heap_Alloc:
	overlay_veneer Heap_AllocFar
	.global Heap_AllocEwram
	.thumb_func
Heap_AllocEwram:
	overlay_veneer Heap_AllocEwramFar
	.global Heap_Release
	.thumb_func
Heap_Release:
	overlay_veneer Sys_FreeFar
	.global Func_020055cc
	.thumb_func
Func_020055cc:
	overlay_veneer Resource_DecodeType01Far
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
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
	overlay_veneer Oam_AddSpriteAtDepthFar
	.global Resource_GetTableEntry
	.thumb_func
Resource_GetTableEntry:
	overlay_veneer Resource_GetTableEntryFar
	.global Unit_Get
	.thumb_func
Unit_Get:
	overlay_veneer Unit_GetFar
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
	.global ObjectDispatch_SetAnimationSpeed
	.thumb_func
ObjectDispatch_SetAnimationSpeed:
	overlay_veneer ObjectDispatch_SetAnimationSpeedFar
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
	overlay_veneer Map_CenterOnFollowedPointFar
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
	overlay_veneer Map_CopyCellsRectFar
	.global Func_02005674
	.thumb_func
Func_02005674:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global Func_0200567c
	.thumb_func
Func_0200567c:
	overlay_veneer Func_080201f0
	.global Func_02005684
	.thumb_func
Func_02005684:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Func_02005694
	.thumb_func
Func_02005694:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_0200569c
	.thumb_func
Func_0200569c:
	overlay_veneer Func_08020230
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_020056b4
	.thumb_func
Func_020056b4:
	overlay_veneer Event_StartFar
	.global Func_020056bc
	.thumb_func
Func_020056bc:
	overlay_veneer Event_EndFar
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
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_0200571c
	.thumb_func
Func_0200571c:
	overlay_veneer ObjectMotion_SetHorizontalPositionAndAngleWithTerrainFar
	.global Func_02005724
	.thumb_func
Func_02005724:
	overlay_veneer ObjectMotion_MoveToObjectFar
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
	overlay_veneer Event_SetMessageIdFar
	.global FieldMessage_ShowAtActor
	.thumb_func
FieldMessage_ShowAtActor:
	overlay_veneer FieldMessage_ShowAtActorFar
	.global Func_02005764
	.thumb_func
Func_02005764:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_02005774
	.thumb_func
Func_02005774:
	overlay_veneer ObjectMotion_ArmCallbackAndRefreshFar
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
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Func_020057a4
	.thumb_func
Func_020057a4:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
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
	overlay_veneer Event_SetExitRequestFar
	.global Func_020057cc
	.thumb_func
Func_020057cc:
	overlay_veneer Func_080c8288
	.global Func_020057d4
	.thumb_func
Func_020057d4:
	overlay_veneer Leader_StopMovingFar
	.global Func_020057dc
	.thumb_func
Func_020057dc:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_020057e4
	.thumb_func
Func_020057e4:
	overlay_veneer BattleEffect_ApplyColorToSourceBufferFar
	.global Func_020057ec
	.thumb_func
Func_020057ec:
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
	.global Func_0200580c
	.thumb_func
Func_0200580c:
	overlay_veneer Event_ResolvePendingActionsFar
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_0200581c
	.thumb_func
Func_0200581c:
	overlay_veneer ObjectTable_SnapshotFar
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
	overlay_veneer Func_080c86e0
	.global Func_0200583c
	.thumb_func
Func_0200583c:
	overlay_veneer Func_080c86e8
	.global Func_02005844
	.thumb_func
Func_02005844:
	overlay_veneer Func_080c8710
	.global Func_0200584c
	.thumb_func
Func_0200584c:
	overlay_veneer Party_GetLeaderActorFar
	.global Func_02005854
	.thumb_func
Func_02005854:
	overlay_veneer Sound_PlayFar
