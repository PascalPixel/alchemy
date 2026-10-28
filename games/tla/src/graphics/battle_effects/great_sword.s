	.section .rodata
	.global BattleEffect_GreatSwordSheet
BattleEffect_GreatSwordSheet:
	.incbin "games/tla/src/graphics/battle_effects/great_sword.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/great_sword.bitmap.lz"
	.byte 0x70, 0xa8, 0x48	@ PACKER: left in the buffer by Resource_Data0DA+0x6bd
