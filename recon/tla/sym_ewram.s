@ EWRAM variables not yet defined in C, in address order, as pret's
@ sym files list them. Each .space runs to the next variable.
	.section .sym_ewram,"aw",%nobits
	.global Data_02000000
Data_02000000:
	.space 0x00000040
	.global GameFlagBytes
GameFlagBytes:
	.space 0x00000200
	.global gPartyState
gPartyState:
	.space 0x0000000c
	.global Data_0200024c
Data_0200024c:
	.space 0x000001d6
	.global Data_02000422
Data_02000422:
	.space 0x00000014
	.global Data_02000436
Data_02000436:
	.space 0x00000012
	.global Data_02000448
Data_02000448:
	.space 0x00000004
	.global Data_0200044c
Data_0200044c:
	.space 0x00000006
	.global Data_02000452
Data_02000452:
	.space 0x00000002
	.global Data_02000454
Data_02000454:
	.space 0x00000004
	.global Data_02000458
Data_02000458:
	.space 0x00000040
	.global Data_02000498
Data_02000498:
	.space 0x00000004
	.global Data_0200049c
Data_0200049c:
	.space 0x0000000e
	.global gPlayerObjectId
gPlayerObjectId:
	.space 0x00000002
	.global Data_020004ac
Data_020004ac:
	.space 0x0000000c
	.global Data_020004b8
Data_020004b8:
	.space 0x00000048
	.global Data_02000500
Data_02000500:
	.space 0x00000004
	.global Data_02000504
Data_02000504:
	.space 0x0000001c
	.global Data_02000520
Data_02000520:
	.space 0x00000ae0
	.global Data_02001000
Data_02001000:
	.space 0x00000024
	.global Data_02001024
Data_02001024:
	.space 0x00001068
	.global Data_0200208c
Data_0200208c:
	.space 0x000002c0
	.global gSceneState
gSceneState:
	.space 0x00000078
	.global Data_020023c4
Data_020023c4:
	.space 0x00000b5c
	.global Data_02002f20
Data_02002f20:
	.space 0x000000e0
	.global gSleepActive
gSleepActive:
	.space 0x00000010
	.global gOamBuckets
gOamBuckets:
	.space 0x00000400
	.global ResourceBlockOwners
ResourceBlockOwners:
	.space 0x00000200
	.global gSchedulerTaskTable
gSchedulerTaskTable:
	.space 0x000000c0
	.global Data_020036d0
Data_020036d0:
	.space 0x00000004
	.global gSerialSendSize
gSerialSendSize:
	.space 0x00000004
	.global Data_020036d8
Data_020036d8:
	.space 0x00000008
	.global ResourceTableEntries
ResourceTableEntries:
	.space 0x00000180
	.global Data_02003860
Data_02003860:
	.space 0x00000010
	.global Data_02003870
Data_02003870:
	.space 0x00000004
	.global Data_02003874
Data_02003874:
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
	.global Data_02003a74
Data_02003a74:
	.space 0x0000001c
	.global Data_02003a90
Data_02003a90:
	.space 0x00000ce8
	.global gModelTransformWork
gModelTransformWork:
	.space 0x00000bd8
	.global Data_02005350
Data_02005350:
	.space 0x00000004
	.global gSerialReceivedSize
gSerialReceivedSize:
	.space 0x0000000c
	.global gSerialRuntime
gSerialRuntime:
	.space 0x00000004
	.global Data_02005364
Data_02005364:
	.space 0x0000015c
	.global gLinkExchangeState
gLinkExchangeState:
	.space 0x00000004
	.global gSerialBlockSequence
gSerialBlockSequence:
	.space 0x00000004
	.global Data_020054c8
Data_020054c8:
	.space 0x00000008
	.global gObjAffineMatrices
gObjAffineMatrices:
	.space 0x00000100
	.global gSerialReceiveDest
gSerialReceiveDest:
	.space 0x00000230
	.global gMusicRestoreDelay
gMusicRestoreDelay:
	.space 0x00000004
	.global Data_02005804
Data_02005804:
	.space 0x00000004
	.global gMusicVolume
gMusicVolume:
	.space 0x00000004
	.global Data_0200580c
Data_0200580c:
	.space 0x00000004
	.global Data_02005810
Data_02005810:
	.space 0x00000004
	.global Data_02005814
Data_02005814:
	.space 0x0000000c
	.global Data_02005820
Data_02005820:
	.space 0x00000010
	.global Data_02005830
Data_02005830:
	.space 0x00000004
	.global Data_02005834
Data_02005834:
	.space 0x00000004
	.global gMusicVolumeTarget
gMusicVolumeTarget:
	.space 0x00000004
	.global Data_0200583c
Data_0200583c:
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
	.global Data_02006888
Data_02006888:
	.space 0x00000004
	.global Data_0200688c
Data_0200688c:
	.space 0x00000004
	.global Sound_CgbNotes
Sound_CgbNotes:
	.space 0x00000100
	.global Data_02006990
