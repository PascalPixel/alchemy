	.section .rodata
	.global BattleEffect_MarsDjinnSheet
BattleEffect_MarsDjinnSheet:
	.incbin "games/tbs/src/graphics/battle_fx/mars_djinn.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/mars_djinn.bitmap.lz"
	.byte 0x04	@ PACKER: left in the buffer by BattleEffect_StarDotSheet+0x3db
