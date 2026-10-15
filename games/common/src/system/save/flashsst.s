	.include "games/common/src/system/save/flashset.s"
	.section .rodata
	.align 2
	.global Flash_MaxTimeSst
Flash_MaxTimeSst:
	flash_max_time 10, 0xffbd, 0xc2
	flash_max_time 10, 0xffbd, 0xc2
	flash_max_time 40, 0xffbd, 0xc2
	flash_max_time 200, 0xffbd, 0xc2
	.global gFlash_ChipUnknown
gFlash_ChipUnknown:
	flash_setup ProgramFlashSectorWithEraseRetry, EraseFlashChip, EraseFlashSector, WaitForFlashWrite, Flash_MaxTimeSst, 0x10000, 0x1000, 12, 16, 0, 3, 3, 0x0000
	.global Flash_ChipSst
Flash_ChipSst:
	flash_setup ProgramFlashSectorWithEraseRetry, EraseFlashChip, EraseFlashSector, WaitForFlashWrite, Flash_MaxTimeSst, 0x10000, 0x1000, 12, 16, 0, 1, 2, 0xd4bf
