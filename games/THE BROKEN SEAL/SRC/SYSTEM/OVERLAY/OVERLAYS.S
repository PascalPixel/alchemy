@ The overlay streams, each overlay's code and data compressed as one
@ resource and listed in RESOURCE/DIRECTORY.S. Camelot's packer wrote each
@ stream rounded up to four bytes from one reused buffer, so the bytes
@ after a stream are what the most recent earlier resource long enough to
@ reach them left there; zero fill is the alignment itself.
	.section .overlays,"a"

	.global Resource_Overlay36F
Resource_Overlay36F:
	.incbin "overlays/resource_36f.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xa0, 0x4f	@ PACKER: left in the buffer by Field_Map171+0x45e
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xa0, 0x4f	@ PACKER: left in the buffer by Field_Map171+0x45e
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x31, 0x32, 0x24	@ PACKER: left in the buffer by Field_Map171+0x46d
	.endif
	.balign 4, 0

	.global Resource_Overlay370
Resource_Overlay370:
	.incbin "overlays/resource_370.lz"
	.ifdef TBS_EDITION_EN
	.byte 0x00, 0x00, 0xdd	@ PACKER: left in the buffer, no earlier resource holds these bytes
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x00, 0xdd	@ PACKER: left in the buffer, no earlier resource holds these bytes
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x08	@ PACKER: left in the buffer, no earlier resource holds these bytes
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x00, 0x00, 0xdd	@ PACKER: left in the buffer, no earlier resource holds these bytes
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x08	@ PACKER: left in the buffer, no earlier resource holds these bytes
	.endif
	.balign 4, 0

	.global Resource_Overlay371
Resource_Overlay371:
	.incbin "overlays/resource_371.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x03, 0x0c, 0xf7	@ PACKER: left in the buffer by Resource_Data326+0x3555
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x03, 0xe6	@ PACKER: left in the buffer by Resource_Data326+0x3552
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xe6, 0x03	@ PACKER: left in the buffer by Resource_Data326+0x3552
	.endif
	.balign 4, 0

	.global Resource_Overlay372
Resource_Overlay372:
	.incbin "overlays/resource_372.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x3f, 0x03	@ PACKER: left in the buffer by Resource_Overlay371+0x2b7e
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x7e, 0x06, 0xc2	@ PACKER: left in the buffer by Resource_Overlay371+0x2b81
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x8e, 0x0e	@ PACKER: left in the buffer by Resource_Overlay371+0x2b66
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x8e, 0x0e	@ PACKER: left in the buffer by Resource_Overlay371+0x2b66
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xe0, 0xbd	@ PACKER: left in the buffer by Resource_Overlay371+0x2b66
	.endif
	.balign 4, 0

	.global Resource_Overlay373
Resource_Overlay373:
	.incbin "overlays/resource_373.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x6e, 0x2b, 0x82	@ PACKER: left in the buffer by Resource_Data326+0x3ac5
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x2b, 0x82	@ PACKER: left in the buffer by Resource_Data326+0x3ac6
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xab, 0x02, 0x3a	@ PACKER: left in the buffer by Resource_Data326+0x3ad9
	.endif
	.balign 4, 0

	.global Resource_Overlay374
Resource_Overlay374:
	.incbin "overlays/resource_374.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x46, 0xd7, 0xfc	@ PACKER: left in the buffer by Resource_Overlay373+0x1cd9
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x50	@ PACKER: left in the buffer by Resource_Overlay373+0x1cdb
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x68, 0x81, 0x46	@ PACKER: left in the buffer by Resource_Overlay373+0x1dc1
	.endif
	.balign 4, 0

	.global Resource_Overlay375
Resource_Overlay375:
	.incbin "overlays/resource_375.lz"
	.ifdef TBS_EDITION_DE
	.byte 0x04, 0x0a, 0x13	@ PACKER: left in the buffer by Resource_Overlay374+0xed1
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x04, 0x0b	@ PACKER: left in the buffer by Resource_Overlay374+0xece
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x78, 0x04	@ PACKER: left in the buffer by Resource_Overlay374+0xece
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xb4, 0x04, 0x0b	@ PACKER: left in the buffer by Resource_Overlay374+0xecd
	.endif
	.balign 4, 0

	.global Resource_Overlay376
Resource_Overlay376:
	.incbin "overlays/resource_376.lz"
	.balign 4, 0

	.global Resource_Overlay377
Resource_Overlay377:
	.incbin "overlays/resource_377.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x42	@ PACKER: left in the buffer by Resource_Overlay374+0x14bf
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0xed	@ PACKER: left in the buffer by Resource_Overlay374+0x14c7
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x21	@ PACKER: left in the buffer by Resource_Overlay374+0x14c7
	.endif
	.balign 4, 0

	.global Resource_Overlay378
Resource_Overlay378:
	.incbin "overlays/resource_378.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x18, 0x48	@ PACKER: left in the buffer by Resource_Overlay374+0x195e
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x72	@ PACKER: left in the buffer by Resource_Overlay374+0x1973
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xe4, 0x0f	@ PACKER: left in the buffer by Resource_Overlay374+0x1972
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x30, 0x30	@ PACKER: left in the buffer by Resource_Overlay374+0x1972
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xe4, 0x0f	@ PACKER: left in the buffer by Resource_Overlay374+0x1972
	.endif
	.balign 4, 0

	.global Resource_Overlay379
Resource_Overlay379:
	.incbin "overlays/resource_379.lz"
	.ifdef TBS_EDITION_DE
	.byte 0x07	@ PACKER: left in the buffer by Resource_Overlay378+0x673
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x07	@ PACKER: left in the buffer by Resource_Overlay378+0x673
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x07	@ PACKER: left in the buffer by Resource_Overlay378+0x673
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x07	@ PACKER: left in the buffer by Resource_Overlay378+0x673
	.endif
	.balign 4, 0

	.global Resource_Overlay37A
Resource_Overlay37A:
	.incbin "overlays/resource_37a.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x5e, 0x0a, 0x3b	@ PACKER: left in the buffer by Resource_Overlay378+0x14c1
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x7c	@ PACKER: left in the buffer by Resource_Overlay378+0x14bf
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x7c, 0x04, 0x64	@ PACKER: left in the buffer by Resource_Overlay378+0x14c1
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x64, 0x3e	@ PACKER: left in the buffer by Resource_Overlay378+0x14c2
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x64, 0x3e	@ PACKER: left in the buffer by Resource_Overlay378+0x14c2
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x64, 0x3e	@ PACKER: left in the buffer by Resource_Overlay378+0x14c2
	.endif
	.balign 4, 0

	.global Resource_Overlay37B
Resource_Overlay37B:
	.incbin "overlays/resource_37b.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x05, 0x08	@ PACKER: left in the buffer, no earlier resource holds these bytes
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x51, 0x05, 0x08	@ PACKER: left in the buffer, no earlier resource holds these bytes
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x00, 0xb1	@ PACKER: left in the buffer, no earlier resource holds these bytes
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x51, 0x05, 0x08	@ PACKER: left in the buffer, no earlier resource holds these bytes
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x51, 0x05, 0x08	@ PACKER: left in the buffer, no earlier resource holds these bytes
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x51, 0x05, 0x08	@ PACKER: left in the buffer, no earlier resource holds these bytes
	.endif
	.balign 4, 0

	.global Resource_Overlay37C
