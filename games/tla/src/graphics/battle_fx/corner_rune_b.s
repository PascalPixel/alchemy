	.section .rodata
	.global BattleEffect_CornerRuneBSheet
BattleEffect_CornerRuneBSheet:
	.incbin "games/tla/src/graphics/battle_fx/corner_rune_b.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/corner_rune_b.bitmap.lz"
	.byte 0x47	@ PACKER: left in the buffer by BattleEffect_CornerRuneASheet+0x947
