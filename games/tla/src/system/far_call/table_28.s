.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global BattleFx_StartBufferInterpolationFar
	.thumb_func
BattleFx_StartBufferInterpolationFar:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_SetBufferWord
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_SetBufferWord
.else
	overlay_veneer BattleFx_StartBufferInterpolation
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
	.global BattleFx_SetPrimaryBufferValueFar
	.thumb_func
BattleFx_SetPrimaryBufferValueFar:
.ifdef TLA_EDITION_ES
	overlay_veneer EventRuntime_FinishScreen
.else
.ifdef TLA_EDITION_IT
	overlay_veneer EventRuntime_FinishScreen
.else
	overlay_veneer BattleFx_SetPrimaryBufferValue
.endif
.endif
