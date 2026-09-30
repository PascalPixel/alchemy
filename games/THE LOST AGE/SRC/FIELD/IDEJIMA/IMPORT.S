@ Import veneers after the IDEJIMA overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — IDEJIMA overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Task_Wait
	.thumb_func
Task_Wait:
	overlay_veneer WaitFramesFar
	.global Func_02003140
	.thumb_func
Func_02003140:
	overlay_veneer Func_080000d0
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.global Func_02003150
	.thumb_func
Func_02003150:
	overlay_veneer Func_080003c8
	.global Func_02003158
	.thumb_func
Func_02003158:
	overlay_veneer Func_080003d0
	.global Func_02003160
	.thumb_func
Func_02003160:
	overlay_veneer Func_080003d8
	.global Engine_ObjectSetMode
	.thumb_func
Engine_ObjectSetMode:
	overlay_veneer Object_SetMode
	.global Engine_ObjectSetScript
	.thumb_func
Engine_ObjectSetScript:
	overlay_veneer Object_SetCallback
	.global Engine_ObjectCreate
	.thumb_func
Engine_ObjectCreate:
	overlay_veneer Func_080200c0
	.global Func_02003180
	.thumb_func
Func_02003180:
	overlay_veneer Func_08020120
	.global Engine_MapCopyCellsTo
	.thumb_func
Engine_MapCopyCellsTo:
	overlay_veneer Func_08020178
	.global Func_02003190
	.thumb_func
Func_02003190:
	overlay_veneer Func_080201e8
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global UiText_ShowPositionedMessageAndWait
	.thumb_func
UiText_ShowPositionedMessageAndWait:
	overlay_veneer UiText_ShowPositionedMessageAndWaitFar
	.global Func_020031a8
	.thumb_func
Func_020031a8:
	overlay_veneer Func_080ad208
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_020031b8
	.thumb_func
Func_020031b8:
	overlay_veneer Func_080c8018
	.global Func_020031c0
	.thumb_func
Func_020031c0:
	overlay_veneer Func_080c8020
	.global Event_PrepareObjectAndApplyValue
	.thumb_func
Event_PrepareObjectAndApplyValue:
	overlay_veneer Event_PrepareObjectAndApplyValueFar
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
	.global Object_RefreshSelectorById
	.thumb_func
Object_RefreshSelectorById:
	overlay_veneer Object_RefreshSelectorByIdFar
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
	.global Func_02003228
	.thumb_func
Func_02003228:
	overlay_veneer Func_080c80f8
	.global Func_02003230
	.thumb_func
Func_02003230:
	overlay_veneer Func_080c8100
	.global Func_02003238
	.thumb_func
Func_02003238:
	overlay_veneer Func_080c8110
	.global Engine_ActorSetAnimation
	.thumb_func
Engine_ActorSetAnimation:
	overlay_veneer Object_SetModeByIdFar
	.global Motion_SetModeAndWaitAnimation
	.thumb_func
Motion_SetModeAndWaitAnimation:
	overlay_veneer Motion_SetModeAndWaitAnimationFar
	.global ObjectMotion_WaitForAnimationChange
	.thumb_func
ObjectMotion_WaitForAnimationChange:
	overlay_veneer ObjectMotion_WaitForAnimationChangeFar
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
	.global Func_02003270
	.thumb_func
Func_02003270:
	overlay_veneer Func_080c8158
	.global Object_LinkPair
	.thumb_func
Object_LinkPair:
	overlay_veneer Object_LinkPairFar
	.global Func_02003280
	.thumb_func
Func_02003280:
	overlay_veneer Func_080c8170
	.global Func_02003288
	.thumb_func
Func_02003288:
	overlay_veneer Func_080c8180
	.global Func_02003290
	.thumb_func
Func_02003290:
	overlay_veneer Func_080c8188
	.global Func_02003298
	.thumb_func
Func_02003298:
	overlay_veneer Func_080c8198
	.global Func_020032a0
	.thumb_func
Func_020032a0:
	overlay_veneer Func_080c81a0
	.global Func_020032a8
	.thumb_func
Func_020032a8:
	overlay_veneer Func_080c81a8
	.global Func_020032b0
	.thumb_func
Func_020032b0:
	overlay_veneer Func_080c81d0
	.global Func_020032b8
	.thumb_func
Func_020032b8:
	overlay_veneer Func_080c81f0
	.global Func_020032c0
	.thumb_func
Func_020032c0:
	overlay_veneer Func_080c8210
	.global Func_020032c8
	.thumb_func
Func_020032c8:
	overlay_veneer Func_080c8218
	.global Func_020032d0
	.thumb_func
Func_020032d0:
	overlay_veneer Func_080c8230
	.global Func_020032d8
	.thumb_func
Func_020032d8:
	overlay_veneer Func_080c8238
	.global Func_020032e0
	.thumb_func
Func_020032e0:
	overlay_veneer Func_080c8240
	.global Func_020032e8
	.thumb_func
Func_020032e8:
	overlay_veneer Func_080c8278
	.global Func_020032f0
	.thumb_func
Func_020032f0:
	overlay_veneer Func_080c8378
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
	.global Func_02003310
	.thumb_func
Func_02003310:
	overlay_veneer Func_080c84e0
	.global Object_LinkObjectAndSetCallback
	.thumb_func
Object_LinkObjectAndSetCallback:
	overlay_veneer Object_LinkObjectAndSetCallbackFar
	.global Func_02003320
	.thumb_func
Func_02003320:
	overlay_veneer Func_080c8930
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer Audio_PlayCue
