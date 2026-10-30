	.include "games/tbs/src/gamepak/flash/flashset.s"
	.section .rodata
	.align 2
	.global Flash_MaxTimeSst
Flash_MaxTimeSst:
	flash_max_time 10, 0xffbd, 0xc2
	flash_max_time 10, 0xffbd, 0xc2
	flash_max_time 40, 0xffbd, 0xc2
	flash_max_time 200, 0xffbd, 0xc2
	.global gFlash_ChipDefault
gFlash_ChipDefault:
	flash_setup ProgramFlashSectorWithEraseRetry, EraseFlashChip, EraseFlashSector, WaitForFlashWrite, Flash_MaxTimeSst, 65536, 4096, 12, 16, 0, 3, 3, 0
	.global Flash_ChipSst
Flash_ChipSst:
	flash_setup ProgramFlashSectorWithEraseRetry, EraseFlashChip, EraseFlashSector, WaitForFlashWrite, Flash_MaxTimeSst, 65536, 4096, 12, 16, 0, 1, 2, 0xd4bf
