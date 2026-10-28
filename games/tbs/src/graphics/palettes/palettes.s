	.section .rodata
	.global gWindow_Palette
gWindow_Palette:
	.incbin "games/tbs/src/graphics/palettes/window.gbapal"
	.global gBasicSpritePalettes
gBasicSpritePalettes:
	.incbin "games/tbs/src/graphics/palettes/basic.gbapal"
