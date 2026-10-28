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
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.global Math_Cosine
	.thumb_func
Math_Cosine:
	overlay_veneer Math_CosineFar
	.global HeapSlot_AllocFar
	.thumb_func
HeapSlot_AllocFar:
	overlay_veneer Func_08000140
	.global HeapSlot_Release
	.thumb_func
HeapSlot_Release:
	overlay_veneer HeapSlot_ReleaseFar
	.global Heap_AllocEwram
	.thumb_func
Heap_AllocEwram:
	overlay_veneer Heap_AllocEwramFar
	.global Heap_Release
	.thumb_func
Heap_Release:
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
	overlay_veneer Actor_SetMode
	.global Actor_SetAnimationSpeed
	.thumb_func
Actor_SetAnimationSpeed:
	overlay_veneer Actor_SetAnimationSpeedFar
	.global Func_02003fb8
	.thumb_func
Func_02003fb8:
	overlay_veneer Actor_StartScriptFar
	.global Func_02003fc0
	.thumb_func
Func_02003fc0:
	overlay_veneer Func_080200c0
	.global Func_02003fc8
	.thumb_func
Func_02003fc8:
	overlay_veneer Actor_Destroy
	.global Actor_SetPositionAndResetMotion
	.thumb_func
Actor_SetPositionAndResetMotion:
	overlay_veneer Actor_SetPositionAndResetMotionFar
	.global Func_02003fd8
	.thumb_func
Func_02003fd8:
	overlay_veneer Map_CenterOnFollowedPointFar
	.global Func_02003fe0
	.thumb_func
Func_02003fe0:
	overlay_veneer Actor_SetTargetPosition
	.global Func_02003fe8
	.thumb_func
Func_02003fe8:
	overlay_veneer Actor_CommitPosition
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
	.global Actor_SetSingleChildFlags
	.thumb_func
Actor_SetSingleChildFlags:
	overlay_veneer Actor_SetSingleChildFlagsFar
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
	.global FieldMessage_Show
	.thumb_func
FieldMessage_Show:
	overlay_veneer FieldMessage_ShowFar
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
	overlay_veneer Event_StartFar
	.global Func_02004048
	.thumb_func
Func_02004048:
	overlay_veneer Event_EndFar
	.global PartyInventory_GiveItem
	.thumb_func
PartyInventory_GiveItem:
	overlay_veneer PartyInventory_GiveItemFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetSpeedParameters
	.thumb_func
Actor_SetSpeedParameters:
	overlay_veneer Actor_SetSpeedParametersFar
	.global Actor_SetMovementTypeById
	.thumb_func
Actor_SetMovementTypeById:
	overlay_veneer Actor_SetMovementTypeByIdFar
	.global Actor_Stop
	.thumb_func
Actor_Stop:
	overlay_veneer Actor_StopFar
	.global Actor_SetMovementTypeAndWaitById
	.thumb_func
Actor_SetMovementTypeAndWaitById:
	overlay_veneer Actor_SetMovementTypeAndWaitByIdFar
	.global Actor_SetPositionAndCommit
	.thumb_func
Actor_SetPositionAndCommit:
	overlay_veneer Actor_SetPositionAndCommitFar
	.global Actor_SetPositionAndReset
	.thumb_func
Actor_SetPositionAndReset:
	overlay_veneer Actor_SetPositionAndResetFar
	.global Func_02004090
	.thumb_func
Func_02004090:
	overlay_veneer Actor_SetHorizontalPositionWithTerrainFar
	.global Func_02004098
	.thumb_func
Func_02004098:
	overlay_veneer Actor_SetHorizontalPositionAndAngleWithTerrainFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global Actor_Jump
	.thumb_func
Actor_Jump:
	overlay_veneer Actor_JumpFar
	.global Actor_SetAngleToward
	.thumb_func
Actor_SetAngleToward:
	overlay_veneer Actor_SetAngleTowardFar
	.global Actor_SetPartAttribute
	.thumb_func
Actor_SetPartAttribute:
	overlay_veneer Actor_SetPartAttributeFar
	.global Func_020040c8
	.thumb_func
Func_020040c8:
	overlay_veneer Event_SetMessageIdFar
	.global Func_020040d0
	.thumb_func
Func_020040d0:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global Func_020040d8
	.thumb_func
Func_020040d8:
	overlay_veneer Func_080c81a8
	.global Actor_ArmCallback
	.thumb_func
Actor_ArmCallback:
	overlay_veneer Actor_ArmCallbackFar
	.global Func_020040e8
	.thumb_func
Func_020040e8:
	overlay_veneer Actor_ArmCallbackAndRefreshFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
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
	overlay_veneer Event_SetExitRequestFar
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
	.global Actor_SetWideSprite
	.thumb_func
Actor_SetWideSprite:
	overlay_veneer Actor_SetWideSpriteFar
	.global Func_02004178
	.thumb_func
Func_02004178:
	overlay_veneer Actor_ReturnHomeFar
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
	overlay_veneer BattleEffect_StopMainActorFar
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
	overlay_veneer Event_ResolvePendingActionsFar
	.global Actor_WatchAndSetScript
	.thumb_func
Actor_WatchAndSetScript:
	overlay_veneer Actor_WatchAndSetScriptFar
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
	overlay_veneer Party_GetLeaderActorFar
	.global Func_020041f8
	.thumb_func
Func_020041f8:
	overlay_veneer Sound_PlayFar
