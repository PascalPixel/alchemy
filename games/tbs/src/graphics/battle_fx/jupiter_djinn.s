	.section .rodata
	.global BattleAnim_JupiterDjinnSheet
BattleAnim_JupiterDjinnSheet:
	.incbin "games/tbs/src/graphics/battle_fx/jupiter_djinn.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/jupiter_djinn.bitmap.lz"
	.byte 0x13	@ PACKER: left in the buffer by BattleAnim_MarsDjinnSheet+0x3a3
