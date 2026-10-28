.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global FieldParticle_UpdateFar
	.thumb_func
FieldParticle_UpdateFar:
	overlay_veneer FieldParticle_Update
	.global FieldParticle_HasPendingTargetFar
	.thumb_func
FieldParticle_HasPendingTargetFar:
	overlay_veneer FieldParticle_HasPendingTarget
	.global FieldParticle_SetPositionFar
	.thumb_func
FieldParticle_SetPositionFar:
	overlay_veneer FieldParticle_SetPosition
	.global FieldParticle_SetActorModeFar
	.thumb_func
FieldParticle_SetActorModeFar:
	overlay_veneer FieldParticle_SetActorMode
	.global FieldParticle_SetCallbackFar
	.thumb_func
FieldParticle_SetCallbackFar:
	overlay_veneer FieldParticle_SetCallback
	.global FieldParticle_InitializeFar
	.thumb_func
FieldParticle_InitializeFar:
	overlay_veneer FieldParticle_Initialize
	.global BattleEffect_ClearOwnedSlotFar
	.thumb_func
BattleEffect_ClearOwnedSlotFar:
	overlay_veneer BattleEffect_ClearOwnedSlot
