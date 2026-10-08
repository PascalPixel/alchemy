	.section .rodata
	.align 2
	.global Flash_Version
Flash_Version:
	.asciz "FLASH_V123"
	.align 2
	.global Flash_Chips
Flash_Chips:
	.4byte Flash_ChipSst, Flash_ChipAtmel, Flash_ChipMacronix, Flash_ChipPanasonic, Flash_ChipUnknown
