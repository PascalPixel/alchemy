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
	.global Sound_UnreadByte
Sound_UnreadByte:
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
	.global MPlayInfo_SE5
MPlayInfo_SE5:
	.space 0x00000040
	.global MPlayInfo_SE6
MPlayInfo_SE6:
	.space 0x00000040
	.global MPlayInfo_Jingle
MPlayInfo_Jingle:
	.space 0x00000040
	.global MPlayInfo_SE7
MPlayInfo_SE7:
	.space 0x00000040
	.global MPlayInfo_BGM
MPlayInfo_BGM:
	.space 0x00000040
	.global MPlayInfo_MenuSE
MPlayInfo_MenuSE:
	.space 0x00000040
	.global MPlayInfo_SE4
MPlayInfo_SE4:
	.space 0x00000040
	.global MPlayMemAccArea
MPlayMemAccArea:
	.space 0x00000010
	.global MPlayInfo_ActionSE
MPlayInfo_ActionSE:
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
	.global Credits_GreyColors
Credits_GreyColors:
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
	.global Credits_PendingSlides
Credits_PendingSlides:
	.space 0x00000004
	.global DisplayScroll_Position
DisplayScroll_Position:
	.space 0x00000004
	.global DisplayScroll_Busy
DisplayScroll_Busy:
	.space 0x00000004
	.global DisplayScroll_LastPosition
DisplayScroll_LastPosition:
	.space 0x00000004
	.global DisplayScroll_Objects
DisplayScroll_Objects:
	.space 0x00000004
	.global EndingImage_Block
EndingImage_Block:
	.space 0x00000002
	.global EndingImage_FadeFrames
EndingImage_FadeFrames:
	.space 0x00000002
	.global EndingImage_MapScroll
EndingImage_MapScroll:
	.space 0x00000004
	.global EndingImage_Scroll
EndingImage_Scroll:
	.space 0x00000004
	.global EndingImage_Row
EndingImage_Row:
	.space 0x00000002
	.global EndingImage_TargetRow
EndingImage_TargetRow:
	.space 0x00000002
	.global EndingImage_Speed
EndingImage_Speed:
	.space 0x00000004
	.global EndingImage_Map
EndingImage_Map:
	.space 0x00000004
	.global EndingImage_Strips
EndingImage_Strips:
	.space 0x00000ad4
	.global OverlayArea
OverlayArea:
	.space 0x00007f58
	.global ScriptedBattle_Snapshot
ScriptedBattle_Snapshot:
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
	.global Haures_MoundCorners
Haures_MoundCorners:
	.space 0x00000088
	.global Iris_TileBuffer
Iris_TileBuffer:
	.space 0x00000170
	.space 0x00000118
	.space 0x0000002f
	.space 0x0000000d
	.space 0x000002ac
	.global Haures_MoundStrip
Haures_MoundStrip:
	.space 0x00000008
	.space 0x000000f0
	.global Haures_MoundStripEnd
Haures_MoundStripEnd:
	.space 0x00000076
	.space 0x00000002
	.space 0x00000002
	.space 0x00000016
	.space 0x00000110
	.space 0x00000080
	.space 0x000001a0
	.global Haures_MoundShades
Haures_MoundShades:
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
	.global Serac_MeshEnd
Serac_MeshEnd:
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
	.global Data_02012f94
Data_02012f94:
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
	.global Flora_PetalTiles
Flora_PetalTiles:
	.space 0x00000002
	.space 0x00000014
	.space 0x00000006
	.space 0x000001e2
	.space 0x00000008
	.space 0x00000010
	.space 0x00000048
	.global Flora_SpritePalettes
Flora_SpritePalettes:
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
	.global Data_02015000
Data_02015000:
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
	.global Data_02017080
Data_02017080:
	.space 0x00000380
	.space 0x000002e8
	.space 0x00000918
	.global ActorSpriteSlots
ActorSpriteSlots:
	.space 0x00001600
	.global TitleIntro_LighthouseTilesTail
TitleIntro_LighthouseTilesTail:
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
	.global TitleIntro_LogoTileBuffer
TitleIntro_LogoTileBuffer:
	.space 0x00001800
	.global MapGridAnimTiles
MapGridAnimTiles:
	.space 0x00002000
	.global WorldMapStateWork
WorldMapStateWork:
	.space 0x00000004
	.space 0x0000061c
	.global MapScreen_CursorMarker
MapScreen_CursorMarker:
	.space 0x0000000c
	.global MapScreen_Cursor
MapScreen_Cursor:
	.space 0x00000004
	.global MapScreen_ScrollX
MapScreen_ScrollX:
	.space 0x00000004
	.global MapScreen_ScrollY
MapScreen_ScrollY:
	.space 0x00000004
	.global MapScreen_NameBox
MapScreen_NameBox:
	.space 0x00000002
	.space 0x00000002
	.space 0x00000002
	.space 0x00000002
	.global MapScreen_Zoomed
MapScreen_Zoomed:
	.space 0x00000002
	.global MapScreen_FirstListOnly
MapScreen_FirstListOnly:
	.space 0x00000002
	.global MapScreen_GlideTargetX
MapScreen_GlideTargetX:
	.space 0x00000004
	.global MapScreen_GlideTargetY
MapScreen_GlideTargetY:
	.space 0x00000004
	.global MapScreen_CursorStartX
MapScreen_CursorStartX:
	.space 0x00000004
	.global MapScreen_CursorStartY
MapScreen_CursorStartY:
	.space 0x00000004
	.global MapScreen_NameRedrawPending
MapScreen_NameRedrawPending:
	.space 0x00000002
	.global MapScreen_NameRedrawDelay
MapScreen_NameRedrawDelay:
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
	.global Data_0203a000
Data_0203a000:
	.space 0x00002000
	.global Data_0203c000
Data_0203c000:
	.space 0x00002000
	.global Data_0203e000
Data_0203e000:
