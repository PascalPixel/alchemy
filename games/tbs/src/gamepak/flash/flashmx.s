	.include "games/tbs/src/gamepak/flash/flashset.s"
	.section .rodata
	.align 2
	.global Flash_MaxTimeMacronix
Flash_MaxTimeMacronix:
	flash_max_time 10, 0xffbd, 0xc2
	flash_max_time 10, 0xffbd, 0xc2
	flash_max_time 2000, 0xffbd, 0xc2
	flash_max_time 2000, 0xffbd, 0xc2
	.global Flash_MaxTimePanasonic
Flash_MaxTimePanasonic:
	flash_max_time 10, 0xffbd, 0xc2
	flash_max_time 10, 0xffbd, 0xc2
	flash_max_time 500, 0xffbd, 0xc2
	flash_max_time 500, 0xffbd, 0xc2
	.global Flash_ChipMacronix
Flash_ChipMacronix:
	flash_setup ProgramFlashSector, EraseFlashChip, EraseFlashSector, WaitForFlashWrite, Flash_MaxTimeMacronix, 0x10000, 0x1000, 12, 16, 0, 3, 1, 0x1cc2
	.global Flash_ChipPanasonic
Flash_ChipPanasonic:
	flash_setup ProgramFlashSector, EraseFlashChip, EraseFlashSector, WaitForFlashWrite, Flash_MaxTimePanasonic, 0x10000, 0x1000, 12, 16, 0, 0, 2, 0x1b32
