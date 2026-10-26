.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Func_080c82a8
	.thumb_func
Func_080c82a8:
.ifdef TLA_EDITION_ES
	overlay_veneer Party_RemapCharacterIdByFlags
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Party_RemapCharacterIdByFlags
.else
	overlay_veneer Func_080d5bec
.endif
.endif
	.global Object_GetTriggerTileAheadOfCurrentFar
	.thumb_func
Object_GetTriggerTileAheadOfCurrentFar:
.ifdef TLA_EDITION_ES
	overlay_veneer Field_FindNearbyTalkTarget
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Field_FindNearbyTalkTarget
.else
	overlay_veneer Func_080cc54c
.endif
.endif
	.global Func_080c82b8
	.thumb_func
Func_080c82b8:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080ca6a4
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080ca6a4
.else
	overlay_veneer Party_RemapCharacterIdByFlags
.endif
.endif
	.global Func_080c82c0
	.thumb_func
Func_080c82c0:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080decb8
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080decb8
.else
	overlay_veneer Field_FindNearbyTalkTarget
.endif
.endif
	.global Func_080c82c8
	.thumb_func
Func_080c82c8:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_FindNearestObject
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_FindNearestObject
.else
	overlay_veneer Func_080dd054
.endif
.endif
	.global Func_080c82d0
	.thumb_func
Func_080c82d0:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080d4d08
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080d4d08
.else
	overlay_veneer Func_080decb8
.endif
.endif
	.global BattleEffect_FindNearestObjectFar
	.thumb_func
BattleEffect_FindNearestObjectFar:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080d50f8
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080d50f8
.else
	overlay_veneer BattleEffect_FindNearestObject
.endif
.endif
	.global Func_080c82e0
	.thumb_func
Func_080c82e0:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080d53b8
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080d53b8
.else
	overlay_veneer Func_080d4d08
.endif
.endif
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
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080d690c
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080d690c
.else
	overlay_veneer Func_080d53b8
.endif
.endif
	.global EventRuntime_PrepareCurrentObjectFar
	.thumb_func
EventRuntime_PrepareCurrentObjectFar:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_StartTwelveFrameBlend
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_StartTwelveFrameBlend
.else
	overlay_veneer EventRuntime_PrepareCurrentObject
.endif
.endif
	.global Func_080c8300
	.thumb_func
Func_080c8300:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_SetBlock30ValuesMaxZero
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_SetBlock30ValuesMaxZero
.else
	overlay_veneer Func_080d690c
.endif
.endif
	.global Func_080c8308
	.thumb_func
Func_080c8308:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_SetBlock30Values12Zero
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_SetBlock30Values12Zero
.else
	overlay_veneer BattleEffect_StartTwelveFrameBlend
.endif
.endif
	.global Func_080c8310
	.thumb_func
Func_080c8310:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_SetBlock30Values128One
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_SetBlock30Values128One
.else
	overlay_veneer BattleEffect_SetBlock30ValuesMaxZero
.endif
.endif
	.global Func_080c8318
	.thumb_func
Func_080c8318:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_StartBufferBlend
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_StartBufferBlend
.else
	overlay_veneer BattleEffect_SetBlock30Values12Zero
.endif
.endif
	.global Func_080c8320
	.thumb_func
Func_080c8320:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080d7408
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080d7408
.else
	overlay_veneer BattleEffect_SetBlock30Values128One
.endif
.endif
	.global Func_080c8328
	.thumb_func
Func_080c8328:
.ifdef TLA_EDITION_ES
	overlay_veneer FieldEffect_InitializeSparkles
.else
.ifdef TLA_EDITION_IT
	overlay_veneer FieldEffect_InitializeSparkles
.else
	overlay_veneer BattleEffect_StartBufferBlend
.endif
.endif
	.global Func_080c8330
	.thumb_func
Func_080c8330:
.ifdef TLA_EDITION_ES
	overlay_veneer DisplayTransition_InitializeBattleEffectState
.else
.ifdef TLA_EDITION_IT
	overlay_veneer DisplayTransition_InitializeBattleEffectState
.else
	overlay_veneer Func_080d6b90
.endif
.endif
	.global Func_080c8338
	.thumb_func
Func_080c8338:
.ifdef TLA_EDITION_ES
	overlay_veneer DisplayTransition_InitializeState
.else
.ifdef TLA_EDITION_IT
	overlay_veneer DisplayTransition_InitializeState
.else
	overlay_veneer FieldEffect_InitializeSparkles
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
	overlay_veneer DisplayTransition_InitializeBattleEffectState
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
	overlay_veneer Map_ClearLayerFlags
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Map_ClearLayerFlags
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
.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_InitializeBuffers
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_InitializeBuffers
.else
	overlay_veneer Map_ClearLayerFlags
.endif
.endif
	.global Func_080c8368
	.thumb_func
Func_080c8368:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ApplyColorToTargetBuffer
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ApplyColorToTargetBuffer
.else
	overlay_veneer Func_080ccec8
.endif
.endif
	.global BattleEffect_InitializeBuffersFar
	.thumb_func
BattleEffect_InitializeBuffersFar:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_ApplyColorToSourceBuffer
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_ApplyColorToSourceBuffer
.else
	overlay_veneer BattleEffect_InitializeBuffers
.endif
.endif
