.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
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
	.global Func_080c85a0
	.thumb_func
Func_080c85a0:
	overlay_veneer EffectSlot_SetObjectMode
	.global Func_080c85a8
	.thumb_func
Func_080c85a8:
	overlay_veneer Func_080ebeb4
	.global Func_080c85b0
	.thumb_func
Func_080c85b0:
	overlay_veneer Func_080ebec8
	.global BattleFx_ClearOwnedSlotFar
	.thumb_func
BattleFx_ClearOwnedSlotFar:
	overlay_veneer BattleFx_ClearOwnedSlot
