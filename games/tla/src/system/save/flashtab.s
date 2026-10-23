	.section .rodata
	.align 2
	.global Flash_Version
Flash_Version:
	.asciz "FLASH_V123"
	.align 2
	.global gFlash_Chips
gFlash_Chips:
	.4byte Flash_ChipSst, gFlash_ChipAtmel, Flash_ChipMacronix, Flash_ChipPanasonic, gFlash_ChipDefault
