.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global BattleEffect_ApplyColorToTargetBufferFar
	.thumb_func
BattleEffect_ApplyColorToTargetBufferFar:
	overlay_veneer BattleEffect_ApplyColorToTargetBuffer
	.global BattleEffect_ApplyColorToSourceBufferFar
	.thumb_func
BattleEffect_ApplyColorToSourceBufferFar:
	overlay_veneer BattleEffect_ApplyColorToSourceBuffer
	.global BattleEffect_InterpolateBuffersFar
	.thumb_func
BattleEffect_InterpolateBuffersFar:
	overlay_veneer BattleEffect_InterpolateBuffers
