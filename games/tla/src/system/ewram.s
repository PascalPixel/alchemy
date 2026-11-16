	.section .sym_ewram,"aw",%nobits
	.global gSaveBuffer
gSaveBuffer:
	.space 0x00000040
	.global gGameFlagBytes
gGameFlagBytes:
	.space 0x00000200
	.global gMainRoutineWork
gMainRoutineWork:
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
	.global gWorldPictureChoice
gWorldPictureChoice:
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
	.global gData_02003a90
gData_02003a90:
	.space 0x00000ce8
	.global gModelTransformWork
gModelTransformWork:
	.space 0x00000bd8
	.global gTitleUpdateOptionEnabled
gTitleUpdateOptionEnabled:
	.space 0x00000004
	.global gSerialReceivedSize
gSerialReceivedSize:
	.space 0x0000000c
	.global gSerialWork
gSerialWork:
	.space 0x00000004
	.global Data_02005364
Data_02005364:
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
	.global gSound_JumpCommand
gSound_JumpCommand:
	.space 0x00000084
	.global Data_02006888
Data_02006888:
	.space 0x00000004
	.global Data_0200688c
Data_0200688c:
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
	.global gEndingPicture_Block
gEndingPicture_Block:
	.space 0x00000002
	.global gEndingPicture_FadeFrames
gEndingPicture_FadeFrames:
	.space 0x00000002
	.global gEndingPicture_MapScroll
gEndingPicture_MapScroll:
	.space 0x00000004
	.global gEndingPicture_Scroll
gEndingPicture_Scroll:
	.space 0x00000004
	.global gEndingPicture_Row
gEndingPicture_Row:
	.space 0x00000002
	.global gScrollTarget
gScrollTarget:
	.space 0x00000002
	.global gEndingPicture_Speed
gEndingPicture_Speed:
	.space 0x00000004
	.global gEndingPicture_Map
gEndingPicture_Map:
	.space 0x00000004
	.global gEndingPicture_Strips
gEndingPicture_Strips:
	.space 0x00000ad4
	.global gOverlayArea
gOverlayArea:
	.space 0x00007f58
	.global gScriptedBattle_Snapshot
gScriptedBattle_Snapshot:
	.space 0x00000014
	.global gScriptedBattle_SnapshotBytes
gScriptedBattle_SnapshotBytes:
	.space 0x00000080
	.global gScriptedBattle_SnapshotVitals
gScriptedBattle_SnapshotVitals:
	.space 0x0000000c
	.global Data_0200fff8
Data_0200fff8:
	.space 0x00000002
	.global Data_0200fffa
Data_0200fffa:
	.space 0x00000006
	.global gMapGridCellBuffer
gMapGridCellBuffer:
	.space 0x00000001
	.global Data_02010001
Data_02010001:
	.space 0x00000001
	.global Data_02010002
Data_02010002:
	.space 0x00000016
	.global Data_02010018
Data_02010018:
	.space 0x00000008
	.global Data_02010020
Data_02010020:
	.space 0x00000020
	.global Data_02010040
Data_02010040:
	.space 0x0000003e
	.global Data_0201007e
Data_0201007e:
	.space 0x00000004
	.global Data_02010082
Data_02010082:
	.space 0x0000001e
	.global Data_020100a0
Data_020100a0:
	.space 0x00000068
	.global Data_02010108
Data_02010108:
	.space 0x00000038
	.global Data_02010140
Data_02010140:
	.space 0x00000018
	.global Data_02010158
Data_02010158:
	.space 0x000000a8
	.global Data_02010200
Data_02010200:
	.space 0x00000008
	.global Data_02010208
Data_02010208:
	.space 0x00000098
	.global Data_020102a0
Data_020102a0:
	.space 0x00000078
	.global Data_02010318
Data_02010318:
	.space 0x00000068
	.global Data_02010380
Data_02010380:
	.space 0x00000080
	.global Data_02010400
Data_02010400:
	.space 0x000000f0
	.global gHaures_WingCorners
gHaures_WingCorners:
	.space 0x00000088
	.global gIris_TileBuffer
gIris_TileBuffer:
	.space 0x00000170
	.global Data_020106e8
Data_020106e8:
	.space 0x00000118
	.global Data_02010800
Data_02010800:
	.space 0x0000002f
	.global Data_0201082f
Data_0201082f:
	.space 0x0000000d
	.global Data_0201083c
Data_0201083c:
	.space 0x000002ac
	.global gHaures_WingStrip
gHaures_WingStrip:
	.space 0x00000008
	.global Data_02010af0
Data_02010af0:
	.space 0x000000f0
	.global gHaures_WingStripEnd
gHaures_WingStripEnd:
	.space 0x00000076
	.global Data_02010c56
