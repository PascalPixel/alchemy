@ Import veneers after the OVERLAY_681 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_681 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_MathDivide
	.thumb_func
Engine_MathDivide:
	overlay_veneer IwramSignedDivide
	.global Engine_MathRemainder
	.thumb_func
Engine_MathRemainder:
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
	.global Runtime_BumpAllocateAlternatePool
	.thumb_func
Runtime_BumpAllocateAlternatePool:
	overlay_veneer Runtime_BumpAllocateAlternatePoolFar
	.global Sys_Free
	.thumb_func
Sys_Free:
	overlay_veneer Sys_FreeFar
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
	.global GameFlag_GetByte
	.thumb_func
GameFlag_GetByte:
	overlay_veneer GameFlag_GetByteFar
	.global GameFlag_SetByte
	.thumb_func
GameFlag_SetByte:
	overlay_veneer GameFlag_SetByteFar
	.global ResourceMetadata_Register
	.thumb_func
ResourceMetadata_Register:
	overlay_veneer ResourceMetadata_RegisterFar
	.global Func_02004d9c
	.thumb_func
Func_02004d9c:
	overlay_veneer Object_SetMode
	.global ObjectDispatch_ApplyValueToChildren
	.thumb_func
ObjectDispatch_ApplyValueToChildren:
	overlay_veneer ObjectDispatch_ApplyValueToChildrenFar
	.global Func_02004dac
	.thumb_func
Func_02004dac:
	overlay_veneer Object_SetCallback
	.global Func_02004db4
	.thumb_func
Func_02004db4:
	overlay_veneer Func_080200c0
	.global Func_02004dbc
	.thumb_func
Func_02004dbc:
	overlay_veneer Object_Destroy
	.global Func_02004dc4
	.thumb_func
Func_02004dc4:
	overlay_veneer Func_08020120
	.global Func_02004dcc
	.thumb_func
Func_02004dcc:
	overlay_veneer Object_SetPosition
	.global Func_02004dd4
	.thumb_func
Func_02004dd4:
	overlay_veneer Func_080201a8
	.global Func_02004ddc
	.thumb_func
Func_02004ddc:
	overlay_veneer Func_080201b0
	.global Func_02004de4
	.thumb_func
Func_02004de4:
	overlay_veneer Func_080201b8
	.global Func_02004dec
	.thumb_func
Func_02004dec:
	overlay_veneer Func_080201c8
	.global Func_02004df4
	.thumb_func
Func_02004df4:
	overlay_veneer Func_080201e0
	.global Func_02004dfc
	.thumb_func
Func_02004dfc:
	overlay_veneer Func_080201e8
	.global Func_02004e04
	.thumb_func
Func_02004e04:
	overlay_veneer Func_080201f0
	.global Func_02004e0c
	.thumb_func
Func_02004e0c:
	overlay_veneer Func_080201f8
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02004e24
	.thumb_func
Func_02004e24:
	overlay_veneer Func_08020228
	.global Func_02004e2c
	.thumb_func
Func_02004e2c:
	overlay_veneer Func_08020230
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_02004e3c
	.thumb_func
Func_02004e3c:
	overlay_veneer Func_08020290
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global PartyInventory_FindOwner
	.thumb_func
PartyInventory_FindOwner:
	overlay_veneer PartyInventory_FindOwnerFar
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02004e5c
	.thumb_func
Func_02004e5c:
	overlay_veneer Func_080c8018
	.global Func_02004e64
	.thumb_func
Func_02004e64:
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
	.global Object_RefreshSelectorById
	.thumb_func
Object_RefreshSelectorById:
	overlay_veneer Object_RefreshSelectorByIdFar
	.global ObjectMotion_EnableActionAndResetMotion
	.thumb_func
ObjectMotion_EnableActionAndResetMotion:
	overlay_veneer ObjectMotion_EnableActionAndResetMotionFar
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02004ebc
	.thumb_func