Data_02006990:
	.space 0x00000080
	.global Data_02006a10
Data_02006a10:
	.space 0x00000080
	.global gMusicPlayerBgm
gMusicPlayerBgm:
	.space 0x000000c0
	.global Sound_WorkBytes
Sound_WorkBytes:
	.space 0x00000010
	.global Data_02006b60
Data_02006b60:
	.space 0x000008a0
	.global Flash_Handler3
Flash_Handler3:
	.space 0x00000004
	.global Flash_Handler0
Flash_Handler0:
	.space 0x00000004
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
	.space 0x000000d4
	.global Data_02007500
Data_02007500:
	.space 0x00000004
	.global Data_02007504
Data_02007504:
	.space 0x00000004
	.global Data_02007508
Data_02007508:
	.space 0x00000004
	.global Data_0200750c
Data_0200750c:
	.space 0x00000004
	.global Data_02007510
Data_02007510:
	.space 0x00000004
	.global Data_02007514
Data_02007514:
	.space 0x00000002
	.global Data_02007516
Data_02007516:
	.space 0x00000002
	.global Data_02007518
Data_02007518:
	.space 0x00000004
	.global Data_0200751c
Data_0200751c:
	.space 0x00000004
	.global Data_02007520
Data_02007520:
	.space 0x00000002
	.global gScrollTarget
gScrollTarget:
	.space 0x00000002
	.global Data_02007524
Data_02007524:
	.space 0x00000004
	.global Data_02007528
Data_02007528:
	.space 0x00000004
	.global Data_0200752c
Data_0200752c:
	.space 0x00000ad4
	.global gOverlayArea
gOverlayArea:
	.space 0x00007f58
	.global Data_0200ff58
Data_0200ff58:
	.space 0x00000014
	.global Data_0200ff6c
Data_0200ff6c:
	.space 0x00000080
	.global Data_0200ffec
Data_0200ffec:
	.space 0x0000000c
	.global Data_0200fff8
Data_0200fff8:
	.space 0x00000002
	.global Data_0200fffa
Data_0200fffa:
	.space 0x00000006
	.global gMapCellBuffer
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
	.global Data_020104f0
Data_020104f0:
	.space 0x00000088
	.global Data_02010578
Data_02010578:
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
	.global Data_02010ae8
Data_02010ae8:
	.space 0x00000008
	.global Data_02010af0
Data_02010af0:
	.space 0x000000f0
	.global Data_02010be0
Data_02010be0:
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
	.global Data_02010fa0
Data_02010fa0:
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
	.global Data_02012540
Data_02012540:
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
	.global Data_02014000
Data_02014000:
	.space 0x00000002
	.global Data_02014002
Data_02014002:
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
	.global Data_02014260
Data_02014260:
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
	.global Data_02015000
Data_02015000:
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
	.global Data_02018000
Data_02018000:
	.space 0x00001600
	.global Data_02019600
Data_02019600:
	.space 0x00002a00
	.global Data_0201c000
Data_0201c000:
	.space 0x00004000
	.global gMapBlocks
gMapBlocks:
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
	.global Data_02026800
Data_02026800:
	.space 0x00001800
	.global Data_02028000
Data_02028000:
	.space 0x00002000
	.global Data_0202a000
Data_0202a000:
	.space 0x00000004
	.global Data_0202a004
Data_0202a004:
	.space 0x0000061c
	.global Data_0202a620
Data_0202a620:
	.space 0x0000000c
	.global Data_0202a62c
Data_0202a62c:
	.space 0x00000004
	.global Data_0202a630
Data_0202a630:
	.space 0x00000004
	.global Data_0202a634
Data_0202a634:
	.space 0x00000004
	.global Data_0202a638
Data_0202a638:
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
	.global Data_0202a640
Data_0202a640:
	.space 0x00000002
	.global Data_0202a642
Data_0202a642:
	.space 0x00000002
	.global Data_0202a644
Data_0202a644:
	.space 0x00000004
	.global Data_0202a648
Data_0202a648:
	.space 0x00000004
	.global Data_0202a64c
Data_0202a64c:
	.space 0x00000004
	.global Data_0202a650
Data_0202a650:
	.space 0x00000004
	.global Data_0202a654
Data_0202a654:
	.space 0x00000002
	.global Data_0202a656
Data_0202a656:
	.space 0x000019aa
	.global gMapCollision
gMapCollision:
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
	.global Data_0202e000
Data_0202e000:
	.space 0x00000002
	.global Data_0202e002
Data_0202e002:
	.space 0x00000002
	.global Data_0202e004
Data_0202e004:
	.space 0x00000004
	.global Data_0202e008
Data_0202e008:
	.space 0x00001ff8
	.global Data_02030000
Data_02030000:
	.space 0x00008000
	.global Data_02038000
Data_02038000:
	.space 0x00002000
	.global Data_0203a000
Data_0203a000:
	.space 0x00002000
	.global Data_0203c000
Data_0203c000:
	.space 0x00002000
	.global Data_0203e000
Data_0203e000:
