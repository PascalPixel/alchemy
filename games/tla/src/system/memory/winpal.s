	.section .rodata
@ The Broken Seal's gUi_WindowPalette, byte for byte
	.global gUi_WindowPalette
gUi_WindowPalette:
	.incbin "games/common/src/system/memory/window.gbapal"
