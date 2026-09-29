@ BattleFx_SpiderWebSheet: a 32x32 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_SpiderWebSheet
BattleFx_SpiderWebSheet:
	.incbin "GRAPHICS/BATTLE_FX/SPIDER_WEB.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SPIDER_WEB.bitmap.lz"
