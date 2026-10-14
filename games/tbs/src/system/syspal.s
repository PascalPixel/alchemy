	.section .rodata
	.global Ui_WindowPalette
Ui_WindowPalette:
	.incbin "games/common/src/system/memory/window.gbapal"
	.global System_BasicColorPalette
System_BasicColorPalette:
	.incbin "games/common/src/system/memory/basic.gbapal"
