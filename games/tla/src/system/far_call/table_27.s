.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global BattleEffect_ApplyColorToTargetBufferFar
	.thumb_func
BattleEffect_ApplyColorToTargetBufferFar:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_InterpolateBuffers
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_InterpolateBuffers
.else
	overlay_veneer BattleEffect_ApplyColorToTargetBuffer
.endif
.endif
	.global BattleEffect_ApplyColorToSourceBufferFar
	.thumb_func
BattleEffect_ApplyColorToSourceBufferFar:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_StartBufferInterpolation
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_StartBufferInterpolation
.else
	overlay_veneer BattleEffect_ApplyColorToSourceBuffer
.endif
.endif
	.global BattleEffect_InterpolateBuffersFar
	.thumb_func
BattleEffect_InterpolateBuffersFar:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080d0c50
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080d0c50
.else
	overlay_veneer BattleEffect_InterpolateBuffers
.endif
.endif