Resource_Overlay37C:
	.incbin "overlays/resource_37c.lz"
	.balign 4, 0

	.global Resource_Overlay37D
Resource_Overlay37D:
	.incbin "overlays/resource_37d.lz"
	.balign 4, 0

	.global Resource_Overlay37E
Resource_Overlay37E:
	.incbin "overlays/resource_37e.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x10, 0x4e	@ PACKER: left in the buffer by Resource_Overlay37D+0xce
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x10, 0x4e	@ PACKER: left in the buffer by Resource_Overlay37D+0xce
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x10, 0x4e	@ PACKER: left in the buffer by Resource_Overlay37D+0xce
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x10, 0x4e	@ PACKER: left in the buffer by Resource_Overlay37D+0xce
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x10, 0x4e	@ PACKER: left in the buffer by Resource_Overlay37D+0xce
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x10, 0x4e	@ PACKER: left in the buffer by Resource_Overlay37D+0xce
	.endif
	.balign 4, 0

	.global Resource_Overlay37F
Resource_Overlay37F:
	.incbin "overlays/resource_37f.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xdb, 0xd4, 0x6d	@ PACKER: left in the buffer by Resource_Overlay37B+0x109d
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0xcb, 0x32, 0x2d	@ PACKER: left in the buffer by Resource_Overlay37B+0x109d
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x36, 0x75, 0x9b	@ PACKER: left in the buffer by Resource_Overlay37B+0x109d
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x36, 0x75, 0x9b	@ PACKER: left in the buffer by Resource_Overlay37B+0x109d
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x36, 0x75, 0x9b	@ PACKER: left in the buffer by Resource_Overlay37B+0x109d
	.endif
	.balign 4, 0

	.global Resource_Overlay380
Resource_Overlay380:
	.incbin "overlays/resource_380.lz"
	.ifdef TBS_EDITION_EN
	.byte 0xc5	@ PACKER: left in the buffer by Resource_Overlay373+0x280b
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x39, 0x03, 0xc5	@ PACKER: left in the buffer by Resource_Overlay373+0x2809
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xd8, 0x65	@ PACKER: left in the buffer by Resource_Overlay373+0x280a
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x39, 0x03	@ PACKER: left in the buffer by Resource_Overlay373+0x280a
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xd8, 0x65	@ PACKER: left in the buffer by Resource_Overlay373+0x280a
	.endif
	.balign 4, 0

	.global Resource_Overlay381
Resource_Overlay381:
	.incbin "overlays/resource_381.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x76, 0x3d, 0x65	@ PACKER: left in the buffer by Resource_Overlay380+0x1fd9
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x75, 0x86	@ PACKER: left in the buffer by Resource_Overlay380+0x1fda
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xfc, 0x75	@ PACKER: left in the buffer by Resource_Overlay380+0x1fda
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xfc, 0x75	@ PACKER: left in the buffer by Resource_Overlay380+0x1fda
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x03	@ PACKER: left in the buffer by Resource_Overlay380+0x1fd7
	.endif
	.balign 4, 0

	.global Resource_Overlay382
Resource_Overlay382:
	.incbin "overlays/resource_382.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x6e, 0x3e, 0x07	@ PACKER: left in the buffer by Resource_Overlay381+0x1359
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xa3, 0x6e, 0x3e	@ PACKER: left in the buffer by Resource_Overlay381+0x1355
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x06	@ PACKER: left in the buffer by Resource_Overlay381+0x1353
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x30	@ PACKER: left in the buffer by Resource_Overlay381+0x1353
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x30	@ PACKER: left in the buffer by Resource_Overlay381+0x1353
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x30	@ PACKER: left in the buffer by Resource_Overlay381+0x1353
	.endif
	.balign 4, 0

	.global Resource_Overlay383
Resource_Overlay383:
	.incbin "overlays/resource_383.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x20	@ PACKER: left in the buffer by Resource_Overlay373+0x2cf3
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x46, 0xe0	@ PACKER: left in the buffer by Resource_Overlay373+0x2c7e
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x01, 0x1e	@ PACKER: left in the buffer by Resource_Overlay373+0x2c7e
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xf2	@ PACKER: left in the buffer by Resource_Overlay373+0x2c7f
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x01	@ PACKER: left in the buffer by Resource_Overlay373+0x2c7f
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xf2	@ PACKER: left in the buffer by Resource_Overlay373+0x2c7f
	.endif
	.balign 4, 0

	.global Resource_Overlay384
Resource_Overlay384:
	.incbin "overlays/resource_384.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x0a, 0xc7	@ PACKER: left in the buffer by Resource_Overlay383+0x252
	.endif
	.balign 4, 0

	.global Resource_Overlay385
Resource_Overlay385:
	.incbin "overlays/resource_385.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xb8	@ PACKER: left in the buffer by Resource_Overlay383+0xe13
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xe0, 0x2d, 0xbc	@ PACKER: left in the buffer by Resource_Overlay383+0xe19
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x21	@ PACKER: left in the buffer by Resource_Overlay383+0xe17
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xc0	@ PACKER: left in the buffer by Resource_Overlay383+0xe17
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xc0	@ PACKER: left in the buffer by Resource_Overlay383+0xe17
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xe0, 0x2d, 0xbc	@ PACKER: left in the buffer by Resource_Overlay383+0xe19
	.endif
	.balign 4, 0

	.global Resource_Overlay386
Resource_Overlay386:
	.incbin "overlays/resource_386.lz"
	.ifdef TBS_EDITION_EN
	.byte 0x03	@ PACKER: left in the buffer by Resource_Overlay385+0x617
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0xc6, 0x40, 0x8e	@ PACKER: left in the buffer by Resource_Overlay385+0x619
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xc6, 0x40, 0x8e	@ PACKER: left in the buffer by Resource_Overlay385+0x619
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xc6, 0x40, 0x8e	@ PACKER: left in the buffer by Resource_Overlay385+0x619
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x03	@ PACKER: left in the buffer by Resource_Overlay385+0x617
	.endif
	.balign 4, 0

	.global Resource_Overlay387
Resource_Overlay387:
	.incbin "overlays/resource_387.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x78	@ PACKER: left in the buffer by Resource_Overlay385+0xbcf
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xc1	@ PACKER: left in the buffer by Resource_Overlay385+0xbd3
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x0b	@ PACKER: left in the buffer by Resource_Overlay385+0xbd3
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xc5, 0x2a	@ PACKER: left in the buffer by Resource_Overlay385+0xbf2
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xc5, 0x2a	@ PACKER: left in the buffer by Resource_Overlay385+0xbf2
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x04, 0x0d, 0x8a	@ PACKER: left in the buffer by Resource_Overlay385+0xbf1
	.endif
	.balign 4, 0

	.global Resource_Overlay388
