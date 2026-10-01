@ Import veneers after the OVERLAY_670 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_670 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
	.global __udivsi3
	.thumb_func
__udivsi3:
	overlay_veneer IwramUnsignedDivide
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
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
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
	.global Func_02005a24
	.thumb_func
Func_02005a24:
	overlay_veneer Object_SetMode
	.global Func_02005a2c
	.thumb_func
Func_02005a2c:
	overlay_veneer Object_SetCallback
	.global Func_02005a34
	.thumb_func
Func_02005a34:
	overlay_veneer Func_080200c0
	.global Func_02005a3c
	.thumb_func
Func_02005a3c:
	overlay_veneer Object_Destroy
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02005a4c
	.thumb_func
Func_02005a4c:
	overlay_veneer Func_08020120
	.global Func_02005a54
	.thumb_func
Func_02005a54:
	overlay_veneer Object_SetPosition
	.global Func_02005a5c
	.thumb_func
Func_02005a5c:
	overlay_veneer Object_CommitPosition
	.global Func_02005a64
	.thumb_func
Func_02005a64:
	overlay_veneer Func_08020178
	.global Func_02005a6c
	.thumb_func
Func_02005a6c:
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
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02005a84
	.thumb_func
Func_02005a84:
	overlay_veneer Func_08038248
	.global Func_02005a8c
	.thumb_func
Func_02005a8c:
	overlay_veneer Func_080ad228
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02005a9c
	.thumb_func
Func_02005a9c:
	overlay_veneer Func_080c8018
	.global Func_02005aa4
	.thumb_func
Func_02005aa4:
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
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
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
	.global Func_02005b0c
	.thumb_func
Func_02005b0c:
	overlay_veneer Func_080c80f8
	.global Func_02005b14
	.thumb_func
Func_02005b14:
	overlay_veneer Func_080c8100
	.global Func_02005b1c
	.thumb_func
Func_02005b1c:
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
	.global Func_02005b4c
	.thumb_func
Func_02005b4c:
	overlay_veneer Func_080c8180
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_02005b5c
	.thumb_func
Func_02005b5c:
	overlay_veneer Func_080c8198
	.global Func_02005b64
	.thumb_func
Func_02005b64:
	overlay_veneer Func_080c81a0
	.global Func_02005b6c
	.thumb_func
Func_02005b6c:
	overlay_veneer Func_080c81a8
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_02005b7c
	.thumb_func
Func_02005b7c:
	overlay_veneer Func_080c81d8
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_02005b8c
	.thumb_func
Func_02005b8c:
	overlay_veneer Func_080c8208
	.global Func_02005b94
	.thumb_func
Func_02005b94:
	overlay_veneer Func_080c8210
	.global Func_02005b9c
	.thumb_func
Func_02005b9c:
	overlay_veneer Func_080c8218
	.global Func_02005ba4
	.thumb_func
Func_02005ba4:
	overlay_veneer Func_080c8230
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_02005bb4
	.thumb_func
Func_02005bb4:
	overlay_veneer Func_080c8240
	.global Func_02005bbc
	.thumb_func
Func_02005bbc:
	overlay_veneer Func_080c8258
	.global Func_02005bc4
	.thumb_func
Func_02005bc4:
	overlay_veneer Func_080c8278
	.global Func_02005bcc
	.thumb_func
Func_02005bcc:
	overlay_veneer Func_080c82e0
	.global Func_02005bd4
	.thumb_func
Func_02005bd4:
	overlay_veneer Func_080c8378
	.global Func_02005bdc
	.thumb_func
Func_02005bdc:
	overlay_veneer Func_080c8380
	.global Func_02005be4
	.thumb_func
Func_02005be4:
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
	.global Func_02005c04
	.thumb_func
Func_02005c04:
	overlay_veneer Func_080c8480
	.global Field_DispatchTypeHandler
	.thumb_func
Field_DispatchTypeHandler:
	overlay_veneer Field_DispatchTypeHandlerFar
	.global Func_02005c14
	.thumb_func
Func_02005c14:
	overlay_veneer Func_080c84a0
	.global Func_02005c1c
	.thumb_func
Func_02005c1c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Field_DispatchTypeHandlerFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Field_DispatchTypeHandlerFar
	.else
	overlay_veneer Func_080c84a8
	.endif
	.endif
	.global Func_02005c24
	.thumb_func
Func_02005c24:
	overlay_veneer Func_080c84b0
	.global Func_02005c2c
	.thumb_func
Func_02005c2c:
	overlay_veneer Func_080c84b8
	.global Func_02005c34
	.thumb_func
Func_02005c34:
	overlay_veneer Func_080c84c0
	.global Func_02005c3c
	.thumb_func
Func_02005c3c:
	overlay_veneer Func_080c84e0
	.global Func_02005c44
	.thumb_func
Func_02005c44:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8508
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8508
	.else
	overlay_veneer Func_080c8518
	.endif
	.endif
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_02005c54
	.thumb_func
Func_02005c54:
	overlay_veneer Func_080c86a8
	.global Func_02005c5c
	.thumb_func
Func_02005c5c:
	overlay_veneer Func_080c86e8
	.global Func_02005c64
	.thumb_func
Func_02005c64:
	overlay_veneer Func_080c86f8
	.global Func_02005c6c
	.thumb_func
Func_02005c6c:
	overlay_veneer Func_080c8708
	.global Func_02005c74
	.thumb_func
Func_02005c74:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c87a0
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c87a0
	.else
	overlay_veneer Func_080c87b0
	.endif
	.endif
	.global Func_02005c7c
	.thumb_func
Func_02005c7c:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8920
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8920
	.else
	overlay_veneer Func_080c8930
	.endif
	.endif
	.global Func_02005c84
	.thumb_func
Func_02005c84:
	overlay_veneer Func_08108008
	.global Func_02005c8c
	.thumb_func
Func_02005c8c:
	overlay_veneer Func_08108010
	.global Func_02005c94
	.thumb_func
Func_02005c94:
	overlay_veneer Func_08108018
	.global Func_02005c9c
	.thumb_func
Func_02005c9c:
	overlay_veneer Audio_PlayCue
