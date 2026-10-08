.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global BattleFx_ApplyColorToTargetBufferFar
	.thumb_func
BattleFx_ApplyColorToTargetBufferFar:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_InterpolateBuffers
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_InterpolateBuffers
.else
	overlay_veneer BattleFx_ApplyColorToTargetBuffer
.endif
.endif
	.global BattleFx_ApplyColorToSourceBufferFar
	.thumb_func
BattleFx_ApplyColorToSourceBufferFar:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_StartBufferInterpolation
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_StartBufferInterpolation
.else
	overlay_veneer BattleFx_ApplyColorToSourceBuffer
.endif
.endif
	.global BattleFx_InterpolateBuffersFar
	.thumb_func
BattleFx_InterpolateBuffersFar:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080d0c50
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080d0c50
.else
	overlay_veneer BattleFx_InterpolateBuffers
.endif
.endif
