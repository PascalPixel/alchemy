.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global __divsi3
	.thumb_func
__divsi3:
	overlay_veneer IwramSignedDivide
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
	.global Heap_AllocEwram
	.thumb_func
Heap_AllocEwram:
	overlay_veneer Heap_AllocEwramFar
	.global Heap_Release
	.thumb_func
Heap_Release:
	overlay_veneer Sys_FreeFar
	.global Func_02001454
	.thumb_func
Func_02001454:
	overlay_veneer Window_LoadGraphicsFar
	.global Func_0200145c
	.thumb_func
Func_0200145c:
	overlay_veneer Object_SetMode
	.global Func_02001464
	.thumb_func
Func_02001464:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_0200146c
	.thumb_func
Func_0200146c:
	overlay_veneer Func_080200c0
	.global ObjectDispatch_SetSingleChildFlags
	.thumb_func
ObjectDispatch_SetSingleChildFlags:
	overlay_veneer ObjectDispatch_SetSingleChildFlagsFar
	.global Func_0200147c
	.thumb_func
Func_0200147c:
	overlay_veneer Map_SetScreenShakeFar
	.global Func_02001484
	.thumb_func
Func_02001484:
	overlay_veneer Func_08038348
	.global FieldEvent_WaitFrames
	.thumb_func
FieldEvent_WaitFrames:
	overlay_veneer FieldEvent_WaitFramesFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global Func_020014b4
	.thumb_func
Func_020014b4:
	overlay_veneer ObjectMotion_SetHorizontalPositionWithTerrainFar
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Object_SetActionById
	.thumb_func
Object_SetActionById:
	overlay_veneer Object_SetActionByIdFar
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Func_020014d4
	.thumb_func
Func_020014d4:
	overlay_veneer ObjectGroup_ConfigureChildValueFar
	.global Object_SetPartAttribute
	.thumb_func
Object_SetPartAttribute:
	overlay_veneer Object_SetPartAttributeFar
	.global Func_020014e4
	.thumb_func
Func_020014e4:
	overlay_veneer Event_SetMessageIdFar
	.global Func_020014ec
	.thumb_func
Func_020014ec:
	overlay_veneer Func_080c8198
	.global Func_020014f4
	.thumb_func
Func_020014f4:
	overlay_veneer Event_ShowMessageAndWaitFar
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global ObjectMotion_SetActionVariant
	.thumb_func
ObjectMotion_SetActionVariant:
	overlay_veneer ObjectMotion_SetActionVariantFar
	.global Func_0200150c
	.thumb_func
Func_0200150c:
	overlay_veneer Func_080c8210
	.global Func_02001514
	.thumb_func
Func_02001514:
	overlay_veneer Func_080c8218
	.global Func_0200151c
	.thumb_func
Func_0200151c:
	overlay_veneer Camera_SetSpeedParametersFar
	.global Camera_MoveWithinMapBounds
	.thumb_func
Camera_MoveWithinMapBounds:
	overlay_veneer Camera_MoveWithinMapBoundsFar
	.global Func_0200152c
	.thumb_func
Func_0200152c:
	overlay_veneer Func_080c8240
	.global Func_02001534
	.thumb_func
Func_02001534:
	overlay_veneer Event_SetDestinationFar
	.global Func_0200153c
	.thumb_func
Func_0200153c:
	overlay_veneer BattleEffect_ApplyColorToTargetBufferFar
	.global Func_02001544
	.thumb_func
Func_02001544:
	overlay_veneer BattleEffect_StartBufferInterpolationFar
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
	.global Func_02001564
	.thumb_func
Func_02001564:
	overlay_veneer Func_080c87f8
	.global Field_StartPaletteTransition
	.thumb_func
Field_StartPaletteTransition:
	.ifdef TLA_EDITION_ES
	overlay_veneer Field_StartPaletteTransitionFar
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Field_StartPaletteTransitionFar
	.else
	overlay_veneer Field_StartPaletteTransitionFar
	.endif
	.endif
	.global Func_02001574
	.thumb_func
Func_02001574:
	overlay_veneer BattleEffect_StartArcEffectFar
	.global Func_0200157c
	.thumb_func
Func_0200157c:
	overlay_veneer BattleEffect_EndArcEffectFar
	.global Func_02001584
	.thumb_func
Func_02001584:
	overlay_veneer BattleEffect_WaitForArcGroupsFar
	.global Func_0200158c
	.thumb_func
Func_0200158c:
	overlay_veneer BattleEffect_SetObjectUpdateModeFar
	.global Func_02001594
	.thumb_func
Func_02001594:
	overlay_veneer Sound_PlayFar
