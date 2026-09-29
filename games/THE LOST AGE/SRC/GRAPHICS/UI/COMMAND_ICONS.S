@ Ui_CommandIcons: 51 command icons of 32x32, drawn with the system palette:
@ a halfword offset per icon, then each as 8-bit tiles in palette LZ.
	.section .rodata
	.global Ui_CommandIcons
Ui_CommandIcons:
	.incbin "GRAPHICS/UI/COMMAND_ICONS.icons"