Resource_Overlay388:
	.incbin "overlays/resource_388.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x90, 0x58	@ PACKER: left in the buffer by Resource_Overlay387+0x122
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x90, 0x58	@ PACKER: left in the buffer by Resource_Overlay387+0x122
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x90, 0x58	@ PACKER: left in the buffer by Resource_Overlay387+0x122
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x90, 0x58	@ PACKER: left in the buffer by Resource_Overlay387+0x122
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x90, 0x58	@ PACKER: left in the buffer by Resource_Overlay387+0x122
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x90, 0x58	@ PACKER: left in the buffer by Resource_Overlay387+0x122
	.endif
	.balign 4, 0

	.global Resource_Overlay389
Resource_Overlay389:
	.incbin "overlays/resource_389.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x04	@ PACKER: left in the buffer by Resource_Overlay383+0x108f
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x0e, 0xe8	@ PACKER: left in the buffer by Resource_Overlay383+0x101a
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x02	@ PACKER: left in the buffer by Resource_Overlay383+0x101b
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x02	@ PACKER: left in the buffer by Resource_Overlay383+0x101b
	.endif
	.balign 4, 0

	.global Resource_Overlay38A
Resource_Overlay38A:
	.incbin "overlays/resource_38a.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x82, 0x07, 0x21	@ PACKER: left in the buffer by Resource_Overlay389+0x679
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x16, 0x56, 0x82	@ PACKER: left in the buffer by Resource_Overlay389+0x679
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x16, 0x56, 0x82	@ PACKER: left in the buffer by Resource_Overlay389+0x679
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x16, 0x56, 0x82	@ PACKER: left in the buffer by Resource_Overlay389+0x679
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x16, 0x56, 0x82	@ PACKER: left in the buffer by Resource_Overlay389+0x679
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x16, 0x56, 0x82	@ PACKER: left in the buffer by Resource_Overlay389+0x679
	.endif
	.balign 4, 0

	.global Resource_Overlay38B
Resource_Overlay38B:
	.incbin "overlays/resource_38b.lz"
	.ifdef TBS_EDITION_DE
	.byte 0x38, 0x03	@ PACKER: left in the buffer by Resource_Overlay389+0xf72
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xf0, 0x00, 0xce	@ PACKER: left in the buffer by Resource_Overlay389+0xf71
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xf0, 0x00, 0xce	@ PACKER: left in the buffer by Resource_Overlay389+0xf71
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x78, 0x00, 0x67	@ PACKER: left in the buffer by Resource_Overlay389+0xf71
	.endif
	.balign 4, 0

	.global Resource_Overlay38C
Resource_Overlay38C:
	.incbin "overlays/resource_38c.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x09, 0xf0, 0x9b	@ PACKER: left in the buffer by Resource_Overlay38B+0x669
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x4c, 0x80, 0xdf	@ PACKER: left in the buffer by Resource_Overlay38B+0x66d
	.endif
	.balign 4, 0

	.global Resource_Overlay38D
Resource_Overlay38D:
	.incbin "overlays/resource_38d.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x94	@ PACKER: left in the buffer by Resource_Overlay383+0x1653
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xfd, 0x11, 0x21	@ PACKER: left in the buffer by Resource_Overlay383+0x1655
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x71, 0xc2, 0x01	@ PACKER: left in the buffer by Resource_Overlay383+0x1651
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x71, 0xc2, 0x01	@ PACKER: left in the buffer by Resource_Overlay383+0x1651
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x71, 0xc2, 0x01	@ PACKER: left in the buffer by Resource_Overlay383+0x1651
	.endif
	.balign 4, 0

	.global Resource_Overlay38E
Resource_Overlay38E:
	.incbin "overlays/resource_38e.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x71	@ PACKER: left in the buffer by Resource_Overlay38D+0x82b
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x47	@ PACKER: left in the buffer by Resource_Overlay38D+0x82f
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x95, 0xf7	@ PACKER: left in the buffer by Resource_Overlay38D+0x82e
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x38, 0x66, 0x10	@ PACKER: left in the buffer by Resource_Overlay38D+0x839
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x38, 0x66, 0x10	@ PACKER: left in the buffer by Resource_Overlay38D+0x839
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x66, 0x10	@ PACKER: left in the buffer by Resource_Overlay38D+0x83a
	.endif
	.balign 4, 0

	.global Resource_Overlay38F
Resource_Overlay38F:
	.incbin "overlays/resource_38f.lz"
	.ifdef TBS_EDITION_EN
	.byte 0xb0	@ PACKER: left in the buffer by Resource_Overlay383+0x1a4f
	.endif
	.balign 4, 0

	.global Resource_Overlay390
Resource_Overlay390:
	.incbin "overlays/resource_390.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x51, 0x16, 0x40	@ PACKER: left in the buffer by Resource_Overlay38F+0x499
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x16, 0x40	@ PACKER: left in the buffer by Resource_Overlay38F+0x49a
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x16, 0x40	@ PACKER: left in the buffer by Resource_Overlay38F+0x49a
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x16, 0x40	@ PACKER: left in the buffer by Resource_Overlay38F+0x49a
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x16, 0x40	@ PACKER: left in the buffer by Resource_Overlay38F+0x49a
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x16, 0x40	@ PACKER: left in the buffer by Resource_Overlay38F+0x49a
	.endif
	.balign 4, 0

	.global Resource_Overlay391
Resource_Overlay391:
	.incbin "overlays/resource_391.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x43	@ PACKER: left in the buffer by Resource_Overlay383+0x1b27
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x04, 0x14	@ PACKER: left in the buffer by Resource_Overlay383+0x1b42
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x22, 0x04	@ PACKER: left in the buffer by Resource_Overlay383+0x1b42
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x80, 0x22	@ PACKER: left in the buffer by Resource_Overlay383+0x1b42
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x80, 0x22	@ PACKER: left in the buffer by Resource_Overlay383+0x1b42
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x80, 0x22	@ PACKER: left in the buffer by Resource_Overlay383+0x1b42
	.endif
	.balign 4, 0

	.global Resource_Overlay392
Resource_Overlay392:
	.incbin "overlays/resource_392.lz"
	.ifdef TBS_EDITION_DE
	.byte 0x3c	@ PACKER: left in the buffer by Resource_Overlay391+0xbff
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x3c	@ PACKER: left in the buffer by Resource_Overlay391+0xbff
	.endif
	.balign 4, 0

	.global Resource_Overlay393
Resource_Overlay393:
	.incbin "overlays/resource_393.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x3c	@ PACKER: left in the buffer by Resource_Overlay392+0xbe7
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x7c, 0x00	@ PACKER: left in the buffer by Resource_Overlay392+0xbfe
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x3c	@ PACKER: left in the buffer by Resource_Overlay392+0xbff
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x00, 0x3c	@ PACKER: left in the buffer by Resource_Overlay392+0xbfe
	.endif
	.balign 4, 0

	.global Resource_Overlay394
