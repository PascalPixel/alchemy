	.section .rodata
	.global gWindow_Palette
gWindow_Palette:
	.incbin "games/tbs/src/system/memory/window.gbapal"
	.global gBasicSpritePalettes
gBasicSpritePalettes:
	.incbin "games/tbs/src/system/memory/basic.gbapal"
