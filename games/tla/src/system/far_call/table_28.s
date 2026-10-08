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
	.global Runtime_ScheduleCallbackAndReleaseBlock32BFar
	.thumb_func
Runtime_ScheduleCallbackAndReleaseBlock32BFar:
.ifdef TLA_EDITION_ES
	overlay_veneer EventRuntime_StartScreen
.else
.ifdef TLA_EDITION_IT
	overlay_veneer EventRuntime_StartScreen
.else
	overlay_veneer Runtime_ScheduleCallbackAndReleaseBlock32B
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
