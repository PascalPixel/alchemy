	.section .rodata
	.global BattleFx_SpiderWebSheet
BattleFx_SpiderWebSheet:
	.incbin "games/tla/src/graphics/battle_fx/spider_web.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/spider_web.bitmap.lz"
	.byte 0x18, 0x85	@ PACKER: left in the buffer by BattleFx_FiveModeImage+0x25a
