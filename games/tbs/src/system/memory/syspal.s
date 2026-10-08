	.section .rodata
	.global Ui_WindowPalette
Ui_WindowPalette:
	.incbin "system/memory/window.gbapal"
	.global System_BasicColorPalette
System_BasicColorPalette:
	.incbin "system/memory/basic.gbapal"
