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
	.space 0x0000000c
	.global Data_0200024c
Data_0200024c:
	.space 0x00000134
	.global gItemCounters
gItemCounters:
	.space 0x00000082
	.global Data_02000402
Data_02000402:
	.space 0x0000002a
	.global Data_0200042c
Data_0200042c:
	.space 0x00000008
	.global Data_02000434
Data_02000434:
	.space 0x00000004
	.global Data_02000438
Data_02000438:
	.space 0x00000033
	.global Data_0200046b
Data_0200046b:
	.space 0x0000000d
	.global Data_02000478
Data_02000478:
	.space 0x00000004
	.global gOverflowItem
gOverflowItem:
	.space 0x0000000e
	.global gPlayerObjectId
gPlayerObjectId:
	.space 0x0000005a
	.global gSaveStamp
gSaveStamp:
	.space 0x0000001c
	.global Data_02000500
Data_02000500:
	.space 0x00000b00
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
	.space 0x00000004
	.global Data_02002224
Data_02002224:
	.space 0x00000014
	.global gSerialReceivedSize
gSerialReceivedSize:
	.space 0x00000008
	.global gSerialRuntime
gSerialRuntime:
	.space 0x00000004
	.global Data_02002244
Data_02002244:
	.space 0x0000015c
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
	.global Flash_Handler0
Flash_Handler0:
	.space 0x00000004
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
	.space 0x00007e00
	.global Data_0200fe00
Data_0200fe00:
	.space 0x00000200
	.global gMapCellBuffer
	.type gMapCellBuffer, %object
	.size gMapCellBuffer, 0xc000
gMapCellBuffer:
	.space 0x00000001
	.global Data_02010001
Data_02010001:
	.space 0x00000001
	.global Data_02010002
Data_02010002:
	.space 0x00000016
	.global Data_02010018
Data_02010018:
	.space 0x00000064
	.global Data_0201007c
Data_0201007c:
	.space 0x00000002
	.global Data_0201007e
Data_0201007e:
	.space 0x000000c2
	.global Data_02010140
Data_02010140:
	.space 0x00000018
	.global Data_02010158
Data_02010158:
	.space 0x00000590
	.global Data_020106e8
Data_020106e8:
	.space 0x00000408
	.global Data_02010af0
Data_02010af0:
	.space 0x00000166
	.global Data_02010c56
Data_02010c56:
	.space 0x00000002
	.global Data_02010c58
Data_02010c58:
	.space 0x00000018
	.global Data_02010c70
Data_02010c70:
	.space 0x00000190
	.global Data_02010e00
Data_02010e00:
	.space 0x00000356
	.global Data_02011156
Data_02011156:
	.space 0x00000aaa
	.global Data_02011c00
Data_02011c00:
	.space 0x00000018
	.global Data_02011c18
Data_02011c18:
	.space 0x00000968
	.global Data_02012580
Data_02012580:
	.space 0x000003c0
	.global Data_02012940
Data_02012940:
	.space 0x00000116
	.global Data_02012a56
Data_02012a56:
	.space 0x0000032a
	.global Data_02012d80
Data_02012d80:
	.space 0x00000a80
	.global Data_02013800
Data_02013800:
	.space 0x00000018
	.global Data_02013818
Data_02013818:
	.space 0x00000028
	.global Data_02013840
Data_02013840:
	.space 0x00000416
	.global Data_02013c56
Data_02013c56:
	.space 0x000003aa
	.global Data_02014000
Data_02014000:
	.space 0x000006e4
	.global Data_020146e4
Data_020146e4:
	.space 0x000003ec
	.global Data_02014ad0
Data_02014ad0:
	.space 0x00000030
	.global Data_02014b00
Data_02014b00:
	.space 0x00000dd2
	.global Data_020158d2
Data_020158d2:
	.space 0x000000ae
	.global Data_02015980
Data_02015980:
	.space 0x00000480
	.global Data_02015e00
Data_02015e00:
	.space 0x00002200
	.global gActorSpriteSlots
gActorSpriteSlots:
	.space 0x000019c0
	.global Data_020199c0
Data_020199c0:
	.space 0x00000780
	.global Data_0201a140
Data_0201a140:
	.space 0x00001ec0
	.global gDecodeBuffer
	.type gDecodeBuffer, %object
	.size gDecodeBuffer, 0x4000
gDecodeBuffer:
	.space 0x00004000
	.global gMapBlocks
	.type gMapBlocks, %object
	.size gMapBlocks, 0x8000
gMapBlocks:
	.space 0x00000004
	.global Data_02020004
Data_02020004:
	.space 0x000001fe
	.global Data_02020202
Data_02020202:
	.space 0x00007dfe
	.global gMapLayerData
	.type gMapLayerData, %object
	.size gMapLayerData, 0x4000
gMapLayerData:
	.space 0x00004000
	.global gMapCollision
	.type gMapCollision, %object
	.size gMapCollision, 0x4000
gMapCollision:
	.space 0x00000001
	.global Data_0202c001
Data_0202c001:
	.space 0x00000fff
	.global Data_0202d000
Data_0202d000:
	.space 0x00000e00
	.global Data_0202de00
Data_0202de00:
	.space 0x00002200
	.global gEwramHeap
gEwramHeap:
	.space 0x00008000
	.global gBgTileBuffer
gBgTileBuffer:
