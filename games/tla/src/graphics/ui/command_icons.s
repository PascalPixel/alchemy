	.section .rodata
	.global Ui_CommandIcons
Ui_CommandIcons:
	.incbin "games/tla/src/graphics/ui/command_icons.icons"
	.byte 0x00, 0x00	@ PACKER: left in the buffer by Ui_Icons+0x3c86