Resource_Overlay394:
	.incbin "overlays/resource_394.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x85	@ PACKER: left in the buffer by Resource_Overlay393+0xb83
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x91, 0xf8, 0x00	@ PACKER: left in the buffer by Resource_Overlay393+0xb81
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x1f, 0x60	@ PACKER: left in the buffer by Resource_Overlay393+0xb82
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x1f, 0x60	@ PACKER: left in the buffer by Resource_Overlay393+0xb82
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x3e, 0xc0, 0x1c	@ PACKER: left in the buffer by Resource_Overlay393+0xb81
	.endif
	.balign 4, 0

	.global Resource_Overlay395
Resource_Overlay395:
	.incbin "overlays/resource_395.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x78, 0x4d, 0x8a	@ PACKER: left in the buffer by Resource_Overlay391+0xea5
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xc1, 0x36, 0xae	@ PACKER: left in the buffer by Resource_Overlay391+0xea5
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xc1, 0x36, 0xae	@ PACKER: left in the buffer by Resource_Overlay391+0xea5
	.endif
	.balign 4, 0

	.global Resource_Overlay396
Resource_Overlay396:
	.incbin "overlays/resource_396.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x96, 0x28	@ PACKER: left in the buffer by Resource_Overlay391+0x15c2
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x8c, 0x0e, 0xe2	@ PACKER: left in the buffer by Resource_Overlay391+0x15c1
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x43, 0x81, 0xd0	@ PACKER: left in the buffer by Resource_Overlay391+0x15c5
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x43, 0x81, 0xd0	@ PACKER: left in the buffer by Resource_Overlay391+0x15c5
	.endif
	.balign 4, 0

	.global Resource_Overlay397
Resource_Overlay397:
	.incbin "overlays/resource_397.lz"
	.ifdef TBS_EDITION_DE
	.byte 0xe9	@ PACKER: left in the buffer by Resource_Overlay396+0x30b
	.endif
	.balign 4, 0

	.global Resource_Overlay398
Resource_Overlay398:
	.incbin "overlays/resource_398.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x70, 0xa6, 0x69	@ PACKER: left in the buffer by Resource_Overlay396+0x7b5
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x70, 0xa6, 0x69	@ PACKER: left in the buffer by Resource_Overlay396+0x7b5
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x70, 0xa6, 0x69	@ PACKER: left in the buffer by Resource_Overlay396+0x7b5
	.endif
	.balign 4, 0

	.global Resource_Overlay399
Resource_Overlay399:
	.incbin "overlays/resource_399.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xf2, 0xfb	@ PACKER: left in the buffer by Resource_Overlay391+0x19f2
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x5b	@ PACKER: left in the buffer by Resource_Overlay391+0x19f3
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x14	@ PACKER: left in the buffer by Resource_Overlay391+0x19ef
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xf7, 0x98	@ PACKER: left in the buffer by Resource_Overlay391+0x1a06
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xf7, 0x98	@ PACKER: left in the buffer by Resource_Overlay391+0x1a06
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x98	@ PACKER: left in the buffer by Resource_Overlay391+0x1a07
	.endif
	.balign 4, 0

	.global Resource_Overlay39A
Resource_Overlay39A:
	.incbin "overlays/resource_39a.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x5f, 0xb3	@ PACKER: left in the buffer by Resource_Overlay399+0x17a2
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xf4, 0xbe	@ PACKER: left in the buffer by Resource_Overlay399+0x17a2
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x6b, 0x36, 0x69	@ PACKER: left in the buffer by Resource_Overlay399+0x17a1
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xe1, 0x20	@ PACKER: left in the buffer by Resource_Overlay399+0x17a2
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xe1, 0x20	@ PACKER: left in the buffer by Resource_Overlay399+0x17a2
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xc3, 0x41	@ PACKER: left in the buffer by Resource_Overlay399+0x17a2
	.endif
	.balign 4, 0

	.global Resource_Overlay39B
Resource_Overlay39B:
	.incbin "overlays/resource_39b.lz"
	.ifdef TBS_EDITION_EN
	.byte 0x46	@ PACKER: left in the buffer by Resource_Overlay383+0x1ea7
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0xdb	@ PACKER: left in the buffer by Resource_Overlay383+0x1ea7
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x03, 0x38, 0xcd	@ PACKER: left in the buffer by Resource_Overlay383+0x1ea5
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x03, 0x38, 0xcd	@ PACKER: left in the buffer by Resource_Overlay383+0x1ea5
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x03, 0x38, 0xcd	@ PACKER: left in the buffer by Resource_Overlay383+0x1ea5
	.endif
	.balign 4, 0

	.global Resource_Overlay39C
Resource_Overlay39C:
	.incbin "overlays/resource_39c.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xed, 0x93	@ PACKER: left in the buffer by Resource_Data326+0x411e
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x48	@ PACKER: left in the buffer by Resource_Data326+0x4127
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x4e	@ PACKER: left in the buffer by Resource_Data326+0x416b
	.endif
	.balign 4, 0

	.global Resource_Overlay39D
Resource_Overlay39D:
	.incbin "overlays/resource_39d.lz"
	.balign 4, 0

	.global Resource_Overlay39E
Resource_Overlay39E:
	.incbin "overlays/resource_39e.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x5c, 0xba, 0x8b	@ PACKER: left in the buffer by Resource_Overlay39C+0x24dd
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xe1	@ PACKER: left in the buffer by Resource_Overlay39C+0x24df
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x91	@ PACKER: left in the buffer by Resource_Overlay39C+0x24df
	.endif
	.balign 4, 0

	.global Resource_Overlay39F
Resource_Overlay39F:
	.incbin "overlays/resource_39f.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x83, 0x38, 0x51	@ PACKER: left in the buffer by Resource_Overlay39E+0x20e1
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x18, 0x10, 0x06	@ PACKER: left in the buffer by Resource_Overlay39E+0x2111
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x18, 0x10, 0x06	@ PACKER: left in the buffer by Resource_Overlay39E+0x2111
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x18, 0x10, 0xad	@ PACKER: left in the buffer by Resource_Overlay39E+0x2111
	.endif
	.balign 4, 0

	.global Resource_Overlay3A0
Resource_Overlay3A0:
	.incbin "overlays/resource_3a0.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x62	@ PACKER: left in the buffer by Resource_Overlay39F+0x1123
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x88, 0x12	@ PACKER: left in the buffer by Resource_Overlay39F+0x1126
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x79, 0x3c, 0x16	@ PACKER: left in the buffer by Resource_Overlay39F+0x1125
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x3c, 0x16	@ PACKER: left in the buffer by Resource_Overlay39F+0x1126
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x3c, 0x16	@ PACKER: left in the buffer by Resource_Overlay39F+0x1126
	.endif
	.balign 4, 0

	.global Resource_Overlay3A1
Resource_Overlay3A1:
	.incbin "overlays/resource_3a1.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xc0	@ PACKER: left in the buffer by Resource_Overlay3A0+0x78b
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xc6, 0x94, 0x68	@ PACKER: left in the buffer by Resource_Overlay3A0+0x791
	.endif
	.balign 4, 0

	.global Resource_Overlay3A2
