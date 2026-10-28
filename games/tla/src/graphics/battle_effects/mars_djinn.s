	.section .rodata
	.global BattleEffect_MarsDjinnSheet
BattleEffect_MarsDjinnSheet:
	.incbin "games/tla/src/graphics/battle_effects/mars_djinn.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/mars_djinn.bitmap.lz"
	.byte 0x04	@ PACKER: left in the buffer by BattleEffect_StarDotSheet+0x3db
