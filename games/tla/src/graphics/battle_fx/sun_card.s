	.section .rodata
	.global BattleEffect_SunCardSheet
BattleEffect_SunCardSheet:
	.incbin "games/tla/src/graphics/battle_fx/sun_card.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/sun_card.bitmap.lz"
	.byte 0x01	@ PACKER: left in the buffer by Resource_Data0DD+0x65f
