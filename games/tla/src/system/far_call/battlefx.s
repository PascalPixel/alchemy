.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Event_GetSpecialValueFar
	.thumb_func
Event_GetSpecialValueFar:
	overlay_veneer Event_GetSpecialValue
	.global BattleEffect_StartItemBreakFar
	.thumb_func
BattleEffect_StartItemBreakFar:
	overlay_veneer BattleEffect_StartItemBreak
	.global BattleEffect_SnapScaleToFullFar
	.thumb_func
BattleEffect_SnapScaleToFullFar:
	overlay_veneer BattleEffect_SnapScaleToFull
	.global UpdateRisingParticleBurstFar
	.thumb_func
UpdateRisingParticleBurstFar:
	overlay_veneer UpdateRisingParticleBurst
	.global BattleParty_ApplyDrainFar
	.thumb_func
BattleParty_ApplyDrainFar:
	overlay_veneer BattleParty_ApplyDrain
	.global BattleParty_ApplyHealthDeltaFar
	.thumb_func
BattleParty_ApplyHealthDeltaFar:
	overlay_veneer BattleParty_ApplyHealthDelta
	.global Func_080c8550
	.thumb_func
Func_080c8550:
	overlay_veneer ResourceTable_CountFreeBlocks
	.global Func_080c8558
	.thumb_func
Func_080c8558:
	overlay_veneer Func_080d295c
	.global Encounter_GetEnemyGroupAtSlotFar
	.thumb_func
Encounter_GetEnemyGroupAtSlotFar:
	overlay_veneer Encounter_GetEnemyGroupAtSlot
	.global Encounter_GetDjinnEnemyGroupFar
	.thumb_func
Encounter_GetDjinnEnemyGroupFar:
	overlay_veneer Encounter_GetDjinnEnemyGroup
	.global Party_PlaySavedMusicFar
	.thumb_func
Party_PlaySavedMusicFar:
	overlay_veneer Party_PlaySavedMusic
	.global Scene_ResolveInteractionResultFar
	.thumb_func
Scene_ResolveInteractionResultFar:
	overlay_veneer Scene_ResolveInteractionResult
	.global Func_080c8580
	.thumb_func
Func_080c8580:
	overlay_veneer Func_080d7788
