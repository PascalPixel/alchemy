@ Far-call stub table at 0x080c8088: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Object_GetByIdFar
	.thumb_func
Object_GetByIdFar:
	overlay_veneer Object_GetById
	.global Func_080c8090
	.thumb_func
Func_080c8090:
	overlay_veneer Func_080d36a8
	.global ObjectMotion_SetSpeedParametersFar
	.thumb_func
ObjectMotion_SetSpeedParametersFar:
	overlay_veneer ObjectMotion_SetSpeedParameters
	.global ObjectMotion_EnableActionAndSetCallbackFar
	.thumb_func
ObjectMotion_EnableActionAndSetCallbackFar:
	overlay_veneer ObjectMotion_EnableActionAndSetCallback
	.global Object_RefreshSelectorByIdFar
	.thumb_func
Object_RefreshSelectorByIdFar:
	overlay_veneer Object_RefreshSelectorById
	.global ObjectMotion_EnableActionAndResetMotionFar
	.thumb_func
ObjectMotion_EnableActionAndResetMotionFar:
	overlay_veneer ObjectMotion_EnableActionAndResetMotion
	.global Object_SetActionCallbackAndRefreshByIdFar
	.thumb_func
Object_SetActionCallbackAndRefreshByIdFar:
	overlay_veneer Object_SetActionCallbackAndRefreshById
	.global ObjectMotion_ResetAndSetPositionFar
	.thumb_func
ObjectMotion_ResetAndSetPositionFar:
	overlay_veneer ObjectMotion_ResetAndSetPosition
	.global ObjectMotion_SetPositionAndCommitFar
	.thumb_func
ObjectMotion_SetPositionAndCommitFar:
	overlay_veneer ObjectMotion_SetPositionAndCommit
	.global ObjectMotion_ResetAndSetPositionInMode2Far
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2Far:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2
	.global ObjectMotion_SetPositionAndResetFar
	.thumb_func
ObjectMotion_SetPositionAndResetFar:
	overlay_veneer ObjectMotion_SetPositionAndReset
	.global ObjectMotion_SnapHeadingAndOffsetFar
	.thumb_func
ObjectMotion_SnapHeadingAndOffsetFar:
	overlay_veneer ObjectMotion_SnapHeadingAndOffset
	.global ObjectMotion_OffsetPositionAndResetMotionFar
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotionFar:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotion
	.global ObjectMotion_CommitCurrentPositionAndActivateFar
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivateFar:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivate
	.global Func_080c80f8
	.thumb_func
Func_080c80f8:
	overlay_veneer Func_080d3118
	.global Func_080c8100
	.thumb_func
Func_080c8100:
	overlay_veneer Func_080d3164
	.global Func_080c8108
	.thumb_func
Func_080c8108:
	overlay_veneer Func_080d31c0
	.global Func_080c8110
	.thumb_func
Func_080c8110:
	overlay_veneer Func_080d3214
	.global Object_SetModeByIdFar
	.thumb_func
Object_SetModeByIdFar:
	overlay_veneer Object_SetModeById
	.global Object_SetActionByIdFar
	.thumb_func
Object_SetActionByIdFar:
	overlay_veneer Object_SetActionById
	.global Motion_SetModeAndWaitAnimationFar
	.thumb_func
Motion_SetModeAndWaitAnimationFar:
	overlay_veneer Motion_SetModeAndWaitAnimation
	.global ObjectMotion_WaitForAnimationChangeFar
	.thumb_func
ObjectMotion_WaitForAnimationChangeFar:
	overlay_veneer ObjectMotion_WaitForAnimationChange
	.global ObjectMotion_LaunchFar
	.thumb_func
ObjectMotion_LaunchFar:
	overlay_veneer ObjectMotion_Launch
	.global ObjectMotion_SetVariantCallbackFar
	.thumb_func
ObjectMotion_SetVariantCallbackFar:
	overlay_veneer ObjectMotion_SetVariantCallback
	.global Motion_SetVarCbAndRefreshFar
	.thumb_func
Motion_SetVarCbAndRefreshFar:
	overlay_veneer Motion_SetVarCbAndRefresh
	.global Func_080c8150
	.thumb_func
Func_080c8150:
	overlay_veneer Func_080d3460
	.global ObjectMotion_SetAngleTowardFar
	.thumb_func
ObjectMotion_SetAngleTowardFar:
	overlay_veneer ObjectMotion_SetAngleToward
	.global Object_LinkPairFar
	.thumb_func
