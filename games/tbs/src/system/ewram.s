	.section .sym_ewram,"aw",%nobits
	.global BupBuffer
BupBuffer:
	.space 64
	.global GameFlagBytes
GameFlagBytes:
	.space 512
	.global MainRoutineStateWork
MainRoutineStateWork:
	.space 320
	.global RareItemStock
RareItemStock:
	.space 252
	.global OverflowItem
OverflowItem:
	.space 14
	.global PlayerActorId
PlayerActorId:
	.space 90
	.global BattleArenaSaveStamp
BattleArenaSaveStamp:
	.space 28
	.global CharacterUnits
CharacterUnits:
	.space 2816
	.global SceneLocalState
SceneLocalState:
	.space 120
	.global InventorySnapshotWork
InventorySnapshotWork:
	.space 140
	@ Slot ids, actor copies and their sprite state: ACTOR_SNAPSHOT.
	.global ActorSnapshotWork
ActorSnapshotWork:
	.space 3836
	.global SleepActive
SleepActive:
	.space 4
	.global BupSlot
BupSlot:
	.space 4
	.global SerialSendSize
SerialSendSize:
	.space 4
	.global TitleBattleOptionEnabled
TitleBattleOptionEnabled:
	.space 4
	.global TitleSendOptionEnabled
TitleSendOptionEnabled:
	.space 16
	.global SerialPeerPayloads
SerialPeerPayloads:
	.space 96
	.global SerialSendSource
SerialSendSource:
	.space 16
	.global IoWriteQueueWork
IoWriteQueueWork:
	.space 400
	.global SerialTransfer
SerialTransfer:
	.space 24
	.global SerialReceivedSize
SerialReceivedSize:
	.space 8
	.global SerialStateWork
SerialStateWork:
	.space 352
	.global LinkExchangeState
LinkExchangeState:
	.space 4
	.global SerialBlockNumber
SerialBlockNumber:
	.space 4
	.global BattleRandomSeed
BattleRandomSeed:
	.space 4
	.global SerialReceiveDestination
SerialReceiveDestination:
	.space 4
	.global SavedStack
SavedStack:
	.space 3152
	.global BgmRestoreDelay
BgmRestoreDelay:
	.space 4
	.global Sound_UnreadByte
Sound_UnreadByte:
	.space 4
	.global BgmVolume
BgmVolume:
	.space 4
	.global BgmTempoPitchStep
BgmTempoPitchStep:
	.space 4
	.global BgmVolumeStep
BgmVolumeStep:
	.space 4
	.global BgmFadeRequested
BgmFadeRequested:
	.space 12
	.global MusicPlayerLastSound
MusicPlayerLastSound:
	.space 16
	.global BgmTempoPitchTarget
BgmTempoPitchTarget:
	.space 4
	.global BgmVolumeTarget
BgmVolumeTarget:
	.space 4
	.global BgmTempoPitch
BgmTempoPitch:
	.space 4
	.global CurrentBgm
CurrentBgm:
	.space 4
	.global SoundCommandMaskWork
SoundCommandMaskWork:
	.space 16
	.global SoundInfoWork
SoundInfoWork:
	.space 4016
	.global MPlayJumpTable
MPlayJumpTable:
	.space 144
	.global CgbChans
CgbChans:
	.space 256
	.global MPlayInfo_SE5
MPlayInfo_SE5:
	.space 64
	.global MPlayInfo_SE6
MPlayInfo_SE6:
	.space 64
	.global MPlayInfo_Jingle
MPlayInfo_Jingle:
	.space 64
	.global MPlayInfo_SE7
MPlayInfo_SE7:
	.space 64
	.global MPlayInfo_BGM
MPlayInfo_BGM:
	.space 64
	.global MPlayInfo_MenuSE
MPlayInfo_MenuSE:
	.space 64
	.global MPlayInfo_SE4
