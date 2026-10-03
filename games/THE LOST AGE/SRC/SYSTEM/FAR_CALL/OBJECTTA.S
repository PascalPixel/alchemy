@ Far-call stub table at 0x080c83a8: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Event_SetStatus1c6Far
	.thumb_func
Event_SetStatus1c6Far:
	overlay_veneer EventRuntime_StartScreen
	.global Event_ClearStatus1c6Far
	.thumb_func
Event_ClearStatus1c6Far:
	overlay_veneer EventRuntime_FinishScreen
	.global Event_WaitValue1c8FramesFar
	.thumb_func
Event_WaitValue1c8FramesFar:
	overlay_veneer EventRuntime_WaitDelay
	.global Func_080c83c0
	.thumb_func
Func_080c83c0:
	overlay_veneer Func_080d6408
	.global Func_080c83c8
	.thumb_func
Func_080c83c8:
	overlay_veneer Func_080d64b8
	.global BattleFx_PlayCueAndStartEmitterOnTargetFar
	.thumb_func
BattleFx_PlayCueAndStartEmitterOnTargetFar:
	overlay_veneer BattleFx_PlayCueAndStartEmitterOnTarget
	.global Func_080c83d8
	.thumb_func
Func_080c83d8:
	overlay_veneer Func_080cf3b4
	.global Func_080c83e0
	.thumb_func
Func_080c83e0:
	overlay_veneer Func_080cf424
	.global Func_080c83e8
	.thumb_func
Func_080c83e8:
	overlay_veneer Func_080ceffc
	.global Func_080c83f0
	.thumb_func
Func_080c83f0:
	overlay_veneer Func_080cefb4
	.global Func_080c83f8
	.thumb_func
Func_080c83f8:
	overlay_veneer Func_080cef68
	.global Func_080c8400
	.thumb_func
Func_080c8400:
	overlay_veneer Func_080cefd0
	.global Func_080c8408
	.thumb_func
Func_080c8408:
	overlay_veneer Func_080cf00c
	.global Func_080c8410
	.thumb_func
Func_080c8410:
	overlay_veneer Func_080cf004
	.global Func_080c8418
	.thumb_func
Func_080c8418:
	overlay_veneer Func_080cf050
	.global Object_SetWideSpriteFar
	.thumb_func
Object_SetWideSpriteFar:
	overlay_veneer Object_SetWideSprite
	.global Func_080c8428
	.thumb_func
Func_080c8428:
	overlay_veneer Func_080ceec8
	.global Func_080c8430
	.thumb_func
Func_080c8430:
	overlay_veneer Func_080cb82c
	.global Func_080c8438
	.thumb_func
Func_080c8438:
	overlay_veneer Func_080cb8a4
	.global Func_080c8440
	.thumb_func
Func_080c8440:
	overlay_veneer Func_080caa4c
	.global Event_SpawnObjectTableFar
	.thumb_func
Event_SpawnObjectTableFar:
	overlay_veneer Event_SpawnObjectTable
	.global Func_080c8450
	.thumb_func
Func_080c8450:
	overlay_veneer Func_080caa2c
	.global ObjectTable_GetFar
	.thumb_func
ObjectTable_GetFar:
	overlay_veneer ObjectTable_Get
	.global Func_080c8460
	.thumb_func
Func_080c8460:
	overlay_veneer Func_080ca9cc
	.global Func_080c8468
	.thumb_func
Func_080c8468:
	overlay_veneer Func_080d1a18
	.global Func_080c8470
	.thumb_func
Func_080c8470:
	overlay_veneer Func_080d1ac4
	.global Func_080c8478
	.thumb_func
Func_080c8478:
	overlay_veneer Func_080d1ad4
	.global Func_080c8480
	.thumb_func
Func_080c8480:
	overlay_veneer Func_080d1cc0
	.global Func_080c8488
	.thumb_func
Func_080c8488:
	overlay_veneer Func_080d1d34
	.global Func_080c8490
	.thumb_func
Func_080c8490:
	overlay_veneer Func_080d1d44
	.global Field_DispatchTypeHandlerFar
	.thumb_func
Field_DispatchTypeHandlerFar:
	overlay_veneer Field_DispatchTypeHandler
	.global Func_080c84a0
	.thumb_func
Func_080c84a0:
	overlay_veneer Func_080dc410
	.global Func_080c84a8
	.thumb_func
Func_080c84a8:
	overlay_veneer Func_080dc62c
	.global Func_080c84b0
	.thumb_func
Func_080c84b0:
	overlay_veneer Func_080dc7cc
	.global Func_080c84b8
	.thumb_func
Func_080c84b8:
	overlay_veneer Func_080dc7e8
	.global Func_080c84c0
	.thumb_func
Func_080c84c0:
	overlay_veneer Func_080dc6d8
	.global BattleEffect_InitializeSharedSceneFar
	.thumb_func
BattleEffect_InitializeSharedSceneFar:
	overlay_veneer BattleEffect_InitializeSharedScene
.ifdef TLA_EDITION_ES
	.global BattleEffect_ResolvePendingActionsFar
	.thumb_func
BattleEffect_ResolvePendingActionsFar:
	overlay_veneer EventRuntime_ResolvePendingActions
.else
.ifdef TLA_EDITION_IT
	.global BattleEffect_ResolvePendingActionsFar
	.thumb_func
BattleEffect_ResolvePendingActionsFar:
	overlay_veneer EventRuntime_ResolvePendingActions
.else
	.global BattleFx_PrepareBufferInterpolationFar
	.thumb_func
BattleFx_PrepareBufferInterpolationFar:
	overlay_veneer BattleFx_PrepareBufferInterpolation
.endif
.endif
