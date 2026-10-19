	.section .rodata
	.global gUi_WindowPalette
gUi_WindowPalette:
	.incbin "games/tbs/src/system/memory/window.gbapal"
	.global gSystem_BasicColorPalette
gSystem_BasicColorPalette:
	.incbin "games/tbs/src/system/memory/basic.gbapal"
