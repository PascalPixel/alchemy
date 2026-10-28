.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Func_080c82a8
	.thumb_func
Func_080c82a8:
	overlay_veneer Func_080d5bec
	.global Actor_GetTriggerTileAheadOfCurrentFar
	.thumb_func
Actor_GetTriggerTileAheadOfCurrentFar:
	overlay_veneer Func_080cc54c
	.global Func_080c82b8
	.thumb_func
Func_080c82b8:
	overlay_veneer Party_RemapCharacterIdByFlags
	.global Func_080c82c0
	.thumb_func
Func_080c82c0:
	overlay_veneer Field_FindNearbyTalkTarget
	.global Func_080c82c8
	.thumb_func
Func_080c82c8:
	overlay_veneer Func_080dd054
	.global Func_080c82d0
	.thumb_func
Func_080c82d0:
	overlay_veneer Func_080decb8
	.global BattleEffect_FindNearestObjectFar
	.thumb_func
BattleEffect_FindNearestObjectFar:
	overlay_veneer BattleEffect_FindNearestObject
	.global Func_080c82e0
	.thumb_func
Func_080c82e0:
	overlay_veneer Func_080d4d08
	.global Func_080c82e8
	.thumb_func
Func_080c82e8:
	overlay_veneer Func_080d50f8
	.global Func_080c82f0
	.thumb_func
Func_080c82f0:
	overlay_veneer Func_080d53b8
	.global Leader_StopMovingFar
	.thumb_func
Leader_StopMovingFar:
	overlay_veneer Leader_StopMoving
	.global Func_080c8300
	.thumb_func
Func_080c8300:
	overlay_veneer Func_080d690c
	.global Func_080c8308
	.thumb_func
Func_080c8308:
	overlay_veneer BattleEffect_StartTwelveFrameBlend
	.global Func_080c8310
	.thumb_func
Func_080c8310:
	overlay_veneer BattleEffect_SetBlock30ValuesMaxZero
	.global Func_080c8318
	.thumb_func
Func_080c8318:
	overlay_veneer BattleEffect_SetBlock30Values12Zero
	.global Func_080c8320
	.thumb_func
Func_080c8320:
	overlay_veneer BattleEffect_SetBlock30Values128One
	.global Func_080c8328
	.thumb_func
Func_080c8328:
	overlay_veneer BattleEffect_StartBufferBlend
	.global Func_080c8330
	.thumb_func
Func_080c8330:
	overlay_veneer Func_080d6b90
	.global Func_080c8338
	.thumb_func
Func_080c8338:
	overlay_veneer FieldEffect_InitializeSparkles
	.global Func_080c8340
	.thumb_func
Func_080c8340:
	overlay_veneer ScreenTransition_InitializeBattleEffectState
	.global ScreenTransition_InitializeStateFar
	.thumb_func
ScreenTransition_InitializeStateFar:
	overlay_veneer ScreenTransition_InitializeState
	.global ScreenTransition_StartFar
	.thumb_func
ScreenTransition_StartFar:
	overlay_veneer ScreenTransition_Start
	.global ScreenTransition_FinishFar
	.thumb_func
ScreenTransition_FinishFar:
	overlay_veneer ScreenTransition_Finish
	.global Func_080c8360
	.thumb_func
Func_080c8360:
	overlay_veneer Map_ClearLayerFlags
	.global Func_080c8368
	.thumb_func
Func_080c8368:
	overlay_veneer Func_080ccec8
	.global BattleEffect_InitializeBuffersFar
	.thumb_func
BattleEffect_InitializeBuffersFar:
	overlay_veneer BattleEffect_InitializeBuffers
