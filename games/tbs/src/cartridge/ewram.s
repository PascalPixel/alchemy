	.section .sym_ewram,"aw",%nobits
	.global gSaveBuffer
gSaveBuffer:
	.space 0x00000040
	.global gGameFlagBytes
gGameFlagBytes:
	.space 0x00000200
	.global gAdventure
gAdventure:
	.space 0x00000140
	.global gRareItemStock
gRareItemStock:
	.space 0x000000fc
	.global gOverflowItem
gOverflowItem:
	.space 0x0000000e
	.global gPlayerActorId
gPlayerActorId:
	.space 0x0000005a
	.global gBattleArenaRecord
gBattleArenaRecord:
	.space 0x00000b1c
	.global gSceneLocalState
gSceneLocalState:
	.space 0x00000078
	.global gInventorySnapshot
gInventorySnapshot:
	.space 0x0000008c
	@ Slot ids, actor copies and their sprite state: ActorSnapshot.
	.global gActorSnapshot
gActorSnapshot:
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
	.global gTitleBattleOptionEnabled
gTitleBattleOptionEnabled:
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
	.global gSerialBlockNumber
gSerialBlockNumber:
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
	.global gBgmRestoreDelay
gBgmRestoreDelay:
	.space 0x00000004
	.global gSound_UnreadByte
gSound_UnreadByte:
	.space 0x00000004
	.global gBgmVolume
gBgmVolume:
	.space 0x00000004
	.global gBgmTempoPitchStep
gBgmTempoPitchStep:
	.space 0x00000004
	.global gBgmVolumeStep
gBgmVolumeStep:
	.space 0x00000004
	.global gBgmFadeRequested
gBgmFadeRequested:
	.space 0x0000000c
	.global gMusicPlayerLastSound
gMusicPlayerLastSound:
	.space 0x00000010
	.global gBgmTempoPitchTarget
gBgmTempoPitchTarget:
	.space 0x00000004
	.global gBgmVolumeTarget
gBgmVolumeTarget:
	.space 0x00000004
	.global gBgmTempoPitch
gBgmTempoPitch:
	.space 0x00000004
	.global gCurrentBgm
gCurrentBgm:
	.space 0x00000004
	.global gSoundCommandMask
gSoundCommandMask:
	.space 0x00000010
	.global gSoundInfo
gSoundInfo:
	.space 0x00000fb0
	.global gMPlayJumpTable
gMPlayJumpTable:
	.space 0x00000090
	.global gCgbChans
gCgbChans:
	.space 0x00000100
	.global gMPlayInfo_SE5
gMPlayInfo_SE5:
	.space 0x00000040
	.global gMPlayInfo_SE6
gMPlayInfo_SE6:
	.space 0x00000040
	.global gMPlayInfo_Fanfare
gMPlayInfo_Fanfare:
	.space 0x00000040
	.global gMPlayInfo_SE7
gMPlayInfo_SE7:
	.space 0x00000040
	.global gMPlayInfo_BGM
gMPlayInfo_BGM:
	.space 0x00000040
	.global gMPlayInfo_MenuSE
gMPlayInfo_MenuSE:
	.space 0x00000040
	.global gMPlayInfo_SE4
gMPlayInfo_SE4:
	.space 0x00000040
	.global gMPlayMemAccArea
gMPlayMemAccArea:
	.space 0x00000010
	.global gMPlayInfo_ActionSE
gMPlayInfo_ActionSE:
	.space 0x00000040
	.global gMPlayTrack_Fanfare
gMPlayTrack_Fanfare:
	.space 0x00000140
	.global gMPlayTrack_BGM
gMPlayTrack_BGM:
	.space 0x00000320
	.global gMPlayTrack_MenuSE
gMPlayTrack_MenuSE:
	.space 0x000000a0
	.global gMPlayTrack_ActionSE
gMPlayTrack_ActionSE:
	.space 0x000000a0
	.global gMPlayTrack_SE4
gMPlayTrack_SE4:
	.space 0x000000a0
	.global gMPlayTrack_SE5
gMPlayTrack_SE5:
	.space 0x000000a0
	.global gMPlayTrack_SE6
gMPlayTrack_SE6:
	.space 0x000000a0
	.global gMPlayTrack_SE7
gMPlayTrack_SE7:
	.space 0x000000a0
	.space 0x00000040
	.global gWaitForFlashWrite
gWaitForFlashWrite:
	.space 0x00000004
	.global gProgramFlashSector
gProgramFlashSector:
	.space 0x00000004
	.global gFlash
gFlash:
	.space 0x00000004
	.global gFlashNumRemainingBytes
gFlashNumRemainingBytes:
	.space 0x00000004
	.global gEraseFlashChip
gEraseFlashChip:
	.space 0x00000004
	.global gEraseFlashSector
gEraseFlashSector:
	.space 0x00000004
	.global gFlashMaxTime
gFlashMaxTime:
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
@ A layout's 64 KB of cells (Tile_ConvertMapGrid) start here and run on
@ through gActorSpriteSlots and gDecodeBuffer, so neither holds anything else
@ while a layout is loaded. Only code that loads the cells again afterwards
@ writes there: a battle; the Battle Arena, which leaves the partner's party in
@ gActorSpriteSlots for the link battle; the world map's still picture, which
@ saves BG1's tiles to gDecodeBuffer; and the debug sprite viewer. The tileset
@ animation reads only past gDecodeBuffer, from gMapGridAnimTiles.
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
	.global gCellPictures
	.type gCellPictures, %object
	.size gCellPictures, 0x8000
gCellPictures:
	.space 0x00008000
	.global gMapGridAnimTiles
	.type gMapGridAnimTiles, %object
	.size gMapGridAnimTiles, 0x4000
gMapGridAnimTiles:
	.space 0x00004000
	.global gMapGridShapes
	.type gMapGridShapes, %object
	.size gMapGridShapes, 0x4000
gMapGridShapes:
	.space 0x00004000
	.global gEwramHeap
gEwramHeap:
	.space 0x00008000
	.global gBgTileBuffer
gBgTileBuffer:
