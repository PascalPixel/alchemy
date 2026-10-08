	.section .rodata
	.global Ui_WindowPalette
Ui_WindowPalette:
	.incbin "SYSTEM/MEMORY/WINDOW.gbapal"
	.global System_BasicColorPalette
System_BasicColorPalette:
	.incbin "SYSTEM/MEMORY/BASIC.gbapal"
