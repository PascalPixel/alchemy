	.section .rodata
	.global BattleAnim_JupiterDjinnSmallSheet
BattleAnim_JupiterDjinnSmallSheet:
	.incbin "games/tbs/src/graphics/battle_fx/jupiter_djinn_small.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/jupiter_djinn_small.bitmap.lz"
	.byte 0x0b, 0x1f	@ PACKER: left in the buffer by BattleAnim_JupiterDjinnSheet+0x25a
