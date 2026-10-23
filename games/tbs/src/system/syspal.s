	.section .rodata
	.global gWindow_Palette
gWindow_Palette:
	.incbin "games/tbs/src/system/memory/window.gbapal"
	.global gSystem_BasicColorPalette
gSystem_BasicColorPalette:
	.incbin "games/tbs/src/system/memory/basic.gbapal"
