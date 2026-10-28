	.include "games/tla/src/gamepak/flash/flashset.s"
	.section .rodata
	.align 2
	.global Flash_MaxTimeAtmel
Flash_MaxTimeAtmel:
	flash_max_time 10, 0xffbd, 0xc2
	flash_max_time 40, 0xffbd, 0xc2
	flash_max_time 0, 0x0000, 0x00
	flash_max_time 40, 0xffbd, 0xc2
	.global gFlash_ChipAtmel
gFlash_ChipAtmel:
	flash_setup ProgramAtmelFlashBlock, EraseAtmelFlashChip, EraseAtmelFlashBlock, WaitForFlashWrite, Flash_MaxTimeAtmel, 0x10000, 0x1000, 12, 16, 0, 3, 3, 0x3d1f
	.global gFlash_ChipAtmelLayout
gFlash_ChipAtmelLayout:
	flash_setup ProgramAtmelFlashSector, EraseAtmelFlashChip, EraseAtmelFlashSector, WaitForFlashWrite, Flash_MaxTimeAtmel, 0x10000, 0x80, 7, 512, 0, 3, 3, 0x3d1f
