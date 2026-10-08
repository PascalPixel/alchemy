.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global EffectSlot_UpdateFar
	.thumb_func
EffectSlot_UpdateFar:
	overlay_veneer EffectSlot_Update
	.global BattleFx_HasReachedTargetFar
	.thumb_func
BattleFx_HasReachedTargetFar:
	overlay_veneer BattleFx_HasReachedTarget
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
	.global BattleFx_ClearOwnedSlotFar
	.thumb_func
BattleFx_ClearOwnedSlotFar:
	overlay_veneer BattleFx_ClearOwnedSlot
