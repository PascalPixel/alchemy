	.section .sym_ewram,"aw",%nobits
	.global BupBuffer
BupBuffer:
	.space 0x00000040
	.global GameFlagBytes
GameFlagBytes:
	.space 0x00000200
	.global MainRoutineStateWork
MainRoutineStateWork:
	.space 0x0000000c
	.global PartyDjinnRecovery
PartyDjinnRecovery:
	.space 0x000001d6
	.space 0x00000014
	.space 0x00000012
	.space 0x00000004
	.space 0x00000006
	.space 0x00000002
	.space 0x00000004
	.space 0x00000040
	.space 0x00000004
	.space 0x0000000e
	.space 0x00000002
	.space 0x0000000c
	.space 0x00000048
	.global MapScreenChoice
MapScreenChoice:
	.space 0x00000004
	.space 0x0000001c
	.global CharacterUnits
CharacterUnits:
	.space 0x00000ae0
	.global SceneLocalState
SceneLocalState:
	.space 0x00000024
	.global SnapshotObjects
SnapshotObjects:
	.space 0x00001068
	.global RareItemStock
RareItemStock:
	.space 0x000002c0
	.global SceneStateWork
SceneStateWork:
	.space 0x00000078
	.global InventorySnapshot
InventorySnapshot:
	.space 0x00000b5c
	.global SoundFlags
SoundFlags:
	.space 0x000000e0
	.global SleepActive
SleepActive:
	.space 0x00000010
	.global OamDepthLists
OamDepthLists:
	.space 0x00000400
	.global VramCacheBlockOwners
VramCacheBlockOwners:
	.space 0x00000200
	.global EventBuf
EventBuf:
	.space 0x000000c0
	.global BupSlot
BupSlot:
	.space 0x00000004
	.global SerialSendSize
SerialSendSize:
	.space 0x00000004
	.global TitleBattleOptionEnabled
TitleBattleOptionEnabled:
	.space 0x00000008
	.global VramCacheEntries
VramCacheEntries:
	.space 0x00000180
	.global AnyFileCleared
AnyFileCleared:
	.space 0x00000010
	.global SerialPeerPayloads
SerialPeerPayloads:
	.space 0x00000004
	.global LinkPeerSignatures
LinkPeerSignatures:
	.space 0x0000005c
	.global SerialSendSource
SerialSendSource:
	.space 0x00000010
	.global IoWriteQueueWork
IoWriteQueueWork:
	.space 0x00000190
	.global SerialTransfer
SerialTransfer:
	.space 0x00000020
	.global MixerPcmBuffer
MixerPcmBuffer:
	.space 0x00000ce8
	.global ModelTransformState
ModelTransformState:
	.space 0x00000bd8
	.global TitleUpdateOptionEnabled
TitleUpdateOptionEnabled:
	.space 0x00000004
	.global SerialReceivedSize
SerialReceivedSize:
	.space 0x0000000c
	.global SerialStateWork
SerialStateWork:
	.space 0x00000004
	.space 0x0000015c
	.global LinkExchangeState
LinkExchangeState:
	.space 0x00000004
	.global SerialBlockNumber
SerialBlockNumber:
	.space 0x00000004
	.global BattleRandomSeed
BattleRandomSeed:
	.space 0x00000008
	.global OamMatrices
OamMatrices:
	.space 0x00000100
	.global SerialReceiveDestination
SerialReceiveDestination:
	.space 0x00000230
	.global BgmRestoreDelay
BgmRestoreDelay:
	.space 0x00000004
	.global SoundUnreadByte
SoundUnreadByte:
	.space 0x00000004
	.global BgmVolume
BgmVolume:
	.space 0x00000004
	.global BgmTempoPitchStep
BgmTempoPitchStep:
	.space 0x00000004
	.global BgmVolumeStep
BgmVolumeStep:
	.space 0x00000004
	.global BgmFadeRequested
BgmFadeRequested:
	.space 0x0000000c
	.global MusicPlayerLastSound
