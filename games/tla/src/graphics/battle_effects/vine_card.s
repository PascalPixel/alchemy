	.section .rodata
	.global BattleEffect_VineCardSheet
BattleEffect_VineCardSheet:
	.incbin "games/tla/src/graphics/battle_effects/vine_card.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/vine_card.bitmap.lz"
	.byte 0x1d, 0x6a, 0x00	@ PACKER: left in the buffer by Resource_Data0DD+0x1039
