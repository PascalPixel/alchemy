	.section .sym_ewram,"aw",%nobits
	.global gSaveBuffer
gSaveBuffer:
	.space 0x00000040
	.global gGameFlagBytes
gGameFlagBytes:
	.space 0x00000200
	.global gMainRoutineState
gMainRoutineState:
	.space 0x0000000c
	.global gPartyDjinnRecovery
gPartyDjinnRecovery:
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
	.global gMapScreenChoice
gMapScreenChoice:
	.space 0x00000004
	.space 0x0000001c
	.global gCharacterUnits
gCharacterUnits:
	.space 0x00000ae0
	.global gSceneLocalState
gSceneLocalState:
	.space 0x00000024
	.global gSnapshotObjects
gSnapshotObjects:
	.space 0x00001068
	.global gRareItemStock
gRareItemStock:
	.space 0x000002c0
	.global gSceneState
gSceneState:
	.space 0x00000078
	.global gInventorySnapshot
gInventorySnapshot:
	.space 0x00000b5c
	.global gSoundFlags
gSoundFlags:
	.space 0x000000e0
	.global gSleepActive
gSleepActive:
	.space 0x00000010
	.global gOamDepthLists
gOamDepthLists:
	.space 0x00000400
	.global gVramCacheBlockOwners
gVramCacheBlockOwners:
	.space 0x00000200
	.global gTasks
gTasks:
	.space 0x000000c0
	.global gSaveSlot
gSaveSlot:
	.space 0x00000004
	.global gSerialSendSize
gSerialSendSize:
	.space 0x00000004
	.global gTitleBattleOptionEnabled
gTitleBattleOptionEnabled:
	.space 0x00000008
	.global gVramCacheEntries
gVramCacheEntries:
	.space 0x00000180
	.global gAnyFileCleared
gAnyFileCleared:
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
	.space 0x00000020
	.global gMixerPcmBuffer
gMixerPcmBuffer:
	.space 0x00000ce8
	.global gModelTransformState
gModelTransformState:
	.space 0x00000bd8
	.global gTitleUpdateOptionEnabled
gTitleUpdateOptionEnabled:
	.space 0x00000004
	.global gSerialReceivedSize
gSerialReceivedSize:
	.space 0x0000000c
	.global gSerialState
gSerialState:
	.space 0x00000004
	.space 0x0000015c
	.global gLinkExchangeState
gLinkExchangeState:
	.space 0x00000004
	.global gSerialBlockNumber
gSerialBlockNumber:
	.space 0x00000004
	.global gBattleRandomSeed
gBattleRandomSeed:
	.space 0x00000008
	.global gOamMatrices
gOamMatrices:
	.space 0x00000100
	.global gSerialReceiveDestination
gSerialReceiveDestination:
	.space 0x00000230
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
	.global gCurrentBgm
gCurrentBgm:
	.space 0x00000004
	.global gBgmTempoPitchTarget
gBgmTempoPitchTarget:
	.space 0x00000004
	.global gBgmVolumeTarget
gBgmVolumeTarget:
	.space 0x00000004
	.global gBgmTempoPitch
gBgmTempoPitch:
	.space 0x00000004
	.global gSoundCommandMask
gSoundCommandMask:
	.space 0x00000010
	.global gSoundInfo
gSoundInfo:
	.space 0x00000fb0
	.global gMPlayJumpTable
gMPlayJumpTable:
	.space 0x00000004
	.space 0x00000084
	.space 0x00000004
	.space 0x00000004
	.global gCgbChans
gCgbChans:
	.space 0x00000100
	.global gMPlayInfo_SE5
gMPlayInfo_SE5:
	.space 0x00000040
	.global gMPlayInfo_SE6
gMPlayInfo_SE6:
	.space 0x00000040
	.global gMPlayInfo_Jingle
gMPlayInfo_Jingle:
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
	.space 0x000000d4
	.global gCredits_PendingSlides
gCredits_PendingSlides:
	.space 0x00000004
	.global gDisplayScroll_Position
gDisplayScroll_Position:
	.space 0x00000004
	.global gDisplayScroll_Busy
gDisplayScroll_Busy:
	.space 0x00000004
	.global gDisplayScroll_LastPosition
gDisplayScroll_LastPosition:
	.space 0x00000004
	.global gDisplayScroll_Objects
gDisplayScroll_Objects:
	.space 0x00000004
	.global gEndingImage_Block
gEndingImage_Block:
	.space 0x00000002
	.global gEndingImage_FadeFrames
gEndingImage_FadeFrames:
	.space 0x00000002
	.global gEndingImage_MapScroll
gEndingImage_MapScroll:
	.space 0x00000004
	.global gEndingImage_Scroll
gEndingImage_Scroll:
	.space 0x00000004
	.global gEndingImage_Row
gEndingImage_Row:
	.space 0x00000002
	.global gEndingImage_TargetRow
gEndingImage_TargetRow:
	.space 0x00000002
	.global gEndingImage_Speed
gEndingImage_Speed:
	.space 0x00000004
	.global gEndingImage_Map
