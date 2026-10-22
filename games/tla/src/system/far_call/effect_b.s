.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global EffectSlot_UpdateFar
	.thumb_func
EffectSlot_UpdateFar:
	overlay_veneer EffectSlot_Update
	.global EffectSlot_HasPendingTargetFar
	.thumb_func
EffectSlot_HasPendingTargetFar:
	overlay_veneer EffectSlot_HasPendingTarget
	.global EffectSlot_SetPositionFar
	.thumb_func
EffectSlot_SetPositionFar:
	overlay_veneer EffectSlot_SetPosition
	.global EffectSlot_SetObjectModeFar
	.thumb_func
EffectSlot_SetObjectModeFar:
	overlay_veneer EffectSlot_SetObjectMode
	.global EffectSlot_SetCallbackFar
	.thumb_func
EffectSlot_SetCallbackFar:
	overlay_veneer EffectSlot_SetCallback
	.global EffectSlot_InitializeFar
	.thumb_func
EffectSlot_InitializeFar:
	overlay_veneer EffectSlot_Initialize
	.global BattleEffect_ClearOwnedSlotFar
	.thumb_func
BattleEffect_ClearOwnedSlotFar:
	overlay_veneer BattleEffect_ClearOwnedSlot