Func_02004ebc:
	overlay_veneer Func_080c80f8
	.global Func_02004ec4
	.thumb_func
Func_02004ec4:
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
	.global ObjectMotion_SetVariantCallback
	.thumb_func
ObjectMotion_SetVariantCallback:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global Func_02004eec
	.thumb_func
Func_02004eec:
	overlay_veneer Func_080c8150
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Func_02004efc
	.thumb_func
Func_02004efc:
	overlay_veneer Func_080c8168
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02004f0c
	.thumb_func
Func_02004f0c:
	overlay_veneer Func_080c8180
	.global Func_02004f14
	.thumb_func
Func_02004f14:
	overlay_veneer Func_080c81a0
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02004f2c
	.thumb_func
Func_02004f2c:
	overlay_veneer Func_080c8210
	.global Func_02004f34
	.thumb_func
Func_02004f34:
	overlay_veneer Func_080c8218
	.global Object_AttachWorkTargetToObject
	.thumb_func
Object_AttachWorkTargetToObject:
	overlay_veneer Object_AttachWorkTargetToObjectFar
	.global Func_02004f44
	.thumb_func
Func_02004f44:
	overlay_veneer Func_080c8230
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_02004f54
	.thumb_func
Func_02004f54:
	overlay_veneer Func_080c8240
	.global Func_02004f5c
	.thumb_func
Func_02004f5c:
	overlay_veneer Func_080c8248
	.global Func_02004f64
	.thumb_func
Func_02004f64:
	overlay_veneer Func_080c8268
	.global Func_02004f6c
	.thumb_func
Func_02004f6c:
	overlay_veneer Func_080c8278
	.global Func_02004f74
	.thumb_func
Func_02004f74:
	overlay_veneer Func_080c8280
	.global Party_SetFields1eeAnd1f0
	.thumb_func
Party_SetFields1eeAnd1f0:
	overlay_veneer Party_SetFields1eeAnd1f0Far
	.global Party_SetFields1f2And1f4
	.thumb_func
Party_SetFields1f2And1f4:
	overlay_veneer Party_SetFields1f2And1f4Far
	.global Func_02004f8c
	.thumb_func
Func_02004f8c:
	overlay_veneer Func_080c8368
	.global Func_02004f94
	.thumb_func
Func_02004f94:
	overlay_veneer Func_080c8378
	.global Func_02004f9c
	.thumb_func
Func_02004f9c:
	overlay_veneer Func_080c8380
	.global Func_02004fa4
	.thumb_func
Func_02004fa4:
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
	.global Func_02004fc4
	.thumb_func
Func_02004fc4:
	overlay_veneer Func_080c8480
	.global Func_02004fcc
	.thumb_func
Func_02004fcc:
	overlay_veneer Func_080c84e0
	.global Func_02004fd4
	.thumb_func
Func_02004fd4:
	overlay_veneer Func_080c8570
	.global Func_02004fdc
	.thumb_func
Func_02004fdc:
	overlay_veneer Func_080c8628
	.global Func_02004fe4
	.thumb_func
Func_02004fe4:
	overlay_veneer Func_080c8778
	.global Func_02004fec
	.thumb_func
Func_02004fec:
	overlay_veneer Func_080c8780
	.global Func_02004ff4
	.thumb_func
Func_02004ff4:
	overlay_veneer Func_080c88d0
	.global ObjectMotion_ResetTargetsAndVelocity
	.thumb_func
ObjectMotion_ResetTargetsAndVelocity:
	overlay_veneer ObjectMotion_ResetTargetsAndVelocityFar
	.global Func_02005004
	.thumb_func
Func_02005004:
	overlay_veneer Func_080c8940
	.global Func_0200500c
	.thumb_func
Func_0200500c:
	overlay_veneer Func_080c8948
	.global Func_02005014
	.thumb_func
Func_02005014:
	overlay_veneer Audio_PlayCue