MPlayInfo_SE4:
	.space 64
	.global MPlayMemAccArea
MPlayMemAccArea:
	.space 16
	.global MPlayInfo_ActionSE
MPlayInfo_ActionSE:
	.space 64
	.global MPlayTrack_Jingle
MPlayTrack_Jingle:
	.space 320
	.global MPlayTrack_BGM
MPlayTrack_BGM:
	.space 800
	.global MPlayTrack_MenuSE
MPlayTrack_MenuSE:
	.space 160
	.global MPlayTrack_ActionSE
MPlayTrack_ActionSE:
	.space 160
	.global MPlayTrack_SE4
MPlayTrack_SE4:
	.space 160
	.global MPlayTrack_SE5
MPlayTrack_SE5:
	.space 160
	.global MPlayTrack_SE6
MPlayTrack_SE6:
	.space 160
	.global MPlayTrack_SE7
MPlayTrack_SE7:
	.space 160
	.space 64
	.global WaitForFlashWriteWork
	.global CreditsScrollPosition
@ The credits borrow the four flash metatiles while flash is idle, each under a
@ name and type of their own.
WaitForFlashWriteWork:
CreditsScrollPosition:
	.space 4
	.global ProgramFlashSectorWork
	.global CreditsEndOfText
ProgramFlashSectorWork:
CreditsEndOfText:
	.space 4
	.global FlashWork
	.global CreditsLastLinePosition
FlashWork:
CreditsLastLinePosition:
	.space 4
	.global FlashNumRemainingBytes
	.global CreditsScrollObjects
FlashNumRemainingBytes:
CreditsScrollObjects:
	.space 4
	.global EraseFlashChipWork
EraseFlashChipWork:
	.space 4
	.global EraseFlashSectorWork
EraseFlashSectorWork:
	.space 4
	.global FlashMaxTime
FlashMaxTime:
	.space 4
	.global FlashReadRoutine
FlashReadRoutine:
	.space 4
	.global FlashTimerNum
FlashTimerNum:
	.space 2
	.global FlashTimerCount
FlashTimerCount:
	.space 2
	.global FlashTimeoutFlag
FlashTimeoutFlag:
	.space 4
	.global FlashTimerReg
FlashTimerReg:
	.space 4
	.global FlashSavedIme
FlashSavedIme:
	.space 13268
	.global OverlayArea
OverlayArea:
	.space 32768
@ A layout's 64 KB of metatiles (fdTileConvertMapGrid) start here and run on
@ through ActorSpriteSlots and DecodeBuffer, so neither holds anything else
@ while a layout is loaded. Only code that loads the metatiles again afterwards
@ writes there: a battle; the Battle Arena, which leaves the partner's party in
@ ActorSpriteSlots for the link battle; the world map's still image, which
@ saves BG1's tiles to DecodeBuffer; and the debug sprite viewer. The tileset
@ animation reads only past DecodeBuffer, from MapGridAnimTiles.
	.global MapGridMetatileBuffer
	.type MapGridMetatileBuffer, %object
	.size MapGridMetatileBuffer, 49152
MapGridMetatileBuffer:
	.space 32768
	.global ActorSpriteSlots
ActorSpriteSlots:
	.space 16384
	.global DecodeBuffer
	.type DecodeBuffer, %object
	.size DecodeBuffer, 16384
DecodeBuffer:
	.space 16384
	.global MetatileTiles
	.type MetatileTiles, %object
	.size MetatileTiles, 32768
MetatileTiles:
	.space 32768
	.global MapGridAnimTiles
	.type MapGridAnimTiles, %object
	.size MapGridAnimTiles, 16384
MapGridAnimTiles:
	.space 16384
	.global MapGridShapes
	.type MapGridShapes, %object
	.size MapGridShapes, 16384
MapGridShapes:
	.space 16384
	.global EwramHeap
EwramHeap:
	.space 32768
	.global BgTileBuffer
BgTileBuffer:
