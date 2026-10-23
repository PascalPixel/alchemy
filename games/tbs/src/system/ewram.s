	.section .sym_ewram,"aw",%nobits
	.global gSaveBuffer
gSaveBuffer:
	.space 0x00000040
	.global gGameFlagBytes
gGameFlagBytes:
	.space 0x00000200
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
	.global gMapLocalState
gMapLocalState:
	.space 0x00000078
	.global gInventorySnapshot
gInventorySnapshot:
	.space 0x0000008c
	@ Slot ids, object event copies and their sprite state: ObjectTableSnapshot.
	.global gObjectSnapshots
gObjectSnapshots:
	.space 0x00000efc
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
	.global gSerialWork
gSerialWork:
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
	.global gSerialReceiveDestination
gSerialReceiveDestination:
	.space 0x00000004
	.global gSavedStack
gSavedStack:
	.space 0x00000c50
	.global gMusicRestoreDelay
gMusicRestoreDelay:
	.space 0x00000004
	.global gEwram_Data1
gEwram_Data1:
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
	.global gMusicFadeRequested
gMusicFadeRequested:
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
	.global gCurrentBgm
gCurrentBgm:
	.space 0x00000004
	.global gAudio_CommandMask
gAudio_CommandMask:
	.space 0x00000010
	.global gSound_Work
gSound_Work:
	.space 0x00000fb0
	.global gSound_CommandTable
gSound_CommandTable:
	.space 0x00000090
	.global gSound_CgbNotes
gSound_CgbNotes:
	.space 0x00000100
	.global gSound_Player5
gSound_Player5:
	.space 0x00000040
	.global Sound_Player6
Sound_Player6:
	.space 0x00000040
	.global gMusicPlayerFanfare
gMusicPlayerFanfare:
	.space 0x00000040
	.global Sound_Player7
Sound_Player7:
	.space 0x00000040
	.global gMusicPlayerBgm
gMusicPlayerBgm:
	.space 0x00000040
	.global Sound_Player2
Sound_Player2:
	.space 0x00000040
	.global Sound_Player4
Sound_Player4:
	.space 0x00000040
	.global gSound_WorkBytes
gSound_WorkBytes:
	.space 0x00000010
	.global gSound_Player3
gSound_Player3:
	.space 0x00000040
	.global Sound_Tracks0
Sound_Tracks0:
	.space 0x00000140
	.global Sound_Tracks1
Sound_Tracks1:
	.space 0x00000320
	.global Sound_Tracks2
Sound_Tracks2:
	.space 0x000000a0
	.global Sound_Tracks3
Sound_Tracks3:
	.space 0x000000a0
	.global Sound_Tracks4
Sound_Tracks4:
	.space 0x000000a0
	.global Sound_Tracks5
Sound_Tracks5:
	.space 0x000000a0
	.global Sound_Tracks6
Sound_Tracks6:
	.space 0x000000a0
	.global Sound_Tracks7
Sound_Tracks7:
	.space 0x000000a0
	.space 0x00000040
	.global gFlash_Handler3
gFlash_Handler3:
	.space 0x00000004
	.global gFlash_Handler0
gFlash_Handler0:
	.space 0x00000004
	.global gFlash
gFlash:
	.space 0x00000004
	.global gFlashNumRemainingBytes
gFlashNumRemainingBytes:
	.space 0x00000004
	.global gFlash_Handler1
gFlash_Handler1:
	.space 0x00000004
	.global gEraseFlashSector
gEraseFlashSector:
	.space 0x00000004
	.global gFlash_Handler4
gFlash_Handler4:
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
	.global gMapGridCellBuffer
	.type gMapGridCellBuffer, %object
	.size gMapGridCellBuffer, 0xc000
gMapGridCellBuffer:
	.space 0x00008000
	.global gActorSpriteSlots
gActorSpriteSlots:
	.space 0x00004000
	.global gDecodeBuffer
	.type gDecodeBuffer, %object
	.size gDecodeBuffer, 0x4000
gDecodeBuffer:
	.space 0x00004000
	.global gMetatiles
	.type gMetatiles, %object
	.size gMetatiles, 0x8000
gMetatiles:
	.space 0x00008000
	.global gMapGridLayerData
	.type gMapGridLayerData, %object
	.size gMapGridLayerData, 0x4000
gMapGridLayerData:
	.space 0x00004000
	.global gMapGridCollision
	.type gMapGridCollision, %object
	.size gMapGridCollision, 0x4000
gMapGridCollision:
	.space 0x00004000
	.global gEwramHeap
gEwramHeap:
	.space 0x00008000
	.global gBgTileBuffer
gBgTileBuffer:
