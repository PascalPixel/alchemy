@ Import veneers after the OVERLAY_669 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_669 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
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
	.global Func_02003100
	.thumb_func
Func_02003100:
	overlay_veneer Object_SetMode
	.global Func_02003108
	.thumb_func
Func_02003108:
	overlay_veneer ObjectDispatch_InitializeFar
	.global Func_02003110
	.thumb_func
Func_02003110:
	overlay_veneer Func_080200c0
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_02003130
	.thumb_func
Func_02003130:
	overlay_veneer Func_080c8018
	.global Func_02003138
	.thumb_func
Func_02003138:
	overlay_veneer Func_080c8020
	.global Inventory_PromptAndSetObjectMode
	.thumb_func
Inventory_PromptAndSetObjectMode:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	.global Func_02003148
	.thumb_func
Func_02003148:
	overlay_veneer Func_080c8078
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
	.global Func_02003198
	.thumb_func
Func_02003198:
	overlay_veneer Func_080c80f8
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
	.global Motion_SetVarCbAndRefresh
	.thumb_func
Motion_SetVarCbAndRefresh:
	overlay_veneer Motion_SetVarCbAndRefreshFar
	.global Func_020031c8
	.thumb_func
Func_020031c8:
	overlay_veneer Func_080c8180
	.global UiText_OpenMessageAtObject
	.thumb_func
UiText_OpenMessageAtObject:
	overlay_veneer UiText_OpenMessageAtObjectFar
	.global Func_020031d8
	.thumb_func
Func_020031d8:
	overlay_veneer Func_080c8198
	.global Func_020031e0
	.thumb_func
Func_020031e0:
	overlay_veneer Func_080c81a0
	.global ObjectMotion_ArmCallback
	.thumb_func
ObjectMotion_ArmCallback:
	overlay_veneer ObjectMotion_ArmCallbackFar
	.global Func_020031f0
	.thumb_func
Func_020031f0:
	overlay_veneer Func_080c81f0
	.global Func_020031f8
	.thumb_func
Func_020031f8:
	overlay_veneer Func_080c8210
	.global Func_02003200
	.thumb_func
Func_02003200:
	overlay_veneer Func_080c8218
	.global Object_AttachWorkTargetToObject
	.thumb_func
Object_AttachWorkTargetToObject:
	overlay_veneer Object_AttachWorkTargetToObjectFar
	.global Motion_CamBounds
	.thumb_func
Motion_CamBounds:
	overlay_veneer Motion_CamBoundsFar
	.global Func_02003218
	.thumb_func
Func_02003218:
	overlay_veneer Func_080c8240
	.global Func_02003220
	.thumb_func
Func_02003220:
	overlay_veneer Func_080c8280
	.global Party_SetFields1eeAnd1f0
	.thumb_func
Party_SetFields1eeAnd1f0:
	overlay_veneer Party_SetFields1eeAnd1f0Far
	.global Event_SetStatus1c6
	.thumb_func
Event_SetStatus1c6:
	overlay_veneer Event_SetStatus1c6Far
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
	.global Func_02003240
	.thumb_func
Func_02003240:
	overlay_veneer Func_080c84e0
	.global Func_02003248
	.thumb_func
Func_02003248:
	overlay_veneer Func_080c85e8
	.global ObjectMotion_OffsetPositionAndReset
	.thumb_func
ObjectMotion_OffsetPositionAndReset:
	overlay_veneer ObjectMotion_OffsetPositionAndResetFar
	.global ObjectMotion_CommitPositionAndActivate
	.thumb_func
ObjectMotion_CommitPositionAndActivate:
	overlay_veneer ObjectMotion_CommitPositionAndActivateFar
	.global Func_02003260
	.thumb_func
Func_02003260:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8798
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8798
	.else
	overlay_veneer Func_080c87a8
	.endif
	.endif
	.global Func_02003268
	.thumb_func
Func_02003268:
	overlay_veneer Func_080c87f8
	.ifdef TLA_EDITION_ES
	.global Field_EndActorSpriteEffect
	.thumb_func
Field_EndActorSpriteEffect:
	overlay_veneer Field_EndActorSpriteEffectFar
	.else
	.ifdef TLA_EDITION_IT
	.global Field_EndActorSpriteEffect
	.thumb_func
Field_EndActorSpriteEffect:
	overlay_veneer Field_EndActorSpriteEffectFar
	.else
	.global Field_BeginPaletteTransition
	.thumb_func
Field_BeginPaletteTransition:
	overlay_veneer Field_BeginPaletteTransitionFar
	.endif
	.endif
	.global Func_02003278
	.thumb_func
Func_02003278:
	overlay_veneer Func_080c8808
	.global Func_02003280
	.thumb_func
Func_02003280:
	overlay_veneer Func_080c8810
	.global Func_02003288
	.thumb_func
Func_02003288:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8808
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8808
	.else
	overlay_veneer Func_080c8818
	.endif
	.endif
	.global Func_02003290
	.thumb_func
Func_02003290:
	overlay_veneer Func_080c8820
	.global Func_02003298
	.thumb_func
Func_02003298:
	overlay_veneer Func_080c8860
	.global Func_020032a0
	.thumb_func
Func_020032a0:
	.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8920
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8920
	.else
	overlay_veneer Func_080c8930
	.endif
	.endif
	.global Func_020032a8
	.thumb_func
Func_020032a8:
	overlay_veneer Audio_PlayCue
