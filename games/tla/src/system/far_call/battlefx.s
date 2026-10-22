.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Event_GetSpecialValueFar
	.thumb_func
Event_GetSpecialValueFar:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_SnapScaleToFull
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_SnapScaleToFull
.else
	overlay_veneer Event_GetSpecialValue
.endif
.endif
	.global BattleEffect_StartItemBreakFar
	.thumb_func
BattleEffect_StartItemBreakFar:
.ifdef TLA_EDITION_ES
	overlay_veneer UpdateRisingParticleBurst
.else
.ifdef TLA_EDITION_IT
	overlay_veneer UpdateRisingParticleBurst
.else
	overlay_veneer BattleEffect_StartItemBreak
.endif
.endif
	.global BattleEffect_SnapScaleToFullFar
	.thumb_func
BattleEffect_SnapScaleToFullFar:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleParty_ApplyDrain
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleParty_ApplyDrain
.else
	overlay_veneer BattleEffect_SnapScaleToFull
.endif
.endif
	.global UpdateRisingParticleBurstFar
	.thumb_func
UpdateRisingParticleBurstFar:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleParty_ApplyHealthDelta
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleParty_ApplyHealthDelta
.else
	overlay_veneer UpdateRisingParticleBurst
.endif
.endif
	.global BattleParty_ApplyDrainFar
	.thumb_func
BattleParty_ApplyDrainFar:
.ifdef TLA_EDITION_ES
	overlay_veneer ResourceTable_CountFreeBlocks
.else
.ifdef TLA_EDITION_IT
	overlay_veneer ResourceTable_CountFreeBlocks
.else
	overlay_veneer BattleParty_ApplyDrain
.endif
.endif
	.global BattleParty_ApplyHealthDeltaFar
	.thumb_func
BattleParty_ApplyHealthDeltaFar:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080d295c
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080d295c
.else
	overlay_veneer BattleParty_ApplyHealthDelta
.endif
.endif
	.global Func_080c8550
	.thumb_func
Func_080c8550:
.ifdef TLA_EDITION_ES
	overlay_veneer Encounter_GetEnemyGroupAtSlot
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Encounter_GetEnemyGroupAtSlot
.else
	overlay_veneer ResourceTable_CountFreeBlocks
.endif
.endif
	.global Func_080c8558
	.thumb_func
Func_080c8558:
.ifdef TLA_EDITION_ES
	overlay_veneer Encounter_GetDjinnEnemyGroup
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Encounter_GetDjinnEnemyGroup
.else
	overlay_veneer Func_080d295c
.endif
.endif
	.global Encounter_GetEnemyGroupAtSlotFar
	.thumb_func
Encounter_GetEnemyGroupAtSlotFar:
.ifdef TLA_EDITION_ES
	overlay_veneer Party_PlaySavedMusic
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Party_PlaySavedMusic
.else
	overlay_veneer Encounter_GetEnemyGroupAtSlot
.endif
.endif
	.global Encounter_GetDjinnEnemyGroupFar
	.thumb_func
Encounter_GetDjinnEnemyGroupFar:
.ifdef TLA_EDITION_ES
	overlay_veneer Scene_ResolveInteractionResult
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Scene_ResolveInteractionResult
.else
	overlay_veneer Encounter_GetDjinnEnemyGroup
.endif
.endif
	.global Party_PlaySavedMusicFar
	.thumb_func
Party_PlaySavedMusicFar:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c9dc8
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c9dc8
.else
	overlay_veneer Party_PlaySavedMusic
.endif
.endif
	.global Scene_ResolveInteractionResultFar
	.thumb_func
Scene_ResolveInteractionResultFar:
.ifdef TLA_EDITION_ES
	overlay_veneer EffectSlot_Update
.else
.ifdef TLA_EDITION_IT
	overlay_veneer EffectSlot_Update
.else
	overlay_veneer Scene_ResolveInteractionResult
.endif
.endif
	.global Func_080c8580
	.thumb_func
Func_080c8580:
.ifdef TLA_EDITION_ES
	overlay_veneer EffectSlot_HasPendingTarget
.else
.ifdef TLA_EDITION_IT
	overlay_veneer EffectSlot_HasPendingTarget
.else
	overlay_veneer Func_080d7788
.endif
.endif