gEndingImage_Map:
	.space 0x00000004
	.global gEndingImage_Strips
gEndingImage_Strips:
	.space 0x00000ad4
	.global gOverlayArea
gOverlayArea:
	.space 0x00007f58
	.global gScriptedBattle_Snapshot
gScriptedBattle_Snapshot:
	.space 0x00000014
	.space 0x00000080
	.space 0x0000000c
	.space 0x00000002
	.space 0x00000006
	.global gMapGridCellBuffer
gMapGridCellBuffer:
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
	.global gHaures_MoundCorners
gHaures_MoundCorners:
	.space 0x00000088
	.global gIris_TileBuffer
gIris_TileBuffer:
	.space 0x00000170
	.space 0x00000118
	.space 0x0000002f
	.space 0x0000000d
	.space 0x000002ac
	.global gHaures_MoundStrip
gHaures_MoundStrip:
	.space 0x00000008
	.space 0x000000f0
	.global gHaures_MoundStripEnd
gHaures_MoundStripEnd:
	.space 0x00000076
	.space 0x00000002
	.space 0x00000002
	.space 0x00000016
	.space 0x00000110
	.space 0x00000080
	.space 0x000001a0
	.global gHaures_MoundShades
gHaures_MoundShades:
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
	.global gSerac_MeshEnd
gSerac_MeshEnd:
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
	.global gEffectScratch
gEffectScratch:
	.space 0x00000002
	.global gFlora_PetalTiles
gFlora_PetalTiles:
	.space 0x00000002
	.space 0x00000014
	.space 0x00000006
	.space 0x000001e2
	.space 0x00000008
	.space 0x00000010
	.space 0x00000048
	.global gFlora_SpritePalettes
gFlora_SpritePalettes:
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
	.global gData_02015000
gData_02015000:
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
	.global gActorSpriteSlots
gActorSpriteSlots:
	.space 0x00001600
	.global gTitleIntro_LighthouseTilesTail
gTitleIntro_LighthouseTilesTail:
	.space 0x00002a00
	.global gDecodeBuffer
gDecodeBuffer:
	.space 0x00004000
	.global gCellTiles
gCellTiles:
	.space 0x00000004
	.space 0x000001fe
	.space 0x00003dfe
	.global gMapShapeGrid
gMapShapeGrid:
	.space 0x00002800
	.global gTitleIntro_LogoTileBuffer
gTitleIntro_LogoTileBuffer:
	.space 0x00001800
	.global gMapGridAnimTiles
gMapGridAnimTiles:
	.space 0x00002000
	.global gWorldMapState
gWorldMapState:
	.space 0x00000004
	.space 0x0000061c
	.global gMapScreen_CursorMarker
gMapScreen_CursorMarker:
	.space 0x0000000c
	.global gMapScreen_Cursor
gMapScreen_Cursor:
	.space 0x00000004
	.global gMapScreen_ScrollX
gMapScreen_ScrollX:
	.space 0x00000004
	.global gMapScreen_ScrollY
gMapScreen_ScrollY:
	.space 0x00000004
	.global gMapScreen_NameBox
gMapScreen_NameBox:
	.space 0x00000002
	.space 0x00000002
	.space 0x00000002
	.space 0x00000002
	.global gMapScreen_Zoomed
gMapScreen_Zoomed:
	.space 0x00000002
	.global gMapScreen_FirstListOnly
gMapScreen_FirstListOnly:
	.space 0x00000002
	.global gMapScreen_GlideTargetX
gMapScreen_GlideTargetX:
	.space 0x00000004
	.global gMapScreen_GlideTargetY
gMapScreen_GlideTargetY:
	.space 0x00000004
	.global gMapScreen_CursorStartX
gMapScreen_CursorStartX:
	.space 0x00000004
	.global gMapScreen_CursorStartY
gMapScreen_CursorStartY:
	.space 0x00000004
	.global gMapScreen_NameRedrawPending
gMapScreen_NameRedrawPending:
	.space 0x00000002
	.global gMapScreen_NameRedrawDelay
gMapScreen_NameRedrawDelay:
	.space 0x000019aa
	.global gMapGridShapes
gMapGridShapes:
	.space 0x00000001
	.space 0x000007ff
	.space 0x00000800
	.space 0x00000e00
	.space 0x00000200
	.global gMapScene
gMapScene:
	.space 0x00000002
	.global gMapSceneEditsOffset
gMapSceneEditsOffset:
	.space 0x00000002
	.global gMapSceneBoundsOffset
gMapSceneBoundsOffset:
	.space 0x00000004
	.global gMapScenePoints
gMapScenePoints:
	.space 0x00001ff8
	.global gEwramHeap
gEwramHeap:
	.space 0x00008000
	.global gBgTileBuffer
gBgTileBuffer:
	.space 0x00002000
	.global gData_0203a000
gData_0203a000:
	.space 0x00002000
	.global gData_0203c000
gData_0203c000:
	.space 0x00002000
	.global gData_0203e000
gData_0203e000:
