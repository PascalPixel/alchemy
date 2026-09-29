@ EWRAM variables not yet defined in C, in address order, as pret's
@ sym files list them. Each .space runs to the next variable.
	.section .sym_ewram,"aw",%nobits
	.global gSaveBuffer
gSaveBuffer:
	.space 0x00000040
	.global GameFlagBytes
GameFlagBytes:
	.space 0x00000200
	.global Data_02000240
Data_02000240:
	.global gCell
gCell:
	.global gGameState
gGameState:
	.space 0x00000140
	.global gItemCounters
gItemCounters:
	.space 0x000000fc
	.global gOverflowItem
gOverflowItem:
	.space 0x0000000e
	.global gPlayerObjectId
gPlayerObjectId:
	.space 0x0000005a
	.global gSaveStamp
gSaveStamp:
	.space 0x00000b1c
	.global gSceneState
gSceneState:
	.space 0x00000078
	.global gInventorySnapshot
gInventorySnapshot:
	.space 0x000000ac
	.global Data_02001124
Data_02001124:
	.space 0x00000edc
	.global gSleepActive
gSleepActive:
	.space 0x00000004
	.global gSaveSlot
gSaveSlot:
	.space 0x00000004
	.global gSerialSendSize
gSerialSendSize:
	.space 0x00000004
	.global gTitleExtraOptionEnabled
gTitleExtraOptionEnabled:
	.space 0x00000004
	.global gTitleSendOptionEnabled
gTitleSendOptionEnabled:
	.space 0x00000010
	.global gSerialPeerPayloads
gSerialPeerPayloads:
	.space 0x00000004
	.global gLinkPeerSignatures
gLinkPeerSignatures:
	.space 0x0000005c
	.global gSerialSendSource
gSerialSendSource:
	.space 0x00000010
	.global gIoWriteQueue
gIoWriteQueue:
	.space 0x00000190
	.global gSerialTransfer
gSerialTransfer:
	.space 0x00000018
	.global gSerialReceivedSize
gSerialReceivedSize:
	.space 0x00000008
	.global gSerialRuntime
gSerialRuntime:
	.space 0x00000160
	.global gLinkExchangeState
gLinkExchangeState:
	.space 0x00000004
	.global gSerialBlockSequence
gSerialBlockSequence:
	.space 0x00000004
	.global gBattleRandomSeed
gBattleRandomSeed:
	.space 0x00000004
	.global gSerialReceiveDest
gSerialReceiveDest:
	.space 0x00000004
	.global gSavedStack
gSavedStack:
	.space 0x00000c50
	.global RomBytes_02003000
RomBytes_02003000:
	.global gMusicRestoreDelay
gMusicRestoreDelay:
	.space 0x00000004
	.global RomBytes_02003004
RomBytes_02003004:
	.space 0x00000004
	.global gMusicVolume
gMusicVolume:
	.space 0x00000004
	.global gMusicPitchStep
gMusicPitchStep:
	.space 0x00000004
	.global gMusicVolumeStep
gMusicVolumeStep:
	.space 0x00000004
	.global Data_02003014
Data_02003014:
	.space 0x0000000c
	.global gMusicPlayerVolumes
gMusicPlayerVolumes:
	.space 0x00000010
	.global gMusicPitchTarget
gMusicPitchTarget:
	.space 0x00000004
	.global gMusicVolumeTarget
gMusicVolumeTarget:
	.space 0x00000004
	.global gMusicPitch
gMusicPitch:
	.space 0x00000004
	.global gAudioSecondaryState
gAudioSecondaryState:
	.space 0x00000004
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
	.space 0x00000084
	.global Data_02004088
Data_02004088:
	.space 0x00000004
	.global Data_0200408c
Data_0200408c:
	.space 0x00000004
	.global Sound_CgbNotes
Sound_CgbNotes:
	.space 0x00000180
	.global gMusicPlayerFanfare
gMusicPlayerFanfare:
	.space 0x00000080
	.global gMusicPlayerBgm
gMusicPlayerBgm:
	.space 0x000000c0
	.global Sound_WorkBytes
Sound_WorkBytes:
	.space 0x00000010
	.global Data_02004360
Data_02004360:
	.space 0x000008a0
	.global Data_02004c00
Data_02004c00:
	.global Flash_Handler3
Flash_Handler3:
	.space 0x00000004
	.global Data_02004c04
Data_02004c04:
	.global Flash_Handler0
Flash_Handler0:
	.space 0x00000004
	.global Data_02004c08
Data_02004c08:
	.global Flash_Layout
Flash_Layout:
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
	.global gEraseFlashSector
gEraseFlashSector:
	.space 0x00000004
	.global Data_02004c18
Data_02004c18:
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
	.space 0x000033d4
	.global gOverlayArea
gOverlayArea:
	.space 0x00008000
	.global gMapCellBuffer
gMapCellBuffer:
	.space 0x0000c000
	.global gDecodeBuffer
gDecodeBuffer:
	.space 0x00004000
	.global gMapBlocks
gMapBlocks:
	.space 0x00008000
	.global gMapLayerData
gMapLayerData:
	.space 0x00004000
	.global gMapCollision
gMapCollision:
	.space 0x00004000
	.global gEwramHeap
gEwramHeap:
	.space 0x00008000
	.global gBgTileBuffer
gBgTileBuffer:
