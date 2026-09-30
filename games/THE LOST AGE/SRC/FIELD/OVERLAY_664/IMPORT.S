@ Import veneers after the OVERLAY_664 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_664 overlay import veneers
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
	.global Func_02005d24
	.thumb_func
Func_02005d24:
	overlay_veneer Func_080001a8
	.global Func_02005d2c
	.thumb_func
Func_02005d2c:
	overlay_veneer Func_080001d8
	.global Func_02005d34
	.thumb_func
Func_02005d34:
	overlay_veneer Func_080001f0
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
	.global Func_02005d5c
	.thumb_func
Func_02005d5c:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_ApplyValueToChildren
	.thumb_func
ObjectDispatch_ApplyValueToChildren:
	overlay_veneer ObjectDispatch_ApplyValueToChildrenFar
	.global Func_02005d6c
	.thumb_func
Func_02005d6c:
	overlay_veneer Object_SetCallback
	.global Func_02005d74
	.thumb_func
Func_02005d74:
	overlay_veneer Func_080200c0
	.global Func_02005d7c
	.thumb_func
Func_02005d7c:
	overlay_veneer Func_080200c8
	.global Func_02005d84
	.thumb_func
Func_02005d84:
	overlay_veneer Func_08020120
	.global Func_02005d8c
	.thumb_func
Func_02005d8c:
	overlay_veneer Object_SetPosition
	.global Func_02005d94
	.thumb_func
Func_02005d94:
	overlay_veneer Object_CommitPosition
	.global Func_02005d9c
	.thumb_func
Func_02005d9c:
	overlay_veneer Func_08020178
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02005dac
	.thumb_func
Func_02005dac:
	overlay_veneer Func_080201e8
	.global Func_02005db4
	.thumb_func
Func_02005db4:
	overlay_veneer Func_080201f0
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02005dcc
	.thumb_func
Func_02005dcc:
	overlay_veneer Func_08020228
	.global Func_02005dd4
	.thumb_func
Func_02005dd4:
	overlay_veneer Func_08020230
	.global Map_EnableUpdateCallback
	.thumb_func
Map_EnableUpdateCallback:
	overlay_veneer Map_EnableUpdateCallbackFar
	.global Map_DisableUpdateCallback
	.thumb_func
Map_DisableUpdateCallback:
	overlay_veneer Map_DisableUpdateCallbackFar
	.global Func_02005dec
	.thumb_func
Func_02005dec:
	overlay_veneer Func_080202f8
	.global Func_02005df4
	.thumb_func
Func_02005df4:
	overlay_veneer Func_08020360
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Func_02005e04
	.thumb_func
Func_02005e04:
	overlay_veneer Func_08038348
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02005e14
	.thumb_func
Func_02005e14:
	overlay_veneer Func_080c8018
	.global Func_02005e1c
	.thumb_func
Func_02005e1c:
	overlay_veneer Func_080c8020
	.global UiText_DrawQuantityPairWithCue
	.thumb_func
UiText_DrawQuantityPairWithCue:
	overlay_veneer UiText_DrawQuantityPairWithCueFar
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
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
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
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02005e6c
	.thumb_func
Func_02005e6c:
	overlay_veneer Func_080c80f8
	.global Func_02005e74
	.thumb_func
Func_02005e74:
	overlay_veneer Func_080c8110
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_02005e84
	.thumb_func
Func_02005e84:
	overlay_veneer Func_080c8150
	.global Object_LinkPair
	.thumb_func
Object_LinkPair:
	overlay_veneer Object_LinkPairFar
	.global Func_02005e94
	.thumb_func
Func_02005e94:
	overlay_veneer Func_080c8168
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02005ea4
	.thumb_func
Func_02005ea4:
	overlay_veneer Func_080c8180
	.global Func_02005eac
	.thumb_func
Func_02005eac:
	overlay_veneer Func_080c81a0
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02005ec4
	.thumb_func
Func_02005ec4:
	overlay_veneer Func_080c8218
	.global Object_AttachWorkTargetToObject
	.thumb_func
Object_AttachWorkTargetToObject:
	overlay_veneer Object_AttachWorkTargetToObjectFar
	.global Func_02005ed4
	.thumb_func
Func_02005ed4:
	overlay_veneer Func_080c8230
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_02005ee4
	.thumb_func
Func_02005ee4:
	overlay_veneer Func_080c8240
	.global Func_02005eec
	.thumb_func
Func_02005eec:
	overlay_veneer Func_080c8248
	.global Func_02005ef4
	.thumb_func
Func_02005ef4:
	overlay_veneer Func_080c8258
	.global Func_02005efc
	.thumb_func
Func_02005efc:
	overlay_veneer Func_080c8278
	.global Func_02005f04
	.thumb_func
Func_02005f04:
	overlay_veneer Func_080c8378
	.global Func_02005f0c
	.thumb_func
Func_02005f0c:
	overlay_veneer Func_080c8380
	.global Func_02005f14
	.thumb_func
Func_02005f14:
	overlay_veneer Func_080c8390
	.global Event_SetStatus1c6
	.thumb_func
Event_SetStatus1c6:
	overlay_veneer Event_SetStatus1c6Far
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global Func_02005f2c
	.thumb_func
Func_02005f2c:
	overlay_veneer Func_080c84e0
	.global Func_02005f34
	.thumb_func
Func_02005f34:
	overlay_veneer Func_080c8540
	.global Func_02005f3c
	.thumb_func
Func_02005f3c:
	overlay_veneer Func_080c8570
	.global Object_SetActionCallback
	.thumb_func
Object_SetActionCallback:
	overlay_veneer Object_SetActionCallbackFar
	.global Func_02005f4c
	.thumb_func
Func_02005f4c:
	overlay_veneer Func_080c8688
	.global Func_02005f54
	.thumb_func
Func_02005f54:
	overlay_veneer Func_080c86d0
	.global Func_02005f5c
	.thumb_func
Func_02005f5c:
	overlay_veneer Func_080c86d8
	.global Func_02005f64
	.thumb_func
Func_02005f64:
	overlay_veneer Func_080c8710
	.global Func_02005f6c
	.thumb_func
Func_02005f6c:
	overlay_veneer Func_080c8718
	.global Func_02005f74
	.thumb_func
Func_02005f74:
	overlay_veneer Func_080c8720
	.global Func_02005f7c
	.thumb_func
Func_02005f7c:
	overlay_veneer Func_080c8728
	.global Func_02005f84
	.thumb_func
Func_02005f84:
	overlay_veneer Func_080c8778
	.global Func_02005f8c
	.thumb_func
Func_02005f8c:
	overlay_veneer Func_080c88c8
	.global Func_02005f94
	.thumb_func
Func_02005f94:
	overlay_veneer Audio_PlayCue
