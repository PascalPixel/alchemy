.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080c8390
	.thumb_func
Func_080c8390:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_SetBufferWord
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_SetBufferWord
.else
	overlay_veneer BattleFx_StartBufferInterpolation
.endif
.endif
	.global Func_080c8398
	.thumb_func
Func_080c8398:
.ifdef TLA_EDITION_ES
	overlay_veneer EventRuntime_StartScreen
.else
.ifdef TLA_EDITION_IT
	overlay_veneer EventRuntime_StartScreen
.else
	overlay_veneer Runtime_ScheduleCallbackAndReleaseBlock32B
.endif
.endif
	.global Func_080c83a0
	.thumb_func
Func_080c83a0:
.ifdef TLA_EDITION_ES
	overlay_veneer EventRuntime_FinishScreen
.else
.ifdef TLA_EDITION_IT
	overlay_veneer EventRuntime_FinishScreen
.else
	overlay_veneer BattleFx_SetPrimaryBufferValue
.endif
.endif