Object_LinkPairFar:
	overlay_veneer Object_LinkPair
	.global Func_080c8168
	.thumb_func
Func_080c8168:
	overlay_veneer ObjectGroup_ConfigureChildValue
	.global Object_SetPartAttributeFar
	.thumb_func
Object_SetPartAttributeFar:
	overlay_veneer Object_SetPartAttribute
	.global Func_080c8178
	.thumb_func
Func_080c8178:
	overlay_veneer Func_080d3780
	.global Func_080c8180
	.thumb_func
Func_080c8180:
	overlay_veneer EventRuntime_SetMessage
	.global UiText_OpenMessageAtObjectFar
	.thumb_func
UiText_OpenMessageAtObjectFar:
	overlay_veneer UiText_OpenMessageAtObject
	.global Func_080c8190
	.thumb_func
Func_080c8190:
	overlay_veneer Func_080d3fb0
	.global Func_080c8198
	.thumb_func
Func_080c8198:
	overlay_veneer Func_080d406c
	.global Func_080c81a0
	.thumb_func
Func_080c81a0:
	overlay_veneer EventRuntime_ShowMessageAndWait
	.global Func_080c81a8
	.thumb_func
Func_080c81a8:
	overlay_veneer Func_080d4084
	.global Func_080c81b0
	.thumb_func
Func_080c81b0:
	overlay_veneer Func_080d40d8
	.global Func_080c81b8
	.thumb_func
Func_080c81b8:
	overlay_veneer Func_080d40dc
	.global Func_080c81c0
	.thumb_func
Func_080c81c0:
	overlay_veneer Func_080d4178
	.global Event_ShowCounterAtPositionFar
	.thumb_func
Event_ShowCounterAtPositionFar:
	overlay_veneer Event_ShowCounterAtPosition
	.global ObjectMotion_ArmCallbackFar
	.thumb_func
ObjectMotion_ArmCallbackFar:
	overlay_veneer ObjectMotion_ArmCallback
	.global Func_080c81d8
	.thumb_func
Func_080c81d8:
	overlay_veneer ObjectMotion_ArmCallbackAndRefresh
	.global Func_080c81e0
	.thumb_func
Func_080c81e0:
	overlay_veneer Object_ResetTargetAndSetMode1
	.global ObjectTable_RunIfActiveFar
	.thumb_func
ObjectTable_RunIfActiveFar:
	overlay_veneer ObjectTable_RunIfActive
	.global Func_080c81f0
	.thumb_func
Func_080c81f0:
	overlay_veneer Func_080d41fc
	.global Func_080c81f8
	.thumb_func
Func_080c81f8:
	overlay_veneer Func_080d4330
	.global ObjectMotion_SetActionVariantFar
	.thumb_func
ObjectMotion_SetActionVariantFar:
	overlay_veneer ObjectMotion_SetActionVariant
	.global Func_080c8208
	.thumb_func
Func_080c8208:
	overlay_veneer Func_080d390c
	.global Func_080c8210
	.thumb_func
Func_080c8210:
	overlay_veneer Func_080d47b4
	.global Func_080c8218
	.thumb_func
Func_080c8218:
	overlay_veneer Func_080d489c
	.global ObjectVisual_CopyAttributesFar
	.thumb_func
ObjectVisual_CopyAttributesFar:
	overlay_veneer ObjectVisual_CopyAttributes
	.global Object_AttachWorkTargetToObjectFar
	.thumb_func
Object_AttachWorkTargetToObjectFar:
	overlay_veneer Object_AttachWorkTargetToObject
	.global Func_080c8230
	.thumb_func
Func_080c8230:
	overlay_veneer Func_080d43ec
	.global Motion_CamBoundsFar
	.thumb_func
Motion_CamBoundsFar:
	overlay_veneer Motion_CamBounds
	.global Func_080c8240
	.thumb_func
Func_080c8240:
	overlay_veneer Func_080d4548
	.global Func_080c8248
	.thumb_func
Func_080c8248:
	overlay_veneer Func_080d450c
	.global Func_080c8250
	.thumb_func
Func_080c8250:
	overlay_veneer Func_080d452c
	.global Func_080c8258
	.thumb_func
Func_080c8258:
	overlay_veneer Func_080d4568
	.global Func_080c8260
	.thumb_func
Func_080c8260:
	overlay_veneer Func_080d45b8