Resource_Overlay3A2:
	.incbin "overlays/resource_3a2.lz"
	.ifdef TBS_EDITION_DE
	.byte 0x81, 0xf1	@ PACKER: left in the buffer by Resource_Overlay3A0+0xd92
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xee	@ PACKER: left in the buffer by Resource_Overlay3A0+0xd8f
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xee	@ PACKER: left in the buffer by Resource_Overlay3A0+0xd8f
	.endif
	.balign 4, 0

	.global Resource_Overlay3A3
Resource_Overlay3A3:
	.incbin "overlays/resource_3a3.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xbb, 0xf8, 0x5d	@ PACKER: left in the buffer by Resource_Overlay3A0+0xfe1
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x5c, 0x8f, 0xff	@ PACKER: left in the buffer by Resource_Overlay3A0+0x1001
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x0c, 0xd7, 0xe3	@ PACKER: left in the buffer by Resource_Overlay3A0+0x1001
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xd7	@ PACKER: left in the buffer by Resource_Overlay3A0+0x1003
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xd7	@ PACKER: left in the buffer by Resource_Overlay3A0+0x1003
	.endif
	.balign 4, 0

	.global Resource_Overlay3A4
Resource_Overlay3A4:
	.incbin "overlays/resource_3a4.lz"
	.ifdef TBS_EDITION_ES
	.byte 0xdc, 0x37	@ PACKER: left in the buffer by Resource_Overlay39C+0x29d2
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xdc, 0x37	@ PACKER: left in the buffer by Resource_Overlay39C+0x29d2
	.endif
	.balign 4, 0

	.global Resource_Overlay3A5
Resource_Overlay3A5:
	.incbin "overlays/resource_3a5.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x2e, 0x24	@ PACKER: left in the buffer by Resource_Overlay3A4+0x181e
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xe7, 0x42	@ PACKER: left in the buffer by Resource_Overlay3A4+0x181e
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x91, 0x8c	@ PACKER: left in the buffer by Resource_Overlay3A4+0x181a
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x19	@ PACKER: left in the buffer by Resource_Overlay3A4+0x182b
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x19	@ PACKER: left in the buffer by Resource_Overlay3A4+0x182b
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x80, 0x29	@ PACKER: left in the buffer by Resource_Overlay3A4+0x182e
	.endif
	.balign 4, 0

	.global Resource_Overlay3A6
Resource_Overlay3A6:
	.incbin "overlays/resource_3a6.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x2f	@ PACKER: left in the buffer by Resource_Overlay3A5+0x16ef
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x64	@ PACKER: left in the buffer by Resource_Overlay3A5+0x16f7
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x07	@ PACKER: left in the buffer by Resource_Overlay3A5+0x16f7
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x78	@ PACKER: left in the buffer by Resource_Overlay3A5+0x16f7
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x78	@ PACKER: left in the buffer by Resource_Overlay3A5+0x16f7
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x4c	@ PACKER: left in the buffer by Resource_Overlay3A5+0x16f7
	.endif
	.balign 4, 0

	.global Resource_Overlay3A7
Resource_Overlay3A7:
	.incbin "overlays/resource_3a7.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x3c, 0xbf	@ PACKER: left in the buffer by Resource_Overlay3A6+0x13d2
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x2e	@ PACKER: left in the buffer by Resource_Overlay3A6+0x13d7
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x69, 0x17	@ PACKER: left in the buffer by Resource_Overlay3A6+0x13d6
	.endif
	.balign 4, 0

	.global Resource_Overlay3A8
Resource_Overlay3A8:
	.incbin "overlays/resource_3a8.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x66, 0x05, 0x64	@ PACKER: left in the buffer by Resource_Overlay3A4+0x26bd
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x1c, 0x41, 0x81	@ PACKER: left in the buffer by Resource_Overlay3A4+0x26c5
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x16	@ PACKER: left in the buffer by Resource_Overlay3A4+0x26c3
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x8f, 0xc3, 0x40	@ PACKER: left in the buffer by Resource_Overlay3A4+0x26c5
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x8f, 0xc3, 0x40	@ PACKER: left in the buffer by Resource_Overlay3A4+0x26c5
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x03, 0x71, 0x5c	@ PACKER: left in the buffer by Resource_Overlay3A4+0x26c5
	.endif
	.balign 4, 0

	.global Resource_Overlay3A9
Resource_Overlay3A9:
	.incbin "overlays/resource_3a9.lz"
	.ifdef TBS_EDITION_EN
	.byte 0x14, 0x01, 0x4e	@ PACKER: left in the buffer by Resource_Overlay3A8+0x765
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x69, 0x02, 0x14	@ PACKER: left in the buffer by Resource_Overlay3A8+0x761
	.endif
	.balign 4, 0

	.global Resource_Overlay3AA
Resource_Overlay3AA:
	.incbin "overlays/resource_3aa.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x45	@ PACKER: left in the buffer by Resource_Overlay3A8+0xe5f
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x22	@ PACKER: left in the buffer by Resource_Overlay3A8+0xe5f
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x22, 0x2f, 0xc4	@ PACKER: left in the buffer by Resource_Overlay3A8+0xe5d
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x19, 0x31, 0x22	@ PACKER: left in the buffer by Resource_Overlay3A8+0xe5d
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x19, 0x31, 0x22	@ PACKER: left in the buffer by Resource_Overlay3A8+0xe5d
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x19, 0x31, 0x22	@ PACKER: left in the buffer by Resource_Overlay3A8+0xe5d
	.endif
	.balign 4, 0

	.global Resource_Overlay3AB
Resource_Overlay3AB:
	.incbin "overlays/resource_3ab.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x60, 0x49	@ PACKER: left in the buffer by Resource_Overlay3A8+0x1366
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x01, 0x6c	@ PACKER: left in the buffer by Resource_Overlay3A8+0x1366
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x62	@ PACKER: left in the buffer by Resource_Overlay3A8+0x1367
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x6c	@ PACKER: left in the buffer by Resource_Overlay3A8+0x1367
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x40	@ PACKER: left in the buffer by Resource_Overlay3A8+0x137f
	.endif
	.balign 4, 0

	.global Resource_Overlay3AC
Resource_Overlay3AC:
	.incbin "overlays/resource_3ac.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x83	@ PACKER: left in the buffer by Resource_Overlay3AB+0x583
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x7c, 0xa6, 0x05	@ PACKER: left in the buffer by Resource_Overlay3AB+0x585
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0xf0, 0x99, 0x16	@ PACKER: left in the buffer by Resource_Overlay3AB+0x585
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x7d, 0x41, 0xae	@ PACKER: left in the buffer by Resource_Overlay3AB+0x585
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xf0, 0x99, 0x16	@ PACKER: left in the buffer by Resource_Overlay3AB+0x585
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x7d, 0x41, 0xae	@ PACKER: left in the buffer by Resource_Overlay3AB+0x585
	.endif
	.balign 4, 0

	.global Resource_Overlay3AD
Resource_Overlay3AD:
	.incbin "overlays/resource_3ad.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xfe	@ PACKER: left in the buffer by Resource_Overlay3AB+0xcaf
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x9b	@ PACKER: left in the buffer by Resource_Overlay3AB+0xcaf
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0xa6	@ PACKER: left in the buffer by Resource_Overlay3AB+0xcaf
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x0d	@ PACKER: left in the buffer by Resource_Overlay3AB+0xcaf
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xa6	@ PACKER: left in the buffer by Resource_Overlay3AB+0xcaf
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x0d	@ PACKER: left in the buffer by Resource_Overlay3AB+0xcaf
	.endif
	.balign 4, 0

	.global Resource_Overlay3AE
Resource_Overlay3AE:
	.incbin "overlays/resource_3ae.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xf8	@ PACKER: left in the buffer by Resource_Overlay3AB+0xffb
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xf8, 0xa5	@ PACKER: left in the buffer by Resource_Overlay3AB+0xfee
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0xc0	@ PACKER: left in the buffer by Resource_Overlay3AB+0xfef
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x15, 0x5f, 0x6f	@ PACKER: left in the buffer by Resource_Overlay3AB+0xfed
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xe1, 0x7d, 0xe0	@ PACKER: left in the buffer by Resource_Overlay3AB+0xfed
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x6f, 0x5c	@ PACKER: left in the buffer by Resource_Overlay3AB+0xfee
	.endif
	.balign 4, 0

	.global Resource_Overlay3AF
Resource_Overlay3AF:
	.incbin "overlays/resource_3af.lz"
	.ifdef TBS_EDITION_DE
	.byte 0x92, 0x56	@ PACKER: left in the buffer by Resource_Overlay39C+0x2ace
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x49, 0xe6, 0xe8	@ PACKER: left in the buffer by Resource_Overlay39C+0x2acd
	.endif
	.balign 4, 0

	.global Resource_Overlay3B0
Resource_Overlay3B0:
	.incbin "overlays/resource_3b0.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x02, 0xb9, 0xed	@ PACKER: left in the buffer by Resource_Overlay3AF+0xc81
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x94, 0x27	@ PACKER: left in the buffer by Resource_Overlay3AF+0xc82
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0xbc, 0x47	@ PACKER: left in the buffer by Resource_Overlay3AF+0xc82
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x51, 0xbc	@ PACKER: left in the buffer by Resource_Overlay3AF+0xc82
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x51, 0xbc	@ PACKER: left in the buffer by Resource_Overlay3AF+0xc82
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x47, 0x51, 0xbc	@ PACKER: left in the buffer by Resource_Overlay3AF+0xc81
	.endif
	.balign 4, 0

	.global Resource_Overlay3B1
Resource_Overlay3B1:
	.incbin "overlays/resource_3b1.lz"
	.ifdef TBS_EDITION_DE
	.byte 0xe8, 0xe0, 0x9d	@ PACKER: left in the buffer by Resource_Overlay39C+0x3aad
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x60, 0x07, 0x10	@ PACKER: left in the buffer by Resource_Overlay39C+0x3aad
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x60, 0x07, 0x10	@ PACKER: left in the buffer by Resource_Overlay39C+0x3aad
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x76, 0x00, 0x11	@ PACKER: left in the buffer by Resource_Overlay39C+0x3aad
	.endif
	.balign 4, 0

	.global Resource_Overlay3B2
Resource_Overlay3B2:
	.incbin "overlays/resource_3b2.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x17, 0x00, 0x74	@ PACKER: left in the buffer by Resource_Overlay3B1+0x20d1
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x33, 0xfe, 0x11	@ PACKER: left in the buffer by Resource_Overlay3B1+0x2119
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x18, 0x33, 0xfe	@ PACKER: left in the buffer by Resource_Overlay3B1+0x2119
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x18, 0x33, 0xfe	@ PACKER: left in the buffer by Resource_Overlay3B1+0x2119
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x18, 0x33, 0xfe	@ PACKER: left in the buffer by Resource_Overlay3B1+0x2119
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x18, 0x33, 0xfe	@ PACKER: left in the buffer by Resource_Overlay3B1+0x2119
	.endif
	.balign 4, 0

	.global Resource_Overlay3B3
Resource_Overlay3B3:
	.incbin "overlays/resource_3b3.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x07	@ PACKER: left in the buffer by Resource_Overlay3B1+0x221f
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x11	@ PACKER: left in the buffer by Resource_Overlay3B1+0x22af
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x11	@ PACKER: left in the buffer by Resource_Overlay3B1+0x22af
	.endif
	.balign 4, 0

	.global Resource_Overlay3B4
Resource_Overlay3B4:
	.incbin "overlays/resource_3b4.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xa9, 0xc7, 0xdc	@ PACKER: left in the buffer by Resource_Overlay3B3+0x1a09
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x21	@ PACKER: left in the buffer by Resource_Overlay3B3+0x1a13
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x20, 0x44	@ PACKER: left in the buffer by Resource_Overlay3B3+0x1a12
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x5d, 0xc5, 0xe9	@ PACKER: left in the buffer by Resource_Overlay3B3+0x1a11
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x5d, 0xc5, 0xe9	@ PACKER: left in the buffer by Resource_Overlay3B3+0x1a11
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xe9	@ PACKER: left in the buffer by Resource_Overlay3B3+0x1a13
	.endif
	.balign 4, 0

	.global Resource_Overlay3B5
Resource_Overlay3B5:
	.incbin "overlays/resource_3b5.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x7b, 0xcc, 0x43	@ PACKER: left in the buffer by Resource_Overlay3B4+0xd41
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x99, 0x07, 0x7c	@ PACKER: left in the buffer by Resource_Overlay3B4+0xd41
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x90	@ PACKER: left in the buffer by Resource_Overlay3B4+0xd3f
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x90	@ PACKER: left in the buffer by Resource_Overlay3B4+0xd3f
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x99, 0x07, 0x7c	@ PACKER: left in the buffer by Resource_Overlay3B4+0xd41
	.endif
	.balign 4, 0

	.global Resource_Overlay3B6
Resource_Overlay3B6:
	.incbin "overlays/resource_3b6.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x25	@ PACKER: left in the buffer by Resource_Overlay3B5+0xbdf
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x0b	@ PACKER: left in the buffer by Resource_Overlay3B5+0xbdf
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x22, 0xd8	@ PACKER: left in the buffer by Resource_Overlay3B5+0xbe2
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xf0, 0x43, 0x33	@ PACKER: left in the buffer by Resource_Overlay3B5+0xbf1
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xd8, 0xa9	@ PACKER: left in the buffer by Resource_Overlay3B5+0xbe2
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xa5	@ PACKER: left in the buffer by Resource_Overlay3B5+0xbef
	.endif
	.balign 4, 0

	.global Resource_Overlay3B7
