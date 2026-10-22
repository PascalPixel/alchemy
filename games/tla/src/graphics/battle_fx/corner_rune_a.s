	.section .rodata
	.global BattleEffect_CornerRuneASheet
BattleEffect_CornerRuneASheet:
	.incbin "games/tla/src/graphics/battle_fx/corner_rune_a.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/corner_rune_a.bitmap.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x84, 0xa5, 0xbe	@ PACKER: left in the buffer by BattleEffect_CrystalBitmapSheet+0x985
	.else
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
