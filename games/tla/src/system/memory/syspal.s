	.section .rodata
@ The Broken Seal's gSystem_BasicColorPalette, byte for byte
	.global gSystem_BasicColorPalette
gSystem_BasicColorPalette:
	.incbin "games/common/src/system/memory/basic.gbapal"
