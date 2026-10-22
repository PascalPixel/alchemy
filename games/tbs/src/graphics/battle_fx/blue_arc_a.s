	.section .rodata
	.global BattleEffect_BlueArcSheetA
BattleEffect_BlueArcSheetA:
	.incbin "games/tbs/src/graphics/battle_fx/blue_arc_a.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/blue_arc_a.bitmap.lz"
	.byte 0x15, 0xc0, 0x04	@ PACKER: left in the buffer by BattleEffect_EmberStreakSheet+0x7d5
