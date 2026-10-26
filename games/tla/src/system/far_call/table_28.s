.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global BattleEffect_StartBufferInterpolationFar
	.thumb_func
BattleEffect_StartBufferInterpolationFar:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_SetPrimaryBufferValue
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_SetPrimaryBufferValue
.else
	overlay_veneer BattleEffect_StartBufferInterpolation
.endif
.endif
	.global BattlePalette_StopBlendAndReleaseBufferFar
	.thumb_func
BattlePalette_StopBlendAndReleaseBufferFar:
.ifdef TLA_EDITION_ES
	overlay_veneer EventRuntime_StartScreen
.else
.ifdef TLA_EDITION_IT
	overlay_veneer EventRuntime_StartScreen
.else
	overlay_veneer BattlePalette_StopBlendAndReleaseBuffer
.endif
.endif
	.global BattleEffect_SetPrimaryBufferValueFar
	.thumb_func
BattleEffect_SetPrimaryBufferValueFar:
.ifdef TLA_EDITION_ES
	overlay_veneer EventRuntime_FinishScreen
.else
.ifdef TLA_EDITION_IT
	overlay_veneer EventRuntime_FinishScreen
.else
	overlay_veneer BattleEffect_SetPrimaryBufferValue
.endif
.endif
