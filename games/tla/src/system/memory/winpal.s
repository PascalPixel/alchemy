	.section .rodata
@ The Broken Seal's Ui_WindowPalette, byte for byte
	.global Ui_WindowPalette
Ui_WindowPalette:
	.incbin "games/common/src/system/memory/window.gbapal"
