	.section .rodata
	.global BattleEffect_SpiderWebSheet
BattleEffect_SpiderWebSheet:
	.incbin "games/tla/src/graphics/battle_effects/spider_web.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/spider_web.bitmap.lz"
	.byte 0x18, 0x85	@ PACKER: left in the buffer by BattleEffect_FiveModeImage+0x25a
