@ Import veneers after the OVERLAY_653 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_653 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
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
	.global Animation_ApplyChildArgument
	.thumb_func
Animation_ApplyChildArgument:
	overlay_veneer Animation_ApplyChildArgumentFar
	.global Func_02005130
	.thumb_func
Func_02005130:
	overlay_veneer Object_SetMode
	.global Func_02005138
	.thumb_func
Func_02005138:
	overlay_veneer Object_SetCallback
	.global Func_02005140
	.thumb_func
Func_02005140:
	overlay_veneer Func_080200c0
	.global Func_02005148
	.thumb_func
Func_02005148:
	overlay_veneer Object_Destroy
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02005158
	.thumb_func
Func_02005158:
	overlay_veneer Func_08020120
	.global Func_02005160
	.thumb_func
Func_02005160:
	overlay_veneer Object_SetPosition
	.global Func_02005168
	.thumb_func
Func_02005168:
	overlay_veneer Object_CommitPosition
	.global Func_02005170
	.thumb_func
Func_02005170:
	overlay_veneer Func_08020178
	.global Func_02005178
	.thumb_func
Func_02005178:
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
	.global Func_02005180
	.thumb_func
Func_02005180:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02005198
	.thumb_func
Func_02005198:
	overlay_veneer Func_08020228
	.global Func_020051a0
	.thumb_func
Func_020051a0:
	overlay_veneer Func_08020230
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Func_020051b0
	.thumb_func
Func_020051b0:
	overlay_veneer Func_08038248
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_020051c0
	.thumb_func
Func_020051c0:
	overlay_veneer Func_080c8018
	.global Func_020051c8
	.thumb_func
Func_020051c8:
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
	.global Func_02005230
	.thumb_func
Func_02005230:
	overlay_veneer Func_080c80f8
	.global Func_02005238
	.thumb_func
Func_02005238:
	overlay_veneer Func_080c8100
	.global Func_02005240
	.thumb_func
Func_02005240:
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
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global ObjectMotion_SetAngleToward
	.thumb_func
ObjectMotion_SetAngleToward:
	overlay_veneer ObjectMotion_SetAngleTowardFar
	.global Func_02005270
	.thumb_func
Func_02005270:
	overlay_veneer Func_080c8168
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_02005280
	.thumb_func
Func_02005280:
	overlay_veneer Func_080c8180
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_02005290
	.thumb_func
Func_02005290:
	overlay_veneer Func_080c8198
	.global Func_02005298
	.thumb_func
Func_02005298:
	overlay_veneer Func_080c81a0
	.global Func_020052a0
	.thumb_func
Func_020052a0:
	overlay_veneer Func_080c81a8
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_020052b0
	.thumb_func
Func_020052b0:
	overlay_veneer Func_080c81d8
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_020052c0
	.thumb_func
Func_020052c0:
	overlay_veneer Func_080c8208
	.global Func_020052c8
	.thumb_func
Func_020052c8:
	overlay_veneer Func_080c8210
	.global Func_020052d0
	.thumb_func
Func_020052d0:
	overlay_veneer Func_080c8218
	.global Func_020052d8
	.thumb_func
Func_020052d8:
	overlay_veneer Func_080c8230
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_020052e8
	.thumb_func
Func_020052e8:
	overlay_veneer Func_080c8240
	.global Func_020052f0
	.thumb_func
Func_020052f0:
	overlay_veneer Func_080c8258
	.global Func_020052f8
	.thumb_func
Func_020052f8:
	overlay_veneer Func_080c8278
	.global Func_02005300
	.thumb_func
Func_02005300:
	overlay_veneer Func_080c8280
	.global Func_02005308
	.thumb_func
Func_02005308:
	overlay_veneer Func_080c8288
	.global Party_SetFields1eeAnd1f0
	.thumb_func
Party_SetFields1eeAnd1f0:
	overlay_veneer Party_SetFields1eeAnd1f0Far
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
	.global Func_02005330
	.thumb_func
Func_02005330:
	overlay_veneer Func_080c83d8
	.global Func_02005338
	.thumb_func
Func_02005338:
	overlay_veneer Func_080c8480
	.global Field_DispatchTypeHandler
	.thumb_func
Field_DispatchTypeHandler:
	overlay_veneer Field_DispatchTypeHandlerFar
	.global Func_02005348
	.thumb_func
Func_02005348:
	overlay_veneer Func_080c84a0
	.global Func_02005350
	.thumb_func
Func_02005350:
	.ifdef TLA_EDITION_ES
	overlay_veneer Field_DispatchTypeHandlerFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Field_DispatchTypeHandlerFar
	.else
	overlay_veneer Func_080c84a8
	.endif
	.endif
	.global Func_02005358
	.thumb_func
Func_02005358:
	overlay_veneer Func_080c84b0
	.global Func_02005360
	.thumb_func
Func_02005360:
	overlay_veneer Func_080c84b8
	.global Func_02005368
	.thumb_func
Func_02005368:
	overlay_veneer Func_080c84c0
	.global Func_02005370
	.thumb_func
Func_02005370:
	overlay_veneer Func_080c84e0
	.global Func_02005378
	.thumb_func
Func_02005378:
	overlay_veneer Func_080c85e8
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_02005388
	.thumb_func
Func_02005388:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8678
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8678
	.else
	overlay_veneer Func_080c8688
	.endif
	.endif
	.global Func_02005390
	.thumb_func
Func_02005390:
	overlay_veneer Func_080c86a8
	.global Func_02005398
	.thumb_func
Func_02005398:
	overlay_veneer Func_080c86d0
	.global Func_020053a0
	.thumb_func
Func_020053a0:
	overlay_veneer Func_080c86d8
	.global Func_020053a8
	.thumb_func
Func_020053a8:
	overlay_veneer Func_080c86e8
	.global Func_020053b0
	.thumb_func
Func_020053b0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8700
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8700
	.else
	overlay_veneer Func_080c8710
	.endif
	.endif
	.global Func_020053b8
	.thumb_func
Func_020053b8:
	overlay_veneer Func_080c8718
	.global Func_020053c0
	.thumb_func
Func_020053c0:
	overlay_veneer Func_080c8720
	.global Func_020053c8
	.thumb_func
Func_020053c8:
	overlay_veneer Func_080c8728
	.global Func_020053d0
	.thumb_func
Func_020053d0:
	overlay_veneer Func_080c8760
	.global Func_020053d8
	.thumb_func
Func_020053d8:
	overlay_veneer Audio_PlayCue
