	.section .rodata
	.global BattleAnim_MarsDjinnSmallSheet
BattleAnim_MarsDjinnSmallSheet:
	.incbin "games/tbs/src/graphics/battle_fx/mars_djinn_small.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/mars_djinn_small.bitmap.lz"
	.byte 0x8a, 0x14, 0x39	@ PACKER: left in the buffer by BattleAnim_MarsDjinnSheet+0x289