Resource_Overlay3B7:
	.incbin "overlays/resource_3b7.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x05	@ PACKER: left in the buffer by Resource_Overlay3B4+0x128b
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xdf	@ PACKER: left in the buffer by Resource_Overlay3B4+0x1273
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0xd3, 0x6e, 0x15	@ PACKER: left in the buffer by Resource_Overlay3B4+0x1279
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xd3, 0x6e, 0x15	@ PACKER: left in the buffer by Resource_Overlay3B4+0x1279
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xd3, 0x6e, 0x15	@ PACKER: left in the buffer by Resource_Overlay3B4+0x1279
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x6e, 0x15	@ PACKER: left in the buffer by Resource_Overlay3B4+0x127a
	.endif
	.balign 4, 0

	.global Resource_Overlay3B8
Resource_Overlay3B8:
	.incbin "overlays/resource_3b8.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x22, 0x79	@ PACKER: left in the buffer by Resource_Overlay3B3+0x1f52
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xe2	@ PACKER: left in the buffer by Resource_Overlay3B3+0x1f4f
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x3e	@ PACKER: left in the buffer by Resource_Overlay3B3+0x1f53
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x92	@ PACKER: left in the buffer by Resource_Overlay3B3+0x1f5f
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x92	@ PACKER: left in the buffer by Resource_Overlay3B3+0x1f5f
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xa2	@ PACKER: left in the buffer by Resource_Overlay3B3+0x1f5f
	.endif
	.balign 4, 0

	.global Resource_Overlay3B9
Resource_Overlay3B9:
	.incbin "overlays/resource_3b9.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x66	@ PACKER: left in the buffer by Resource_Overlay3B8+0x1aeb
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x8d, 0x10, 0x11	@ PACKER: left in the buffer by Resource_Overlay3B8+0x1ae9
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x90, 0x01, 0x04	@ PACKER: left in the buffer by Resource_Overlay3B8+0x1ae9
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x90, 0x01, 0x04	@ PACKER: left in the buffer by Resource_Overlay3B8+0x1ae9
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x90, 0x01, 0x04	@ PACKER: left in the buffer by Resource_Overlay3B8+0x1ae9
	.endif
	.balign 4, 0

	.global Resource_Overlay3BA
Resource_Overlay3BA:
	.incbin "overlays/resource_3ba.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x68, 0x40	@ PACKER: left in the buffer by Resource_Overlay3B1+0x2ac2
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x68	@ PACKER: left in the buffer by Resource_Overlay3B1+0x2abf
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x3d, 0x4b, 0x1a	@ PACKER: left in the buffer by Resource_Overlay3B1+0x2abd
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xec, 0x80, 0x21	@ PACKER: left in the buffer by Resource_Overlay3B1+0x2ac1
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xcc, 0x4c, 0xcd	@ PACKER: left in the buffer by Resource_Overlay3B1+0x2b2d
	.endif
	.balign 4, 0

	.global Resource_Overlay3BB
Resource_Overlay3BB:
	.incbin "overlays/resource_3bb.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x06, 0x43, 0x38	@ PACKER: left in the buffer by Resource_Overlay3B1+0x2da1
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x06, 0x43	@ PACKER: left in the buffer by Resource_Overlay3B1+0x2d9e
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x06	@ PACKER: left in the buffer by Resource_Overlay3B1+0x2d9f
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x06	@ PACKER: left in the buffer by Resource_Overlay3B1+0x2d9f
	.endif
	.balign 4, 0

	.global Resource_Overlay3BC
Resource_Overlay3BC:
	.incbin "overlays/resource_3bc.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xf4	@ PACKER: left in the buffer by Resource_Overlay3B1+0x34df
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0xf4, 0x02	@ PACKER: left in the buffer by Resource_Overlay3B1+0x34de
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x07, 0xf4, 0x02	@ PACKER: left in the buffer by Resource_Overlay3B1+0x34dd
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x57, 0xec, 0x04	@ PACKER: left in the buffer by Resource_Overlay3B1+0x3551
	.endif
	.balign 4, 0

	.global Resource_Overlay3BD
Resource_Overlay3BD:
	.incbin "overlays/resource_3bd.lz"
	.ifdef TBS_EDITION_EN
	.byte 0x20	@ PACKER: left in the buffer by Resource_Overlay3BC+0x2563
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x08	@ PACKER: left in the buffer by Resource_Overlay3BC+0x2567
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xc9	@ PACKER: left in the buffer by Resource_Overlay3BC+0x2563
	.endif
	.balign 4, 0

	.global Resource_Overlay3BE
Resource_Overlay3BE:
	.incbin "overlays/resource_3be.lz"
	.ifdef TBS_EDITION_EN
	.byte 0x12	@ PACKER: left in the buffer by Resource_Overlay3BD+0x1207
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0xd0, 0x12	@ PACKER: left in the buffer by Resource_Overlay3BD+0x1206
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xc2	@ PACKER: left in the buffer by Resource_Overlay3BD+0x1207
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xc2	@ PACKER: left in the buffer by Resource_Overlay3BD+0x1207
	.endif
	.balign 4, 0

	.global Resource_Overlay3BF
Resource_Overlay3BF:
	.incbin "overlays/resource_3bf.lz"
	.ifdef TBS_EDITION_EN
	.byte 0x19	@ PACKER: left in the buffer by Resource_Overlay3B1+0x39b3
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x80, 0x37, 0x06	@ PACKER: left in the buffer by Resource_Overlay3B1+0x39b5
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x06	@ PACKER: left in the buffer by Resource_Overlay3B1+0x39b7
	.endif
	.balign 4, 0

	.global Resource_Overlay3C0
Resource_Overlay3C0:
	.incbin "overlays/resource_3c0.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x36, 0x3b	@ PACKER: left in the buffer by Resource_Overlay3BF+0xece
	.endif
	.balign 4, 0

	.global Resource_Overlay3C1
Resource_Overlay3C1:
	.incbin "overlays/resource_3c1.lz"
	.balign 4, 0

	.global Resource_Overlay3C2
Resource_Overlay3C2:
	.incbin "overlays/resource_3c2.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xb1, 0x62, 0x41	@ PACKER: left in the buffer by Resource_Overlay3C0+0x7c1
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x76, 0xce	@ PACKER: left in the buffer by Resource_Overlay3C0+0x7c6
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x76, 0xce	@ PACKER: left in the buffer by Resource_Overlay3C0+0x7c6
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x76, 0xce	@ PACKER: left in the buffer by Resource_Overlay3C0+0x7c6
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x76, 0xce	@ PACKER: left in the buffer by Resource_Overlay3C0+0x7c6
	.endif
	.balign 4, 0

	.global Resource_Overlay3C3
