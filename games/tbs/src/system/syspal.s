	.section .rodata
	.global Ui_WindowPalette
Ui_WindowPalette:
	.incbin "common/system/memory/window.gbapal"
	.global System_BasicColorPalette
System_BasicColorPalette:
	.incbin "common/system/memory/basic.gbapal"