MusicPlayerLastSound:
	.space 0x00000010
	.global CurrentBgm
CurrentBgm:
	.space 0x00000004
	.global BgmTempoPitchTarget
BgmTempoPitchTarget:
	.space 0x00000004
	.global BgmVolumeTarget
BgmVolumeTarget:
	.space 0x00000004
	.global BgmTempoPitch
BgmTempoPitch:
	.space 0x00000004
	.global SoundCommandMask
SoundCommandMask:
	.space 0x00000010
	.global SoundInfoWork
SoundInfoWork:
	.space 0x00000fb0
	.global MPlayJumpTable
MPlayJumpTable:
	.space 0x00000004
	.space 0x00000084
	.space 0x00000004
	.space 0x00000004
	.global CgbChans
CgbChans:
	.space 0x00000100
	.global MPlayInfoSE5
MPlayInfoSE5:
	.space 0x00000040
	.global MPlayInfoSE6
MPlayInfoSE6:
	.space 0x00000040
	.global MPlayInfoJingle
MPlayInfoJingle:
	.space 0x00000040
	.global MPlayInfoSE7
MPlayInfoSE7:
	.space 0x00000040
	.global MPlayInfoBGM
MPlayInfoBGM:
	.space 0x00000040
	.global MPlayInfoMenuSE
MPlayInfoMenuSE:
	.space 0x00000040
	.global MPlayInfoSE4
MPlayInfoSE4:
	.space 0x00000040
	.global MPlayMemAccArea
MPlayMemAccArea:
	.space 0x00000010
	.global MPlayInfoActionSE
MPlayInfoActionSE:
	.space 0x00000040
	.global SoundTracks0
SoundTracks0:
	.space 0x00000140
	.global SoundTracks1
SoundTracks1:
	.space 0x00000320
	.global SoundTracks2
SoundTracks2:
	.space 0x000000a0
	.global SoundTracks3
SoundTracks3:
	.space 0x000000a0
	.global SoundTracks4
SoundTracks4:
	.space 0x000000a0
	.global SoundTracks5
SoundTracks5:
	.space 0x000000a0
	.global SoundTracks6
SoundTracks6:
	.space 0x000000a0
	.global SoundTracks7
SoundTracks7:
	.space 0x000000a0
	.space 0x00000040
	.global CreditsGreyColors
CreditsGreyColors:
	.global WaitForFlashWriteWork
WaitForFlashWriteWork:
	.space 0x00000004
	.global ProgramFlashSectorWork
ProgramFlashSectorWork:
	.space 0x00000004
	.global FlashWork
FlashWork:
	.space 0x00000004
	.global FlashNumRemainingBytes
FlashNumRemainingBytes:
	.space 0x00000004
	.global EraseFlashChipWork
EraseFlashChipWork:
	.space 0x00000004
	.global EraseFlashSectorWork
EraseFlashSectorWork:
	.space 0x00000004
	.global FlashMaxTime
FlashMaxTime:
	.space 0x00000004
	.global FlashReadRoutine
FlashReadRoutine:
	.space 0x00000004
	.global FlashTimerNum
FlashTimerNum:
	.space 0x00000002
	.global FlashTimerCount
FlashTimerCount:
	.space 0x00000002
	.global FlashTimeoutFlag
FlashTimeoutFlag:
	.space 0x00000004
	.global FlashTimerReg
FlashTimerReg:
	.space 0x00000004
	.global FlashSavedIme
FlashSavedIme:
	.space 0x000000d4
	.global CreditsPendingSlides
CreditsPendingSlides:
	.space 0x00000004
	.global DisplayScrollPosition
DisplayScrollPosition:
	.space 0x00000004
	.global DisplayScrollBusy
DisplayScrollBusy:
	.space 0x00000004
	.global DisplayScrollLastPosition
DisplayScrollLastPosition:
	.space 0x00000004
	.global DisplayScrollObjects
DisplayScrollObjects:
	.space 0x00000004
	.global EndingImageBlock
