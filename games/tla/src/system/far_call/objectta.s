.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Event_StartScreenTransitionFar
	.thumb_func
Event_StartScreenTransitionFar:
	overlay_veneer EventRuntime_StartScreen
	.global Event_FinishScreenTransitionFar
	.thumb_func
Event_FinishScreenTransitionFar:
	overlay_veneer EventRuntime_FinishScreen
	.global Event_WaitTransitionFramesFar
	.thumb_func
Event_WaitTransitionFramesFar:
	overlay_veneer EventRuntime_WaitDelay
	.global DisplayScroll_InitializeHBlankDmaFar
	.thumb_func
DisplayScroll_InitializeHBlankDmaFar:
	overlay_veneer Func_080d6408
	.global DisplayScroll_DisarmHBlankDmaFar
	.thumb_func
DisplayScroll_DisarmHBlankDmaFar:
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
	.global ObjectEvent_GetObjectFar
	.thumb_func
ObjectEvent_GetObjectFar:
	overlay_veneer ObjectEvent_GetObject
	.global ObjectEvent_SetMode4WithCueFar
	.thumb_func
ObjectEvent_SetMode4WithCueFar:
	overlay_veneer ObjectEvent_SetMode4WithCue
	.global ObjectEvent_SetMode5WithCueFar
	.thumb_func
ObjectEvent_SetMode5WithCueFar:
	overlay_veneer ObjectEvent_SetMode5WithCue
	.global ObjectEvent_SetMode2Far
	.thumb_func
ObjectEvent_SetMode2Far:
	overlay_veneer ObjectEvent_SetMode2
	.global ObjectEvent_PlaceOnGroundFar
	.thumb_func
ObjectEvent_PlaceOnGroundFar:
	overlay_veneer ObjectEvent_PlaceOnGround
	.global ObjectEvent_RemoveFar
	.thumb_func
ObjectEvent_RemoveFar:
	overlay_veneer ObjectEvent_Remove
	.global Func_080c8418
	.thumb_func
Func_080c8418:
	overlay_veneer Func_080cf050
	.global Object_SetWideSpriteFar
	.thumb_func
Object_SetWideSpriteFar:
	overlay_veneer Object_SetWideSprite
	.global Object_ReturnHomeFar
	.thumb_func
Object_ReturnHomeFar:
	overlay_veneer Object_ReturnHome
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
	.global ObjectTable_DestroyAtIndexFar
	.thumb_func
ObjectTable_DestroyAtIndexFar:
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
	.global BattleFx_LoadActionEffectResourcesFar
	.thumb_func
BattleFx_LoadActionEffectResourcesFar:
	overlay_veneer BattleFx_LoadActionEffectResources
	.global BattleFx_SetupObjectPairFar
	.thumb_func
BattleFx_SetupObjectPairFar:
	overlay_veneer BattleFx_SetupObjectPair
	.global EffectRuntime_StopCurrentObjectFar
	.thumb_func
EffectRuntime_StopCurrentObjectFar:
	overlay_veneer EffectRuntime_StopCurrentObject
	.global BattleEffect_CleanupSceneObjectsFar
	.thumb_func
BattleEffect_CleanupSceneObjectsFar:
	overlay_veneer BattleEffect_CleanupSceneObjects
	.global EventObject_InitializeFar
	.thumb_func
EventObject_InitializeFar:
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
