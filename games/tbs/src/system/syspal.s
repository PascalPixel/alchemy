	.section .rodata
	.global Ui_WindowPalette
Ui_WindowPalette:
	.incbin "system/window.gbapal"
	.global System_BasicColorPalette
System_BasicColorPalette:
	.incbin "system/basic.gbapal"