EndingImageBlock:
	.space 0x00000002
	.global EndingImageFadeFrames
EndingImageFadeFrames:
	.space 0x00000002
	.global EndingImageMapScroll
EndingImageMapScroll:
	.space 0x00000004
	.global EndingImageScroll
EndingImageScroll:
	.space 0x00000004
	.global EndingImageRow
EndingImageRow:
	.space 0x00000002
	.global EndingImageTargetRow
EndingImageTargetRow:
	.space 0x00000002
	.global EndingImageSpeed
EndingImageSpeed:
	.space 0x00000004
	.global EndingImageMap
EndingImageMap:
	.space 0x00000004
	.global EndingImageStrips
EndingImageStrips:
	.space 0x00000ad4
	.global OverlayArea
OverlayArea:
	.space 0x00007f58
	.global ScriptedBattleSnapshot
ScriptedBattleSnapshot:
	.space 0x00000014
	.space 0x00000080
	.space 0x0000000c
	.space 0x00000002
	.space 0x00000006
	.global MapGridMetatileBuffer
MapGridMetatileBuffer:
	.space 0x00000001
	.space 0x00000001
	.space 0x00000016
	.space 0x00000008
	.space 0x00000020
	.space 0x0000003e
	.space 0x00000004
	.space 0x0000001e
	.space 0x00000068
	.space 0x00000038
	.space 0x00000018
	.space 0x000000a8
	.space 0x00000008
	.space 0x00000098
	.space 0x00000078
	.space 0x00000068
	.space 0x00000080
	.space 0x000000f0
	.global HauresMoundCorners
HauresMoundCorners:
	.space 0x00000088
	.global IrisTileBuffer
IrisTileBuffer:
	.space 0x00000170
	.space 0x00000118
	.space 0x0000002f
	.space 0x0000000d
	.space 0x000002ac
	.global HauresMoundStrip
HauresMoundStrip:
	.space 0x00000008
	.space 0x000000f0
	.global HauresMoundStripEnd
HauresMoundStripEnd:
	.space 0x00000076
	.space 0x00000002
	.space 0x00000002
	.space 0x00000016
	.space 0x00000110
	.space 0x00000080
	.space 0x000001a0
	.global HauresMoundShades
HauresMoundShades:
	.space 0x00000060
	.space 0x00000156
	.space 0x00000192
	.space 0x00000118
	.space 0x00000409
	.space 0x000001d7
	.space 0x00000220
	.space 0x00000018
	.space 0x00000008
	.space 0x00000300
	.space 0x00000020
	.space 0x00000028
	.space 0x00000098
	.space 0x00000400
	.space 0x000000de
	.space 0x00000062
	.global SeracMeshEnd
SeracMeshEnd:
	.space 0x0000018a
	.space 0x000000a1
	.space 0x00000095
	.space 0x00000164
	.space 0x000000f2
	.space 0x0000032a
	.space 0x000000ee
	.space 0x00000072
	.space 0x00000014
	.space 0x00000014
	.space 0x0000008c
	.global Data02012f94
Data02012f94:
	.space 0x000007f4
	.space 0x00000078
	.space 0x00000018
	.space 0x00000028
	.space 0x00000096
	.space 0x00000380
	.space 0x000001c2
	.space 0x000001e8
	.global EffectScratch
EffectScratch:
	.space 0x00000002
	.global FloraPetalTiles
FloraPetalTiles:
	.space 0x00000002
	.space 0x00000014
	.space 0x00000006
	.space 0x000001e2
	.space 0x00000008
	.space 0x00000010
	.space 0x00000048
	.global FloraSpritePalettes
FloraSpritePalettes:
	.space 0x000001a0
	.space 0x0000036b
	.space 0x00000095
	.space 0x00000100
	.space 0x00000089
	.space 0x0000004f
	.space 0x000000f8
	.space 0x00000030
	.space 0x00000156
	.space 0x0000002a
	.space 0x00000180
	.space 0x00000018
	.space 0x00000008
	.space 0x0000007d
	.space 0x00000163
	.global Data02015000
