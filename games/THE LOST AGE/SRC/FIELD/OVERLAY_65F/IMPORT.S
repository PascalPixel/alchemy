@ Import veneers after the OVERLAY_65F overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_65F overlay import veneers
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
	.global Func_0200512c
	.thumb_func
Func_0200512c:
	overlay_veneer Func_080000d0
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
	.global Func_0200514c
	.thumb_func
Func_0200514c:
	overlay_veneer Func_080003c8
	.global Func_02005154
	.thumb_func
Func_02005154:
	overlay_veneer Func_080003d0
	.global Func_0200515c
	.thumb_func
Func_0200515c:
	overlay_veneer Func_080003d8
	.global Func_02005164
	.thumb_func
Func_02005164:
	overlay_veneer Object_SetMode
	.global Func_0200516c
	.thumb_func
Func_0200516c:
	overlay_veneer Object_SetCallback
	.global Func_02005174
	.thumb_func
Func_02005174:
	overlay_veneer Func_080200c0
	.global Func_0200517c
	.thumb_func
Func_0200517c:
	overlay_veneer Func_08020170
	.global Func_02005184
	.thumb_func
Func_02005184:
	overlay_veneer Func_08020178
	.global Func_0200518c
	.thumb_func
Func_0200518c:
	overlay_veneer Func_080201e8
	.global Func_02005194
	.thumb_func
Func_02005194:
	overlay_veneer Func_080201f0
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_020051ac
	.thumb_func
Func_020051ac:
	overlay_veneer Func_080c8018
	.global Func_020051b4
	.thumb_func
Func_020051b4:
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
	.global ObjectMotion_EnableActionAndResetMotion
	.thumb_func
ObjectMotion_EnableActionAndResetMotion:
	overlay_veneer ObjectMotion_EnableActionAndResetMotionFar
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global ObjectMotion_SnapHeadingAndOffset
	.thumb_func
ObjectMotion_SnapHeadingAndOffset:
	overlay_veneer ObjectMotion_SnapHeadingAndOffsetFar
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_0200520c
	.thumb_func
Func_0200520c:
	overlay_veneer Func_080c80f8
	.global Func_02005214
	.thumb_func
Func_02005214:
	overlay_veneer Func_080c8100
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global ObjectMotion_Launch
	.thumb_func
ObjectMotion_Launch:
	overlay_veneer ObjectMotion_LaunchFar
	.global ObjectMotion_SetVariantCallback
	.thumb_func
ObjectMotion_SetVariantCallback:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Func_0200523c
	.thumb_func
Func_0200523c:
	overlay_veneer Func_080c8158
	.global Func_02005244
	.thumb_func
Func_02005244:
	overlay_veneer Func_080c8170
	.global Func_0200524c
	.thumb_func
Func_0200524c:
	overlay_veneer Func_080c8180
	.global Func_02005254
	.thumb_func
Func_02005254:
	overlay_veneer Func_080c8188
	.global Func_0200525c
	.thumb_func
Func_0200525c:
	overlay_veneer Func_080c8198
	.global Func_02005264
	.thumb_func
Func_02005264:
	overlay_veneer Func_080c81a0
	.global Func_0200526c
	.thumb_func
Func_0200526c:
	overlay_veneer Func_080c81d0
	.global Func_02005274
	.thumb_func
Func_02005274:
	overlay_veneer Func_080c81d8
	.global Func_0200527c
	.thumb_func
Func_0200527c:
	overlay_veneer Func_080c81f0
	.global Func_02005284
	.thumb_func
Func_02005284:
	overlay_veneer Func_080c8200
	.global Func_0200528c
	.thumb_func
Func_0200528c:
	overlay_veneer Func_080c8210
	.global Func_02005294
	.thumb_func
Func_02005294:
	overlay_veneer Func_080c8218
	.global Func_0200529c
	.thumb_func
Func_0200529c:
	overlay_veneer Func_080c8228
	.global Func_020052a4
	.thumb_func
Func_020052a4:
	overlay_veneer Func_080c8230
	.global Func_020052ac
	.thumb_func
Func_020052ac:
	overlay_veneer Func_080c8238
	.global Func_020052b4
	.thumb_func
Func_020052b4:
	overlay_veneer Func_080c8240
	.global Func_020052bc
	.thumb_func
Func_020052bc:
	overlay_veneer Func_080c8248
	.global Func_020052c4
	.thumb_func
Func_020052c4:
	overlay_veneer Func_080c8278
	.global Func_020052cc
	.thumb_func
Func_020052cc:
	overlay_veneer Func_080c8378
	.global Func_020052d4
	.thumb_func
Func_020052d4:
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
	.global Func_020052f4
	.thumb_func
Func_020052f4:
	overlay_veneer Func_080c8480
	.global Func_020052fc
	.thumb_func
Func_020052fc:
	overlay_veneer Func_080c8498
	.global Func_02005304
	.thumb_func
Func_02005304:
	overlay_veneer Func_080c84a0
	.global Func_0200530c
	.thumb_func
Func_0200530c:
	overlay_veneer Func_080c84a8
	.global Func_02005314
	.thumb_func
Func_02005314:
	overlay_veneer Func_080c84b0
	.global Func_0200531c
	.thumb_func
Func_0200531c:
	overlay_veneer Func_080c84b8
	.global Func_02005324
	.thumb_func
Func_02005324:
	overlay_veneer Func_080c84e0
	.global Func_0200532c
	.thumb_func
Func_0200532c:
	overlay_veneer Func_080c8580
	.global Func_02005334
	.thumb_func
Func_02005334:
	overlay_veneer Func_080c85e8
	.global ObjectMotion_OffsetPositionAndReset
	.thumb_func
ObjectMotion_OffsetPositionAndReset:
	overlay_veneer ObjectMotion_OffsetPositionAndResetFar
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_02005354
	.thumb_func
Func_02005354:
	overlay_veneer Func_080c86c0
	.global Func_0200535c
	.thumb_func
Func_0200535c:
	overlay_veneer Func_080c86d0
	.global Func_02005364
	.thumb_func
Func_02005364:
	overlay_veneer Func_080c86d8
	.global Func_0200536c
	.thumb_func
Func_0200536c:
	overlay_veneer Func_080c8718
	.global Func_02005374
	.thumb_func
Func_02005374:
	overlay_veneer Func_080c8720
	.global Func_0200537c
	.thumb_func
Func_0200537c:
	overlay_veneer Func_080c8728
	.global Func_02005384
	.thumb_func
Func_02005384:
	overlay_veneer Func_080c8778
	.global Func_0200538c
	.thumb_func
Func_0200538c:
	overlay_veneer Func_080c8968
	.global Func_02005394
	.thumb_func
Func_02005394:
	overlay_veneer Audio_PlayCue
