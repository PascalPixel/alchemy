.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Func_080c82a8
	.thumb_func
Func_080c82a8:
.ifdef TLA_EDITION_ES
	overlay_veneer Event_SelectSpawnObjectMode
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Event_SelectSpawnObjectMode
.else
	overlay_veneer Func_080d5bec
.endif
.endif
	.global Object_GetTriggerTileAheadOfCurrentFar
	.thumb_func
Object_GetTriggerTileAheadOfCurrentFar:
	overlay_veneer Func_080cc54c
	.global Func_080c82b8
	.thumb_func
Func_080c82b8:
	overlay_veneer Func_080ca6a4
	.global Func_080c82c0
	.thumb_func
Func_080c82c0:
	overlay_veneer Func_080cd91c
	.global Func_080c82c8
	.thumb_func
Func_080c82c8:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_FindNearestObject
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_FindNearestObject
.else
	overlay_veneer Func_080dd054
.endif
.endif
	.global Func_080c82d0
	.thumb_func
Func_080c82d0:
	overlay_veneer Func_080decb8
	.global BattleFx_FindNearestObjectFar
	.thumb_func
BattleFx_FindNearestObjectFar:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080dd054
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080dd054
.else
	overlay_veneer BattleFx_FindNearestObject
.endif
.endif
	.global Func_080c82e0
	.thumb_func
Func_080c82e0:
	overlay_veneer Func_080d4d08
	.global Func_080c82e8
	.thumb_func
Func_080c82e8:
.ifdef TLA_EDITION_ES
	overlay_veneer EventRuntime_PrepareCurrentObject
.else
.ifdef TLA_EDITION_IT
	overlay_veneer EventRuntime_PrepareCurrentObject
.else
	overlay_veneer Func_080d50f8
.endif
.endif
	.global Func_080c82f0
	.thumb_func
Func_080c82f0:
	overlay_veneer Func_080d53b8
	.global EventRuntime_PrepareCurrentObjectFar
	.thumb_func
EventRuntime_PrepareCurrentObjectFar:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080d2260
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080d2260
.else
	overlay_veneer EventRuntime_PrepareCurrentObject
.endif
.endif
	.global Func_080c8300
	.thumb_func
Func_080c8300:
	overlay_veneer Func_080d690c
	.global Func_080c8308
	.thumb_func
Func_080c8308:
	overlay_veneer Func_080d7304
	.global Func_080c8310
	.thumb_func
Func_080c8310:
	overlay_veneer Func_080d73b4
	.global Func_080c8318
	.thumb_func
Func_080c8318:
	overlay_veneer Func_080d73e0
	.global Func_080c8320
	.thumb_func
Func_080c8320:
	overlay_veneer Func_080d7408
	.global Func_080c8328
	.thumb_func
Func_080c8328:
	overlay_veneer Func_080d7430
	.global Func_080c8330
	.thumb_func
Func_080c8330:
	overlay_veneer Func_080d6b90
	.global Func_080c8338
	.thumb_func
Func_080c8338:
.ifdef TLA_EDITION_ES
	overlay_veneer DisplayTransition_InitializeState
.else
.ifdef TLA_EDITION_IT
	overlay_veneer DisplayTransition_InitializeState
.else
	overlay_veneer Func_080d7240
.endif
.endif
	.global Func_080c8340
	.thumb_func
Func_080c8340:
.ifdef TLA_EDITION_ES
	overlay_veneer DisplayTransition_Start
.else
.ifdef TLA_EDITION_IT
	overlay_veneer DisplayTransition_Start
.else
	overlay_veneer Func_080d00f8
.endif
.endif
	.global DisplayTransition_InitializeStateFar
	.thumb_func
DisplayTransition_InitializeStateFar:
.ifdef TLA_EDITION_ES
	overlay_veneer DisplayTransition_Finish
.else
.ifdef TLA_EDITION_IT
	overlay_veneer DisplayTransition_Finish
.else
	overlay_veneer DisplayTransition_InitializeState
.endif
.endif
	.global DisplayTransition_StartFar
	.thumb_func
DisplayTransition_StartFar:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080d01cc
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080d01cc
.else
	overlay_veneer DisplayTransition_Start
.endif
.endif
	.global DisplayTransition_FinishFar
	.thumb_func
DisplayTransition_FinishFar:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080d0520
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080d0520
.else
	overlay_veneer DisplayTransition_Finish
.endif
.endif
	.global Func_080c8360
	.thumb_func
Func_080c8360:
	overlay_veneer Func_080d0748
	.global Func_080c8368
	.thumb_func
Func_080c8368:
	overlay_veneer Func_080ccec8
	.global BattleEffect_InitializeBuffersFar
	.thumb_func
BattleEffect_InitializeBuffersFar:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_ApplyColorToSourceBuffer
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_ApplyColorToSourceBuffer
.else
	overlay_veneer BattleEffect_InitializeBuffers
.endif
.endif
