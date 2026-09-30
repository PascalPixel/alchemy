@ EWRAM variables not yet defined in C, in address order, as pret's
@ sym files list them. Each .space runs to the next variable.
	.section .sym_ewram,"aw",%nobits
	.space 0x00000040
	.global GameFlagBytes
GameFlagBytes:
	.space 0x00000200
	.global gPartyState
gPartyState:
	.space 0x0000210c
	.global gSceneState
gSceneState:
	.space 0x00000cc4
	.global gOamBuckets
gOamBuckets:
	.space 0x000006d0
	.global ResourceTableEntries
ResourceTableEntries:
	.space 0x00001df0
	.global gObjAffineMatrices
gObjAffineMatrices:
	.space 0x00000330
	.global gMusicRestoreDelay
gMusicRestoreDelay:
	.space 0x00000008
	.global gMusicVolume
gMusicVolume:
	.space 0x00000030
	.global gMusicVolumeTarget
gMusicVolumeTarget:
	.space 0x00000008
	.global Audio_CommandMask
Audio_CommandMask:
	.space 0x00000010
	.global Sound_Work
Sound_Work:
	.space 0x00000fb0
	.global Sound_CommandTable
Sound_CommandTable:
	.space 0x00000004
	.global Sound_JumpCommand
Sound_JumpCommand:
	.space 0x0000008c
	.global Sound_CgbNotes
Sound_CgbNotes:
	.space 0x00000200
	.global gMusicPlayerBgm
gMusicPlayerBgm:
	.space 0x000000c0
	.global Sound_WorkBytes
Sound_WorkBytes:
	.space 0x000008b0
	.global Flash_Handler3
Flash_Handler3:
	.space 0x00000004
	.global Flash_Handler0
Flash_Handler0:
	.space 0x00000004
	.global gFlash
gFlash:
	.space 0x00000004
	.global gFlashNumRemainingBytes
gFlashNumRemainingBytes:
	.space 0x00000004
	.global Flash_Handler1
Flash_Handler1:
	.space 0x00000004
	.global Flash_Handler2
Flash_Handler2:
	.space 0x00000004
	.global Flash_Handler4
Flash_Handler4:
	.space 0x00000004
	.global gFlashReadRoutine
gFlashReadRoutine:
	.space 0x00000004
	.global gFlashTimerNum
gFlashTimerNum:
	.space 0x00000002
	.global gFlashTimerCount
gFlashTimerCount:
	.space 0x00000002
	.global gFlashTimeoutFlag
gFlashTimeoutFlag:
	.space 0x00000004
	.global gFlashTimerReg
gFlashTimerReg:
	.space 0x00000004
	.global gFlashSavedIme
gFlashSavedIme:
	.space 0x000000f6
	.global gScrollTarget
gScrollTarget:
	.space 0x00008ade
	.global gMapCellBuffer
gMapCellBuffer:
	.space 0x00010000
	.global gMapBlocks
gMapBlocks:
