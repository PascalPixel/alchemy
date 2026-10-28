.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
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
	.global Runtime_BumpAllocateAlternatePool
	.thumb_func
Runtime_BumpAllocateAlternatePool:
	overlay_veneer Runtime_BumpAllocateAlternatePoolFar
	.global MemFree
	.thumb_func
MemFree:
	overlay_veneer Sys_FreeFar
	.global Func_02003f70
	.thumb_func
Func_02003f70:
	overlay_veneer Resource_DecodeType01Far
	.global VramCache_Load
	.thumb_func
VramCache_Load:
	overlay_veneer VramCache_LoadFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02003f88
	.thumb_func
Func_02003f88:
	overlay_veneer Oam_AddSpriteAtDepthFar
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
	.global Func_02003fa8
	.thumb_func
Func_02003fa8:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_SetAnimationSpeed
	.thumb_func
ObjectDispatch_SetAnimationSpeed:
	overlay_veneer ObjectDispatch_SetAnimationSpeedFar
	.global Func_02003fb8
	.thumb_func
Func_02003fb8:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02003fc0
	.thumb_func
Func_02003fc0:
	overlay_veneer Func_080200c0
	.global Func_02003fc8
	.thumb_func
Func_02003fc8:
	overlay_veneer Object_Destroy
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02003fd8
	.thumb_func
Func_02003fd8:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02003fe0
	.thumb_func
Func_02003fe0:
	overlay_veneer Object_SetPosition
	.global Func_02003fe8
	.thumb_func
Func_02003fe8:
	overlay_veneer Object_CommitPosition
	.global Func_02003ff0
	.thumb_func
Func_02003ff0:
	overlay_veneer Map_CopyCellPictureIndicesRectFar
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02004000
	.thumb_func
Func_02004000:
	overlay_veneer Map_CopyCellAttributeRectFar
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Map_EnableUpdateCallback
	.thumb_func
Map_EnableUpdateCallback:
	overlay_veneer Map_EnableUpdateCallbackFar
	.global Map_DisableUpdateCallback
	.thumb_func
Map_DisableUpdateCallback:
	overlay_veneer Map_DisableUpdateCallbackFar
	.global Func_02004020
	.thumb_func
Func_02004020:
	overlay_veneer Func_080202f0
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Func_02004030
	.thumb_func
Func_02004030:
	overlay_veneer Func_08038248
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Func_02004040
	.thumb_func
Func_02004040:
	overlay_veneer EventRuntime_StartFar
	.global Func_02004048
	.thumb_func
Func_02004048:
	overlay_veneer EventRuntime_EndFar
	.global PartyInventory_GiveItem
	.thumb_func
PartyInventory_GiveItem:
	overlay_veneer PartyInventory_GiveItemFar
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
	.global Func_02004090
	.thumb_func
Func_02004090:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Func_02004098
	.thumb_func
Func_02004098:
	overlay_veneer ObjectMotion_SetHorizontalPositionAndAngleWithTerrainFar
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
	.global Func_020040c8
	.thumb_func
Func_020040c8:
	overlay_veneer EventRuntime_SetMessageFar
	.global Func_020040d0
	.thumb_func
Func_020040d0:
	overlay_veneer EventRuntime_ShowMessageAndWaitFar
	.global Func_020040d8
	.thumb_func
Func_020040d8:
	overlay_veneer Func_080c81a8
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_020040e8
	.thumb_func
Func_020040e8:
	overlay_veneer ObjectMotion_ArmCallbackAndRefreshFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_020040f8
	.thumb_func
Func_020040f8:
	overlay_veneer Func_080c8208
	.global Func_02004100
	.thumb_func
Func_02004100:
	overlay_veneer Func_080c8210
	.global Func_02004108
	.thumb_func
Func_02004108:
	overlay_veneer Func_080c8218
	.global Camera_FollowActor
	.thumb_func
Camera_FollowActor:
	overlay_veneer Camera_FollowActorFar
	.global Func_02004118
	.thumb_func
Func_02004118:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_02004128
	.thumb_func
Func_02004128:
	overlay_veneer Func_080c8240
	.global Func_02004130
	.thumb_func
Func_02004130:
	overlay_veneer Func_080c8258
	.global Func_02004138
	.thumb_func
Func_02004138:
	overlay_veneer EventRuntime_SetCountdownFar
	.global Func_02004140
	.thumb_func
Func_02004140:
	overlay_veneer Field_RequestBattleFromEncounterTableFar
	.global Func_02004148
	.thumb_func
Func_02004148:
	overlay_veneer Func_080c8288
	.global Party_SetFields1eeAnd1f0
	.thumb_func
Party_SetFields1eeAnd1f0:
	overlay_veneer Party_SetFields1eeAnd1f0Far
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
	.global Func_02004178
	.thumb_func
Func_02004178:
	overlay_veneer Object_ReturnHomeFar
	.global Func_02004180
	.thumb_func
Func_02004180:
	overlay_veneer Func_080c8480
	.global Field_DispatchTypeHandler
	.thumb_func
Field_DispatchTypeHandler:
	overlay_veneer Field_DispatchTypeHandlerFar
	.global Func_02004190
	.thumb_func
Func_02004190:
	overlay_veneer BattleEffect_LoadActionEffectResourcesFar
	.global Func_02004198
	.thumb_func
Func_02004198:
	overlay_veneer BattleEffect_SetupObjectPairFar
	.global Func_020041a0
	.thumb_func
Func_020041a0:
	overlay_veneer EffectRuntime_StopCurrentObjectFar
	.global Func_020041a8
	.thumb_func
Func_020041a8:
	overlay_veneer BattleEffect_CleanupSceneObjectsFar
	.global Func_020041b0
	.thumb_func
Func_020041b0:
	overlay_veneer EventObject_InitializeFar
	.global Func_020041b8
	.thumb_func
Func_020041b8:
	overlay_veneer EventRuntime_ResolvePendingActionsFar
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_020041c8
	.thumb_func
Func_020041c8:
	overlay_veneer Func_080c8688
	.global Func_020041d0
	.thumb_func
Func_020041d0:
	overlay_veneer Func_080c86a8
	.global Func_020041d8
	.thumb_func
Func_020041d8:
	overlay_veneer Func_080c86e8
	.global Func_020041e0
	.thumb_func
Func_020041e0:
	overlay_veneer Func_080c8710
	.global Func_020041e8
	.thumb_func
Func_020041e8:
	overlay_veneer Func_080c8730
	.global Func_020041f0
	.thumb_func
Func_020041f0:
	overlay_veneer EventRuntime_GetControlledOwnerFar
	.global Func_020041f8
	.thumb_func
Func_020041f8:
	overlay_veneer Sound_PlayFar