Data_02010c56:
	.space 0x00000002
	.global Data_02010c58
Data_02010c58:
	.space 0x00000002
	.global Data_02010c5a
Data_02010c5a:
	.space 0x00000016
	.global Data_02010c70
Data_02010c70:
	.space 0x00000110
	.global Data_02010d80
Data_02010d80:
	.space 0x00000080
	.global Data_02010e00
Data_02010e00:
	.space 0x000001a0
	.global gHaures_WingShades
gHaures_WingShades:
	.space 0x00000060
	.global Data_02011000
Data_02011000:
	.space 0x00000156
	.global Data_02011156
Data_02011156:
	.space 0x00000192
	.global Data_020112e8
Data_020112e8:
	.space 0x00000118
	.global Data_02011400
Data_02011400:
	.space 0x00000409
	.global Data_02011809
Data_02011809:
	.space 0x000001d7
	.global Data_020119e0
Data_020119e0:
	.space 0x00000220
	.global Data_02011c00
Data_02011c00:
	.space 0x00000018
	.global Data_02011c18
Data_02011c18:
	.space 0x00000008
	.global Data_02011c20
Data_02011c20:
	.space 0x00000300
	.global Data_02011f20
Data_02011f20:
	.space 0x00000020
	.global Data_02011f40
Data_02011f40:
	.space 0x00000028
	.global Data_02011f68
Data_02011f68:
	.space 0x00000098
	.global Data_02012000
Data_02012000:
	.space 0x00000400
	.global Data_02012400
Data_02012400:
	.space 0x000000de
	.global Data_020124de
Data_020124de:
	.space 0x00000062
	.global gSerac_MeshEnd
gSerac_MeshEnd:
	.space 0x0000018a
	.global Data_020126ca
Data_020126ca:
	.space 0x000000a1
	.global Data_0201276b
Data_0201276b:
	.space 0x00000095
	.global Data_02012800
Data_02012800:
	.space 0x00000164
	.global Data_02012964
Data_02012964:
	.space 0x000000f2
	.global Data_02012a56
Data_02012a56:
	.space 0x0000032a
	.global Data_02012d80
Data_02012d80:
	.space 0x000000ee
	.global Data_02012e6e
Data_02012e6e:
	.space 0x00000072
	.global Data_02012ee0
Data_02012ee0:
	.space 0x00000014
	.global Data_02012ef4
Data_02012ef4:
	.space 0x00000014
	.global Data_02012f08
Data_02012f08:
	.space 0x0000008c
	.global Data_02012f94
Data_02012f94:
	.space 0x000007f4
	.global Data_02013788
Data_02013788:
	.space 0x00000078
	.global Data_02013800
Data_02013800:
	.space 0x00000018
	.global Data_02013818
Data_02013818:
	.space 0x00000028
	.global Data_02013840
Data_02013840:
	.space 0x00000096
	.global Data_020138d6
Data_020138d6:
	.space 0x00000380
	.global Data_02013c56
Data_02013c56:
	.space 0x000001c2
	.global Data_02013e18
Data_02013e18:
	.space 0x000001e8
	.global gData_02014000
gData_02014000:
	.space 0x00000002
	.global gFlora_PetalTiles
gFlora_PetalTiles:
	.space 0x00000002
	.global Data_02014004
Data_02014004:
	.space 0x00000014
	.global Data_02014018
Data_02014018:
	.space 0x00000006
	.global Data_0201401e
Data_0201401e:
	.space 0x000001e2
	.global Data_02014200
Data_02014200:
	.space 0x00000008
	.global Data_02014208
Data_02014208:
	.space 0x00000010
	.global Data_02014218
Data_02014218:
	.space 0x00000048
	.global gFlora_SpritePalettes
gFlora_SpritePalettes:
	.space 0x000001a0
	.global Data_02014400
Data_02014400:
	.space 0x0000036b
	.global Data_0201476b
Data_0201476b:
	.space 0x00000095
	.global Data_02014800
Data_02014800:
	.space 0x00000100
	.global Data_02014900
Data_02014900:
	.space 0x00000089
	.global Data_02014989
Data_02014989:
	.space 0x0000004f
	.global Data_020149d8
Data_020149d8:
	.space 0x000000f8
	.global Data_02014ad0
Data_02014ad0:
	.space 0x00000030
	.global Data_02014b00
Data_02014b00:
	.space 0x00000156
	.global Data_02014c56
Data_02014c56:
	.space 0x0000002a
	.global Data_02014c80
Data_02014c80:
	.space 0x00000180
	.global Data_02014e00
Data_02014e00:
	.space 0x00000018
	.global Data_02014e18
Data_02014e18:
	.space 0x00000008
	.global Data_02014e20
