@ Import veneers after the OVERLAY_661 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_661 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_MathDivide
	.thumb_func
Engine_MathDivide:
	overlay_veneer IwramSignedDivide
	.global IwramUnsignedDivideEntry
	.thumb_func
IwramUnsignedDivideEntry:
	overlay_veneer IwramUnsignedDivide
	.global Engine_MathModulo
	.thumb_func
Engine_MathModulo:
	overlay_veneer IwramUnsignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Func_02003f28
	.thumb_func
Func_02003f28:
	overlay_veneer Func_080000d0
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
	.global Func_02003f60
	.thumb_func
Func_02003f60:
	overlay_veneer Func_08000170
	.global Sys_Free
	.thumb_func
Sys_Free:
	overlay_veneer Sys_FreeFar
	.global Func_02003f70
	.thumb_func
Func_02003f70:
	overlay_veneer Func_080001a8
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02003f88
	.thumb_func
Func_02003f88:
	overlay_veneer Func_080001e8
	.global Func_02003f90
	.thumb_func
Func_02003f90:
	overlay_veneer Func_080003c8
	.global Func_02003f98
	.thumb_func
Func_02003f98:
	overlay_veneer Func_080003d0
	.global Func_02003fa0
	.thumb_func
Func_02003fa0:
	overlay_veneer Func_080003d8
	.global Func_02003fa8
	.thumb_func
Func_02003fa8:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_ApplyValueToChildren
	.thumb_func
ObjectDispatch_ApplyValueToChildren:
	overlay_veneer ObjectDispatch_ApplyValueToChildrenFar
	.global Func_02003fb8
	.thumb_func
Func_02003fb8:
	overlay_veneer Object_SetCallback
	.global Func_02003fc0
	.thumb_func
Func_02003fc0:
	overlay_veneer Func_080200c0
	.global Func_02003fc8
	.thumb_func
Func_02003fc8:
	overlay_veneer Func_080200c8
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02003fd8
	.thumb_func
Func_02003fd8:
	overlay_veneer Func_08020120
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
	overlay_veneer Func_08020178
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02004000
	.thumb_func
Func_02004000:
	overlay_veneer Func_080201e8
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
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
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02004040
	.thumb_func
Func_02004040:
	overlay_veneer Func_080c8018
	.global Func_02004048
	.thumb_func
Func_02004048:
	overlay_veneer Func_080c8020
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
	overlay_veneer Func_080c80f8
	.global Func_02004098
	.thumb_func
Func_02004098:
	overlay_veneer Func_080c8100
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
	.global Func_020040b8
	.thumb_func
Func_020040b8:
	overlay_veneer Func_080c8158
	.global Func_020040c0
	.thumb_func
Func_020040c0:
	overlay_veneer Func_080c8170
	.global Func_020040c8
	.thumb_func
Func_020040c8:
	overlay_veneer Func_080c8180
	.global Func_020040d0
	.thumb_func
Func_020040d0:
	overlay_veneer Func_080c81a0
	.global Func_020040d8
	.thumb_func
Func_020040d8:
	overlay_veneer Func_080c81a8
	.global Func_020040e0
	.thumb_func
Func_020040e0:
	overlay_veneer Func_080c81d0
	.global Func_020040e8
	.thumb_func
Func_020040e8:
	overlay_veneer Func_080c81d8
	.global Func_020040f0
	.thumb_func
Func_020040f0:
	overlay_veneer Func_080c8200
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
	.global Func_02004110
	.thumb_func
Func_02004110:
	overlay_veneer Func_080c8228
	.global Func_02004118
	.thumb_func
Func_02004118:
	overlay_veneer Func_080c8230
	.global Func_02004120
	.thumb_func
Func_02004120:
	overlay_veneer Func_080c8238
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
	overlay_veneer Func_080c8278
	.global Func_02004140
	.thumb_func
Func_02004140:
	overlay_veneer Func_080c8280
	.global Func_02004148
	.thumb_func
Func_02004148:
	overlay_veneer Func_080c8288
	.global Func_02004150
	.thumb_func
Func_02004150:
	overlay_veneer Func_080c8290
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
	.global Func_02004170
	.thumb_func
Func_02004170:
	overlay_veneer Func_080c8420
	.global Func_02004178
	.thumb_func
Func_02004178:
	overlay_veneer Func_080c8428
	.global Func_02004180
	.thumb_func
Func_02004180:
	overlay_veneer Func_080c8480
	.global Func_02004188
	.thumb_func
Func_02004188:
	overlay_veneer Func_080c8498
	.global Func_02004190
	.thumb_func
Func_02004190:
	overlay_veneer Func_080c84a0
	.global Func_02004198
	.thumb_func
Func_02004198:
	overlay_veneer Func_080c84a8
	.global Func_020041a0
	.thumb_func
Func_020041a0:
	overlay_veneer Func_080c84b0
	.global Func_020041a8
	.thumb_func
Func_020041a8:
	overlay_veneer Func_080c84b8
	.global Func_020041b0
	.thumb_func
Func_020041b0:
	overlay_veneer Func_080c84c0
	.global Func_020041b8
	.thumb_func
Func_020041b8:
	overlay_veneer Func_080c84e0
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
	overlay_veneer Func_080c8778
	.global Func_020041f8
	.thumb_func
Func_020041f8:
	overlay_veneer Audio_PlayCue
