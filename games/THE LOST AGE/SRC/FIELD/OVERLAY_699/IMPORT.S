@ Import veneers after the OVERLAY_699 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_699 overlay import veneers
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
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Scheduler_AddOrUpdateCallback
	.thumb_func
Scheduler_AddOrUpdateCallback:
	overlay_veneer Scheduler_AddOrUpdateCallbackFar
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
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global Runtime_BumpAllocate
	.thumb_func
Runtime_BumpAllocate:
	overlay_veneer Runtime_BumpAllocateFar
	.global Sys_Free
	.thumb_func
Sys_Free:
	overlay_veneer Sys_FreeFar
	.global Func_02001bac
	.thumb_func
Func_02001bac:
	overlay_veneer Func_080001a8
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	overlay_veneer VramBlock_LoadCachedFar
	.global Resource_FindFreeEntry
	.thumb_func
Resource_FindFreeEntry:
	overlay_veneer Resource_FindFreeEntryFar
	.global Func_02001bc4
	.thumb_func
Func_02001bc4:
	overlay_veneer Func_080001e8
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global GameFlag_GetByte
	.thumb_func
GameFlag_GetByte:
	overlay_veneer GameFlag_GetByteFar
	.global GameFlag_SetByte
	.thumb_func
GameFlag_SetByte:
	overlay_veneer GameFlag_SetByteFar
	.global Func_02001bec
	.thumb_func
Func_02001bec:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_ApplyValueToChildren
	.thumb_func
ObjectDispatch_ApplyValueToChildren:
	overlay_veneer ObjectDispatch_ApplyValueToChildrenFar
	.global Func_02001bfc
	.thumb_func
Func_02001bfc:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02001c04
	.thumb_func
Func_02001c04:
	overlay_veneer Func_080200c0
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02001c14
	.thumb_func
Func_02001c14:
	overlay_veneer Func_08020120
	.global Func_02001c1c
	.thumb_func
Func_02001c1c:
	overlay_veneer Object_SetPosition
	.global Func_02001c24
	.thumb_func
Func_02001c24:
	overlay_veneer Object_CommitPosition
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02001c34
	.thumb_func
Func_02001c34:
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
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02001c4c
	.thumb_func
Func_02001c4c:
	overlay_veneer Func_08020228
	.global Func_02001c54
	.thumb_func
Func_02001c54:
	overlay_veneer Func_08020230
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Map_EnableUpdateCallback
	.thumb_func
Map_EnableUpdateCallback:
	overlay_veneer Map_EnableUpdateCallbackFar
	.global Map_DisableUpdateCallback
	.thumb_func
Map_DisableUpdateCallback:
	overlay_veneer Map_DisableUpdateCallbackFar
	.global Func_02001c74
	.thumb_func
Func_02001c74:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_08020330
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_08020330
	.else
	overlay_veneer Func_08020360
	.endif
	.endif
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02001c84
	.thumb_func
Func_02001c84:
	overlay_veneer Func_080c8018
	.global Func_02001c8c
	.thumb_func
Func_02001c8c:
	overlay_veneer Func_080c8020
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_SetPositionAndCommit
	.thumb_func
ObjectMotion_SetPositionAndCommit:
	overlay_veneer ObjectMotion_SetPositionAndCommitFar
	.global Func_02001cac
	.thumb_func
Func_02001cac:
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02001cc4
	.thumb_func
Func_02001cc4:
	overlay_veneer Func_080c8218
	.global Object_AttachWorkTargetToObject
	.thumb_func
Object_AttachWorkTargetToObject:
	overlay_veneer Object_AttachWorkTargetToObjectFar
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_02001cdc
	.thumb_func
Func_02001cdc:
	overlay_veneer Func_080c8258
	.global Func_02001ce4
	.thumb_func
Func_02001ce4:
	overlay_veneer Func_080c8278
	.global Func_02001cec
	.thumb_func
Func_02001cec:
	overlay_veneer Func_080c8280
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
	.global Func_02001d0c
	.thumb_func
Func_02001d0c:
	overlay_veneer Func_080c84e0
	.global Func_02001d14
	.thumb_func
Func_02001d14:
	overlay_veneer Func_080c85c0
	.global Func_02001d1c
	.thumb_func
Func_02001d1c:
	overlay_veneer Func_080c8760
	.global Func_02001d24
	.thumb_func
Func_02001d24:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87d8
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87d8
	.else
	overlay_veneer Func_080c87e8
	.endif
	.endif
	.global Func_02001d2c
	.thumb_func
Func_02001d2c:
	overlay_veneer Func_080c8830
	.global Func_02001d34
	.thumb_func
Func_02001d34:
	overlay_veneer Func_080c8848
	.global Func_02001d3c
	.thumb_func
Func_02001d3c:
	overlay_veneer Func_080c8880
	.global Func_02001d44
	.thumb_func
Func_02001d44:
	overlay_veneer Func_080c8948
	.global Func_02001d4c
	.thumb_func
Func_02001d4c:
	overlay_veneer Audio_PlayCue
