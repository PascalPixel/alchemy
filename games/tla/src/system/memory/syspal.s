	.section .rodata
@ The Broken Seal's System_BasicColorPalette, byte for byte
	.global System_BasicColorPalette
System_BasicColorPalette:
	.incbin "games/common/src/system/memory/basic.gbapal"
