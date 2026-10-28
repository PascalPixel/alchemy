	.section .rodata
	.global BattleEffect_BlueArcSheetA
BattleEffect_BlueArcSheetA:
	.incbin "games/tla/src/graphics/battle_effects/blue_arc_a.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/blue_arc_a.bitmap.lz"
	.byte 0x15, 0xc0, 0x04	@ PACKER: left in the buffer by BattleEffect_EmberStreakSheet+0x7d5
