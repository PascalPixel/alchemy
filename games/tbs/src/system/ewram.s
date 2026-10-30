	.section .sym_ewram,"aw",%nobits
	.global gSaveBuffer
gSaveBuffer:
	.space 64
	.global gGameFlagBytes
gGameFlagBytes:
	.space 512
	.global gMainRoutineWork
gMainRoutineWork:
	.space 320
	.global gRareItemStock
gRareItemStock:
	.space 252
	.global gOverflowItem
gOverflowItem:
	.space 14
	.global gPlayerActorId
gPlayerActorId:
	.space 90
	.global gBattleArenaRecord
gBattleArenaRecord:
	.space 2844
	.global gSceneLocalState
gSceneLocalState:
	.space 120
	.global gInventorySnapshot
gInventorySnapshot:
	.space 140
	@ Slot ids, actor copies and their sprite state: ActorSnapshot.
	.global gActorSnapshot
gActorSnapshot:
	.space 3836
	.global gSleepActive
gSleepActive:
	.space 4
	.global gSaveSlot
gSaveSlot:
	.space 4
	.global gSerialSendSize
gSerialSendSize:
	.space 4
	.global gTitleBattleOptionEnabled
gTitleBattleOptionEnabled:
	.space 4
	.global gTitleSendOptionEnabled
gTitleSendOptionEnabled:
	.space 16
	.global gSerialPeerPayloads
gSerialPeerPayloads:
	.space 4
	.global gLinkPeerSignatures
gLinkPeerSignatures:
	.space 92
	.global gSerialSendSource
gSerialSendSource:
	.space 16
	.global gIoWriteQueue
gIoWriteQueue:
	.space 400
	.global gSerialTransfer
gSerialTransfer:
	.space 24
	.global gSerialReceivedSize
gSerialReceivedSize:
	.space 8
	.global gSerialWork
gSerialWork:
	.space 352
	.global gLinkExchangeState
gLinkExchangeState:
	.space 4
	.global gSerialBlockNumber
gSerialBlockNumber:
	.space 4
	.global gBattleRandomSeed
gBattleRandomSeed:
	.space 4
	.global gSerialReceiveDestination
gSerialReceiveDestination:
	.space 4
	.global gSavedStack
gSavedStack:
	.space 3152
	.global gBgmRestoreDelay
gBgmRestoreDelay:
	.space 4
	.global gSound_UnreadByte
gSound_UnreadByte:
	.space 4
	.global gBgmVolume
gBgmVolume:
	.space 4
	.global gBgmTempoPitchStep
gBgmTempoPitchStep:
	.space 4
	.global gBgmVolumeStep
gBgmVolumeStep:
	.space 4
	.global gBgmFadeRequested
gBgmFadeRequested:
	.space 12
	.global gMusicPlayerLastSound
gMusicPlayerLastSound:
	.space 16
	.global gBgmTempoPitchTarget
gBgmTempoPitchTarget:
	.space 4
	.global gBgmVolumeTarget
gBgmVolumeTarget:
	.space 4
	.global gBgmTempoPitch
gBgmTempoPitch:
	.space 4
	.global gCurrentBgm
gCurrentBgm:
	.space 4
	.global gSoundCommandMask
gSoundCommandMask:
	.space 16
	.global gSoundInfo
gSoundInfo:
	.space 4016
	.global gMPlayJumpTable
gMPlayJumpTable:
	.space 144
	.global gCgbChans
gCgbChans:
	.space 256
	.global gMPlayInfo_SE5
gMPlayInfo_SE5:
	.space 64
	.global gMPlayInfo_SE6
gMPlayInfo_SE6:
	.space 64
	.global gMPlayInfo_Jingle
gMPlayInfo_Jingle:
	.space 64
	.global gMPlayInfo_SE7
gMPlayInfo_SE7:
	.space 64
	.global gMPlayInfo_BGM
gMPlayInfo_BGM:
	.space 64
	.global gMPlayInfo_MenuSE
gMPlayInfo_MenuSE:
	.space 64
	.global gMPlayInfo_SE4
gMPlayInfo_SE4:
	.space 64
	.global gMPlayMemAccArea
gMPlayMemAccArea:
	.space 16
	.global gMPlayInfo_ActionSE
gMPlayInfo_ActionSE:
	.space 64
	.global gMPlayTrack_Jingle
gMPlayTrack_Jingle:
	.space 320
	.global gMPlayTrack_BGM
gMPlayTrack_BGM:
	.space 800
	.global gMPlayTrack_MenuSE
gMPlayTrack_MenuSE:
	.space 160
	.global gMPlayTrack_ActionSE
gMPlayTrack_ActionSE:
	.space 160
	.global gMPlayTrack_SE4
gMPlayTrack_SE4:
	.space 160
	.global gMPlayTrack_SE5
gMPlayTrack_SE5:
	.space 160
	.global gMPlayTrack_SE6
gMPlayTrack_SE6:
	.space 160
	.global gMPlayTrack_SE7
gMPlayTrack_SE7:
	.space 160
	.space 64
	.global gWaitForFlashWrite
gWaitForFlashWrite:
	.space 4
	.global gProgramFlashSector
gProgramFlashSector:
	.space 4
	.global gFlash
gFlash:
	.space 4
	.global gFlashNumRemainingBytes
gFlashNumRemainingBytes:
	.space 4
	.global gEraseFlashChip
gEraseFlashChip:
	.space 4
	.global gEraseFlashSector
gEraseFlashSector:
	.space 4
	.global gFlashMaxTime
gFlashMaxTime:
	.space 4
	.global gFlashReadRoutine
gFlashReadRoutine:
	.space 4
	.global gFlashTimerNum
gFlashTimerNum:
	.space 2
	.global gFlashTimerCount
gFlashTimerCount:
	.space 2
	.global gFlashTimeoutFlag
gFlashTimeoutFlag:
	.space 4
	.global gFlashTimerReg
gFlashTimerReg:
	.space 4
	.global gFlashSavedIme
gFlashSavedIme:
	.space 13268
	.global gOverlayArea
gOverlayArea:
	.space 32768
@ A layout's 64 KB of cells (Tile_ConvertMapGrid) start here and run on
@ through gActorSpriteSlots and gDecodeBuffer, so neither holds anything else
@ while a layout is loaded. Only code that loads the cells again afterwards
@ writes there: a battle; the Battle Arena, which leaves the partner's party in
@ gActorSpriteSlots for the link battle; the world map's still picture, which
@ saves BG1's tiles to gDecodeBuffer; and the debug sprite viewer. The tileset
@ animation reads only past gDecodeBuffer, from gMapGridAnimTiles.
	.global gMapGridCellBuffer
	.type gMapGridCellBuffer, %object
	.size gMapGridCellBuffer, 49152
gMapGridCellBuffer:
	.space 32768
	.global gActorSpriteSlots
gActorSpriteSlots:
	.space 16384
	.global gDecodeBuffer
	.type gDecodeBuffer, %object
	.size gDecodeBuffer, 16384
gDecodeBuffer:
	.space 16384
	.global gCellPictures
	.type gCellPictures, %object
	.size gCellPictures, 32768
gCellPictures:
	.space 32768
	.global gMapGridAnimTiles
	.type gMapGridAnimTiles, %object
	.size gMapGridAnimTiles, 16384
gMapGridAnimTiles:
	.space 16384
	.global gMapGridShapes
	.type gMapGridShapes, %object
	.size gMapGridShapes, 16384
gMapGridShapes:
	.space 16384
	.global gEwramHeap
gEwramHeap:
	.space 32768
	.global gBgTileBuffer
gBgTileBuffer:
