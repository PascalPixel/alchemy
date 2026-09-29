@ Ui_Icons: 80 icon slots of 32x32, 57 of them drawn, each with its own
@ 16-colour palette: a halfword offset per slot (0 when empty), then each
@ icon's palette and its pixels in the 4-bit icon coder.
	.section .rodata
	.global Ui_Icons
Ui_Icons:
	.incbin "GRAPHICS/UI/ICONS/ICONS.icons4"
