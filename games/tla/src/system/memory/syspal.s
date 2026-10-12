	.section .rodata
@ The Broken Seal's System_BasicColorPalette, byte for byte
	.global System_BasicColorPalette
System_BasicColorPalette:
	.incbin "common/system/memory/basic.gbapal"