Resource_Overlay3C3:
	.incbin "overlays/resource_3c3.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x57, 0x40, 0x18	@ PACKER: left in the buffer by Resource_Overlay3C2+0x735
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xee, 0x9d, 0x02	@ PACKER: left in the buffer by Resource_Overlay3C2+0x735
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x9c, 0x02	@ PACKER: left in the buffer by Resource_Overlay3C2+0x736
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x8b, 0x09, 0xde	@ PACKER: left in the buffer by Resource_Overlay3C2+0x791
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x8b, 0x09, 0xde	@ PACKER: left in the buffer by Resource_Overlay3C2+0x791
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x31, 0xc1, 0x7b	@ PACKER: left in the buffer by Resource_Overlay3C2+0x791
	.endif
	.balign 4, 0

	.global Resource_Overlay3C4
Resource_Overlay3C4:
	.incbin "overlays/resource_3c4.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x6e	@ PACKER: left in the buffer by Resource_Overlay3BF+0x244f
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x48, 0x9c	@ PACKER: left in the buffer by Resource_Overlay3BF+0x2452
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x42, 0x04	@ PACKER: left in the buffer by Resource_Overlay3BF+0x2452
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x42, 0x04	@ PACKER: left in the buffer by Resource_Overlay3BF+0x2452
	.endif
	.balign 4, 0

	.global Resource_Overlay3C5
Resource_Overlay3C5:
	.incbin "overlays/resource_3c5.lz"
	.ifdef TBS_EDITION_DE
	.byte 0xa0, 0x2d	@ PACKER: left in the buffer by Resource_Overlay3C4+0x1ffe
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x45	@ PACKER: left in the buffer by Resource_Overlay3C4+0x1fff
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x45	@ PACKER: left in the buffer by Resource_Overlay3C4+0x1fff
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x52, 0xd0	@ PACKER: left in the buffer by Resource_Overlay3C4+0x1ffe
	.endif
	.balign 4, 0

	.global Resource_Overlay3C6
Resource_Overlay3C6:
	.incbin "overlays/resource_3c6.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xba, 0x80	@ PACKER: left in the buffer by Resource_Overlay3C5+0xd2e
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0xac	@ PACKER: left in the buffer by Resource_Overlay3C5+0xd43
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xac	@ PACKER: left in the buffer by Resource_Overlay3C5+0xd43
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xac	@ PACKER: left in the buffer by Resource_Overlay3C5+0xd43
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xac	@ PACKER: left in the buffer by Resource_Overlay3C5+0xd43
	.endif
	.balign 4, 0

	.global Resource_Overlay3C7
Resource_Overlay3C7:
	.incbin "overlays/resource_3c7.lz"
	.balign 4, 0

	.global Resource_Overlay3C8
Resource_Overlay3C8:
	.incbin "overlays/resource_3c8.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x46	@ PACKER: left in the buffer by Resource_Overlay39C+0x3f47
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xe7, 0x0a, 0xa9	@ PACKER: left in the buffer by Resource_Overlay39C+0x3f4d
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x14	@ PACKER: left in the buffer by Resource_Overlay39C+0x3f6f
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xb8, 0xcc, 0x12	@ PACKER: left in the buffer by Resource_Overlay39C+0x3f75
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xb8, 0xcc, 0x12	@ PACKER: left in the buffer by Resource_Overlay39C+0x3f75
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x97, 0x59, 0x62	@ PACKER: left in the buffer by Resource_Overlay39C+0x3f75
	.endif
	.balign 4, 0

	.global Resource_Overlay3C9
Resource_Overlay3C9:
	.incbin "overlays/resource_3c9.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xfb, 0x9a	@ PACKER: left in the buffer by Resource_Overlay3C8+0x3686
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xbc	@ PACKER: left in the buffer by Resource_Overlay3C8+0x3687
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0xf0, 0x09, 0x3e	@ PACKER: left in the buffer by Resource_Overlay3C8+0x3689
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xf2	@ PACKER: left in the buffer by Resource_Overlay3C8+0x3687
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xf2	@ PACKER: left in the buffer by Resource_Overlay3C8+0x3687
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x3c	@ PACKER: left in the buffer by Resource_Overlay3C8+0x3687
	.endif
	.balign 4, 0

	.global Resource_Overlay3CA
Resource_Overlay3CA:
	.incbin "overlays/resource_3ca.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x87, 0x1e, 0x9e	@ PACKER: left in the buffer by Resource_Overlay3C9+0xfb9
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xb4, 0xcf, 0x87	@ PACKER: left in the buffer by Resource_Overlay3C9+0xfb9
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x18	@ PACKER: left in the buffer by Resource_Overlay3C9+0xfb7
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xb4, 0xcf, 0x87	@ PACKER: left in the buffer by Resource_Overlay3C9+0xfb9
	.endif
	.balign 4, 0

	.global Resource_Overlay3CB
Resource_Overlay3CB:
	.incbin "overlays/resource_3cb.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xa3, 0x1a	@ PACKER: left in the buffer by Resource_Overlay3C9+0x1172
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x8a	@ PACKER: left in the buffer by Resource_Overlay3C9+0x116f
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x20, 0x70	@ PACKER: left in the buffer by Resource_Overlay3C9+0x118e
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xfe, 0x20, 0x70	@ PACKER: left in the buffer by Resource_Overlay3C9+0x118d
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x26, 0x1a	@ PACKER: left in the buffer by Resource_Overlay3C9+0x118a
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x70	@ PACKER: left in the buffer by Resource_Overlay3C9+0x118f
	.endif
	.balign 4, 0

	.global Resource_Overlay3CC
Resource_Overlay3CC:
	.incbin "overlays/resource_3cc.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x51	@ PACKER: left in the buffer by Resource_Overlay3CB+0x107
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x51	@ PACKER: left in the buffer by Resource_Overlay3CB+0x107
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x19	@ PACKER: left in the buffer by Resource_Overlay3CB+0x107
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x19	@ PACKER: left in the buffer by Resource_Overlay3CB+0x107
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x19	@ PACKER: left in the buffer by Resource_Overlay3CB+0x107
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x19	@ PACKER: left in the buffer by Resource_Overlay3CB+0x107
	.endif
	.balign 4, 0

	.global Resource_Overlay3CD
Resource_Overlay3CD:
	.incbin "overlays/resource_3cd.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xa9, 0x51, 0x65	@ PACKER: left in the buffer by Resource_Overlay3CB+0x47d
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x01, 0x30, 0x27	@ PACKER: left in the buffer by Resource_Overlay3CB+0x5fd
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x61	@ PACKER: left in the buffer by Resource_Overlay3CB+0x5ff
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xa8, 0xbd, 0x61	@ PACKER: left in the buffer by Resource_Overlay3CB+0x5fd
	.endif
	.balign 4, 0

	.global Resource_Overlay3CE
Resource_Overlay3CE:
	.incbin "overlays/resource_3ce.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xf1	@ PACKER: left in the buffer by Resource_Overlay3CB+0x6f7
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x3b, 0x07, 0xf2	@ PACKER: left in the buffer by Resource_Overlay3CB+0x799
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xad, 0x62, 0x6c	@ PACKER: left in the buffer by Resource_Overlay3CB+0x7cd
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x2a, 0x7d	@ PACKER: left in the buffer by Resource_Overlay3CB+0x7ca
	.endif
	.balign 4, 0
