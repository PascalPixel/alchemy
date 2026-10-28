	.section .rodata
	.global BattleEffect_JupiterDjinnSheet
BattleEffect_JupiterDjinnSheet:
	.incbin "games/tla/src/graphics/battle_effects/jupiter_djinn.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/jupiter_djinn.bitmap.lz"
	.byte 0x13	@ PACKER: left in the buffer by BattleEffect_MarsDjinnSheet+0x3a3