Data_02014e20:
	.space 0x0000007d
	.global Data_02014e9d
Data_02014e9d:
	.space 0x00000163
	.global gData_02015000
gData_02015000:
	.space 0x00000018
	.global Data_02015018
Data_02015018:
	.space 0x00000270
	.global Data_02015288
Data_02015288:
	.space 0x000001d8
	.global Data_02015460
Data_02015460:
	.space 0x0000030b
	.global Data_0201576b
Data_0201576b:
	.space 0x00000155
	.global Data_020158c0
Data_020158c0:
	.space 0x00000012
	.global Data_020158d2
Data_020158d2:
	.space 0x000000ae
	.global Data_02015980
Data_02015980:
	.space 0x00000280
	.global Data_02015c00
Data_02015c00:
	.space 0x00000400
	.global Data_02016000
Data_02016000:
	.space 0x0000000b
	.global Data_0201600b
Data_0201600b:
	.space 0x0000000d
	.global Data_02016018
Data_02016018:
	.space 0x00000026
	.global Data_0201603e
Data_0201603e:
	.space 0x000001e2
	.global Data_02016220
Data_02016220:
	.space 0x000004c4
	.global Data_020166e4
Data_020166e4:
	.space 0x0000015a
	.global Data_0201683e
Data_0201683e:
	.space 0x000000c2
	.global Data_02016900
Data_02016900:
	.space 0x00000018
	.global Data_02016918
Data_02016918:
	.space 0x000002a8
	.global Data_02016bc0
Data_02016bc0:
	.space 0x00000018
	.global Data_02016bd8
Data_02016bd8:
	.space 0x000000e5
	.global Data_02016cbd
Data_02016cbd:
	.space 0x00000043
	.global Data_02016d00
Data_02016d00:
	.space 0x00000018
	.global Data_02016d18
Data_02016d18:
	.space 0x000000e8
	.global Data_02016e00
Data_02016e00:
	.space 0x00000016
	.global Data_02016e16
Data_02016e16:
	.space 0x0000012a
	.global Data_02016f40
Data_02016f40:
	.space 0x00000018
	.global Data_02016f58
Data_02016f58:
	.space 0x00000128
	.global Data_02017080
Data_02017080:
	.space 0x00000380
	.global Data_02017400
Data_02017400:
	.space 0x000002e8
	.global Data_020176e8
Data_020176e8:
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
	.global gCellPictures
gCellPictures:
	.space 0x00000004
	.global Data_02020004
Data_02020004:
	.space 0x000001fe
	.global Data_02020202
Data_02020202:
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
	.global gWorldMapWork
gWorldMapWork:
	.space 0x00000004
	.global Data_0202a004
Data_0202a004:
	.space 0x0000061c
	.global gWorldPicture_CursorMarker
gWorldPicture_CursorMarker:
	.space 0x0000000c
	.global gWorldPicture_Cursor
gWorldPicture_Cursor:
	.space 0x00000004
	.global gWorldPicture_ScrollX
gWorldPicture_ScrollX:
	.space 0x00000004
	.global gWorldPicture_ScrollY
gWorldPicture_ScrollY:
	.space 0x00000004
	.global gWorldPicture_NameBox
gWorldPicture_NameBox:
	.space 0x00000002
	.global Data_0202a63a
Data_0202a63a:
	.space 0x00000002
	.global Data_0202a63c
Data_0202a63c:
	.space 0x00000002
	.global Data_0202a63e
Data_0202a63e:
	.space 0x00000002
	.global gWorldPicture_Zoomed
gWorldPicture_Zoomed:
	.space 0x00000002
	.global gWorldPicture_FirstListOnly
gWorldPicture_FirstListOnly:
	.space 0x00000002
	.global gWorldPicture_GlideTargetX
gWorldPicture_GlideTargetX:
	.space 0x00000004
	.global gWorldPicture_GlideTargetY
gWorldPicture_GlideTargetY:
	.space 0x00000004
	.global gWorldPicture_CursorStartX
gWorldPicture_CursorStartX:
	.space 0x00000004
	.global gWorldPicture_CursorStartY
gWorldPicture_CursorStartY:
	.space 0x00000004
	.global gWorldPicture_NameRedrawPending
gWorldPicture_NameRedrawPending:
	.space 0x00000002
	.global gWorldPicture_NameRedrawDelay
gWorldPicture_NameRedrawDelay:
	.space 0x000019aa
	.global gMapGridShapes
gMapGridShapes:
	.space 0x00000001
	.global Data_0202c001
Data_0202c001:
	.space 0x000007ff
	.global Data_0202c800
Data_0202c800:
	.space 0x00000800
	.global Data_0202d000
Data_0202d000:
	.space 0x00000e00
	.global Data_0202de00
Data_0202de00:
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