Data02015000:
	.space 0x00000018
	.space 0x00000270
	.space 0x000001d8
	.space 0x0000030b
	.space 0x00000155
	.space 0x00000012
	.space 0x000000ae
	.space 0x00000280
	.space 0x00000400
	.space 0x0000000b
	.space 0x0000000d
	.space 0x00000026
	.space 0x000001e2
	.space 0x000004c4
	.space 0x0000015a
	.space 0x000000c2
	.space 0x00000018
	.space 0x000002a8
	.space 0x00000018
	.space 0x000000e5
	.space 0x00000043
	.space 0x00000018
	.space 0x000000e8
	.space 0x00000016
	.space 0x0000012a
	.space 0x00000018
	.space 0x00000128
	.global Data02017080
Data02017080:
	.space 0x00000380
	.space 0x000002e8
	.space 0x00000918
	.global ActorSpriteSlots
ActorSpriteSlots:
	.space 0x00001600
	.global TitleIntroLighthouseTilesTail
TitleIntroLighthouseTilesTail:
	.space 0x00002a00
	.global DecodeBuffer
DecodeBuffer:
	.space 0x00004000
	.global MetatileTiles
MetatileTiles:
	.space 0x00000004
	.space 0x000001fe
	.space 0x00003dfe
	.global MapShapeGrid
MapShapeGrid:
	.space 0x00002800
	.global TitleIntroLogoTileBuffer
TitleIntroLogoTileBuffer:
	.space 0x00001800
	.global MapGridAnimationTiles
MapGridAnimationTiles:
	.space 0x00002000
	.global WorldMapStateWork
WorldMapStateWork:
	.space 0x00000004
	.space 0x0000061c
	.global MapScreenCursorMarker
MapScreenCursorMarker:
	.space 0x0000000c
	.global MapScreenCursor
MapScreenCursor:
	.space 0x00000004
	.global MapScreenScrollX
MapScreenScrollX:
	.space 0x00000004
	.global MapScreenScrollY
MapScreenScrollY:
	.space 0x00000004
	.global MapScreenNameBox
MapScreenNameBox:
	.space 0x00000002
	.space 0x00000002
	.space 0x00000002
	.space 0x00000002
	.global MapScreenZoomed
MapScreenZoomed:
	.space 0x00000002
	.global MapScreenFirstListOnly
MapScreenFirstListOnly:
	.space 0x00000002
	.global MapScreenGlideTargetX
MapScreenGlideTargetX:
	.space 0x00000004
	.global MapScreenGlideTargetY
MapScreenGlideTargetY:
	.space 0x00000004
	.global MapScreenCursorStartX
MapScreenCursorStartX:
	.space 0x00000004
	.global MapScreenCursorStartY
MapScreenCursorStartY:
	.space 0x00000004
	.global MapScreenNameRedrawPending
MapScreenNameRedrawPending:
	.space 0x00000002
	.global MapScreenNameRedrawDelay
MapScreenNameRedrawDelay:
	.space 0x000019aa
	.global MapGridShapes
MapGridShapes:
	.space 0x00000001
	.space 0x000007ff
	.space 0x00000800
	.space 0x00000e00
	.space 0x00000200
	.global MapScene
MapScene:
	.space 0x00000002
	.global MapSceneEditsOffset
MapSceneEditsOffset:
	.space 0x00000002
	.global MapSceneBoundsOffset
MapSceneBoundsOffset:
	.space 0x00000004
	.global MapScenePoints
MapScenePoints:
	.space 0x00001ff8
	.global EwramHeap
EwramHeap:
	.space 0x00008000
	.global BgTileBuffer
BgTileBuffer:
	.space 0x00002000
	.global Data0203a000
Data0203a000:
	.space 0x00002000
	.global Data0203c000
Data0203c000:
	.space 0x00002000
	.global Data0203e000
Data0203e000:
