	.section .rodata
	.global BattleAnim_MarsDjinnSheet
BattleAnim_MarsDjinnSheet:
	.incbin "games/tbs/src/graphics/battle_fx/mars_djinn.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/mars_djinn.bitmap.lz"
	.byte 0x04	@ PACKER: left in the buffer by BattleAnim_StarDotSheet+0x3db
