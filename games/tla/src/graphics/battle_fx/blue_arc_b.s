	.section .rodata
	.global BattleEffect_BlueArcSheetB
BattleEffect_BlueArcSheetB:
	.incbin "games/tla/src/graphics/battle_fx/blue_arc_b.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/blue_arc_b.bitmap.lz"
	.byte 0xaa, 0x30	@ PACKER: left in the buffer by BattleEffect_EmberStreakSheet+0xace
