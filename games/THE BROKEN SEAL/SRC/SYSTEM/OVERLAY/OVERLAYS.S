@ The overlay streams, each overlay's code and data compressed as one
@ resource and listed in RESOURCE/DIRECTORY.S. Camelot's packer wrote each
@ stream rounded up to four bytes from one reused output buffer, so the
@ bytes after a stream's end are what an earlier stream left in that buffer.
@ They are kept as tagged literal bytes per edition, naming the stream
@ whose bytes they repeat where one does; zero fill is the alignment
@ itself.
	.section .overlays,"a"

	.global Resource_Overlay36F
Resource_Overlay36F:
	.incbin "overlays/resource_36f.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xa0, 0x4f	@ PACKER_GARBAGE: stale buffer bytes, source stream not identified
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xa0, 0x4f	@ PACKER_GARBAGE: stale buffer bytes, source stream not identified
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x31, 0x32, 0x24	@ PACKER_GARBAGE: stale buffer bytes, source stream not identified
	.endif
	.balign 4, 0

	.global Resource_Overlay370
Resource_Overlay370:
	.incbin "overlays/resource_370.lz"
	.ifdef TBS_EDITION_EN
	.byte 0x00, 0x00, 0xdd	@ PACKER_GARBAGE: stale buffer bytes, source stream not identified
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x00, 0xdd	@ PACKER_GARBAGE: stale buffer bytes, source stream not identified
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x08	@ PACKER_GARBAGE: stale buffer bytes, source stream not identified
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x00, 0x00, 0xdd	@ PACKER_GARBAGE: stale buffer bytes, source stream not identified
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x08	@ PACKER_GARBAGE: stale buffer bytes, source stream not identified
	.endif
	.balign 4, 0

	.global Resource_Overlay371
Resource_Overlay371:
	.incbin "overlays/resource_371.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x03, 0x0c, 0xf7	@ PACKER_GARBAGE: stale buffer bytes, source stream not identified
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x03, 0xe6	@ PACKER_GARBAGE: stale buffer bytes, source stream not identified
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xe6, 0x03	@ PACKER_GARBAGE: stale buffer bytes, source stream not identified
	.endif
	.balign 4, 0

	.global Resource_Overlay372
Resource_Overlay372:
	.incbin "overlays/resource_372.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x3f, 0x03	@ PACKER_GARBAGE: resource_371's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x7e, 0x06, 0xc2	@ PACKER_GARBAGE: resource_371's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x8e, 0x0e	@ PACKER_GARBAGE: resource_371's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x8e, 0x0e	@ PACKER_GARBAGE: resource_371's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xe0, 0xbd	@ PACKER_GARBAGE: resource_371's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay373
Resource_Overlay373:
	.incbin "overlays/resource_373.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x6e, 0x2b, 0x82	@ PACKER_GARBAGE: stale buffer bytes, source stream not identified
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x2b, 0x82	@ PACKER_GARBAGE: stale buffer bytes, source stream not identified
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xab, 0x02, 0x3a	@ PACKER_GARBAGE: stale buffer bytes, source stream not identified
	.endif
	.balign 4, 0

	.global Resource_Overlay374
Resource_Overlay374:
	.incbin "overlays/resource_374.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x46, 0xd7, 0xfc	@ PACKER_GARBAGE: resource_373's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x50	@ PACKER_GARBAGE: resource_373's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x68, 0x81, 0x46	@ PACKER_GARBAGE: resource_373's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay375
Resource_Overlay375:
	.incbin "overlays/resource_375.lz"
	.ifdef TBS_EDITION_DE
	.byte 0x04, 0x0a, 0x13	@ PACKER_GARBAGE: resource_374's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x04, 0x0b	@ PACKER_GARBAGE: resource_374's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x78, 0x04	@ PACKER_GARBAGE: resource_374's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xb4, 0x04, 0x0b	@ PACKER_GARBAGE: resource_374's bytes at this offset
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
	.byte 0x42	@ PACKER_GARBAGE: resource_374's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0xed	@ PACKER_GARBAGE: resource_374's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x21	@ PACKER_GARBAGE: resource_374's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay378
Resource_Overlay378:
	.incbin "overlays/resource_378.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x18, 0x48	@ PACKER_GARBAGE: resource_374's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x72	@ PACKER_GARBAGE: resource_374's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xe4, 0x0f	@ PACKER_GARBAGE: resource_374's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x30, 0x30	@ PACKER_GARBAGE: resource_374's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xe4, 0x0f	@ PACKER_GARBAGE: resource_374's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay379
Resource_Overlay379:
	.incbin "overlays/resource_379.lz"
	.ifdef TBS_EDITION_DE
	.byte 0x07	@ PACKER_GARBAGE: resource_378's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x07	@ PACKER_GARBAGE: resource_378's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x07	@ PACKER_GARBAGE: resource_378's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x07	@ PACKER_GARBAGE: resource_378's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay37A
Resource_Overlay37A:
	.incbin "overlays/resource_37a.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x5e, 0x0a, 0x3b	@ PACKER_GARBAGE: resource_378's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x7c	@ PACKER_GARBAGE: resource_378's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x7c, 0x04, 0x64	@ PACKER_GARBAGE: resource_378's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x64, 0x3e	@ PACKER_GARBAGE: resource_378's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x64, 0x3e	@ PACKER_GARBAGE: resource_378's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x64, 0x3e	@ PACKER_GARBAGE: resource_378's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay37B
Resource_Overlay37B:
	.incbin "overlays/resource_37b.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x05, 0x08	@ PACKER_GARBAGE: stale buffer bytes, source stream not identified
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x51, 0x05, 0x08	@ PACKER_GARBAGE: stale buffer bytes, source stream not identified
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x00, 0xb1	@ PACKER_GARBAGE: stale buffer bytes, source stream not identified
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x51, 0x05, 0x08	@ PACKER_GARBAGE: stale buffer bytes, source stream not identified
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x51, 0x05, 0x08	@ PACKER_GARBAGE: stale buffer bytes, source stream not identified
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x51, 0x05, 0x08	@ PACKER_GARBAGE: stale buffer bytes, source stream not identified
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
	.byte 0x10, 0x4e	@ PACKER_GARBAGE: resource_37d's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x10, 0x4e	@ PACKER_GARBAGE: resource_37d's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x10, 0x4e	@ PACKER_GARBAGE: resource_37d's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x10, 0x4e	@ PACKER_GARBAGE: resource_37d's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x10, 0x4e	@ PACKER_GARBAGE: resource_37d's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x10, 0x4e	@ PACKER_GARBAGE: resource_37d's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay37F
Resource_Overlay37F:
	.incbin "overlays/resource_37f.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xdb, 0xd4, 0x6d	@ PACKER_GARBAGE: resource_37b's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0xcb, 0x32, 0x2d	@ PACKER_GARBAGE: resource_37b's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x36, 0x75, 0x9b	@ PACKER_GARBAGE: resource_37b's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x36, 0x75, 0x9b	@ PACKER_GARBAGE: resource_37b's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x36, 0x75, 0x9b	@ PACKER_GARBAGE: resource_37b's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay380
Resource_Overlay380:
	.incbin "overlays/resource_380.lz"
	.ifdef TBS_EDITION_EN
	.byte 0xc5	@ PACKER_GARBAGE: resource_373's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x39, 0x03, 0xc5	@ PACKER_GARBAGE: resource_373's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xd8, 0x65	@ PACKER_GARBAGE: resource_373's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x39, 0x03	@ PACKER_GARBAGE: resource_373's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xd8, 0x65	@ PACKER_GARBAGE: resource_373's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay381
Resource_Overlay381:
	.incbin "overlays/resource_381.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x76, 0x3d, 0x65	@ PACKER_GARBAGE: resource_380's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x75, 0x86	@ PACKER_GARBAGE: resource_380's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xfc, 0x75	@ PACKER_GARBAGE: resource_380's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xfc, 0x75	@ PACKER_GARBAGE: resource_380's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x03	@ PACKER_GARBAGE: resource_380's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay382
Resource_Overlay382:
	.incbin "overlays/resource_382.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x6e, 0x3e, 0x07	@ PACKER_GARBAGE: resource_381's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xa3, 0x6e, 0x3e	@ PACKER_GARBAGE: resource_381's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x06	@ PACKER_GARBAGE: resource_381's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x30	@ PACKER_GARBAGE: resource_381's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x30	@ PACKER_GARBAGE: resource_381's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x30	@ PACKER_GARBAGE: resource_381's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay383
Resource_Overlay383:
	.incbin "overlays/resource_383.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x20	@ PACKER_GARBAGE: resource_373's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x46, 0xe0	@ PACKER_GARBAGE: resource_373's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x01, 0x1e	@ PACKER_GARBAGE: resource_373's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xf2	@ PACKER_GARBAGE: resource_373's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x01	@ PACKER_GARBAGE: resource_373's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xf2	@ PACKER_GARBAGE: resource_373's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay384
Resource_Overlay384:
	.incbin "overlays/resource_384.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x0a, 0xc7	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay385
Resource_Overlay385:
	.incbin "overlays/resource_385.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xb8	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xe0, 0x2d, 0xbc	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x21	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xc0	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xc0	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xe0, 0x2d, 0xbc	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay386
Resource_Overlay386:
	.incbin "overlays/resource_386.lz"
	.ifdef TBS_EDITION_EN
	.byte 0x03	@ PACKER_GARBAGE: resource_385's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0xc6, 0x40, 0x8e	@ PACKER_GARBAGE: resource_385's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xc6, 0x40, 0x8e	@ PACKER_GARBAGE: resource_385's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xc6, 0x40, 0x8e	@ PACKER_GARBAGE: resource_385's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x03	@ PACKER_GARBAGE: resource_385's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay387
Resource_Overlay387:
	.incbin "overlays/resource_387.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x78	@ PACKER_GARBAGE: resource_385's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xc1	@ PACKER_GARBAGE: resource_385's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x0b	@ PACKER_GARBAGE: resource_385's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xc5, 0x2a	@ PACKER_GARBAGE: resource_385's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xc5, 0x2a	@ PACKER_GARBAGE: resource_385's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x04, 0x0d, 0x8a	@ PACKER_GARBAGE: resource_385's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay388
Resource_Overlay388:
	.incbin "overlays/resource_388.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x90, 0x58	@ PACKER_GARBAGE: resource_387's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x90, 0x58	@ PACKER_GARBAGE: resource_387's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x90, 0x58	@ PACKER_GARBAGE: resource_387's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x90, 0x58	@ PACKER_GARBAGE: resource_387's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x90, 0x58	@ PACKER_GARBAGE: resource_387's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x90, 0x58	@ PACKER_GARBAGE: resource_387's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay389
Resource_Overlay389:
	.incbin "overlays/resource_389.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x04	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x0e, 0xe8	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x02	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x02	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay38A
Resource_Overlay38A:
	.incbin "overlays/resource_38a.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x82, 0x07, 0x21	@ PACKER_GARBAGE: resource_389's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x16, 0x56, 0x82	@ PACKER_GARBAGE: resource_389's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x16, 0x56, 0x82	@ PACKER_GARBAGE: resource_389's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x16, 0x56, 0x82	@ PACKER_GARBAGE: resource_389's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x16, 0x56, 0x82	@ PACKER_GARBAGE: resource_389's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x16, 0x56, 0x82	@ PACKER_GARBAGE: resource_389's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay38B
Resource_Overlay38B:
	.incbin "overlays/resource_38b.lz"
	.ifdef TBS_EDITION_DE
	.byte 0x38, 0x03	@ PACKER_GARBAGE: resource_389's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xf0, 0x00, 0xce	@ PACKER_GARBAGE: resource_389's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xf0, 0x00, 0xce	@ PACKER_GARBAGE: resource_389's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x78, 0x00, 0x67	@ PACKER_GARBAGE: resource_389's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay38C
Resource_Overlay38C:
	.incbin "overlays/resource_38c.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x09, 0xf0, 0x9b	@ PACKER_GARBAGE: resource_38b's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x4c, 0x80, 0xdf	@ PACKER_GARBAGE: resource_38b's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay38D
Resource_Overlay38D:
	.incbin "overlays/resource_38d.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x94	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xfd, 0x11, 0x21	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x71, 0xc2, 0x01	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x71, 0xc2, 0x01	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x71, 0xc2, 0x01	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay38E
Resource_Overlay38E:
	.incbin "overlays/resource_38e.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x71	@ PACKER_GARBAGE: resource_38d's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x47	@ PACKER_GARBAGE: resource_38d's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x95, 0xf7	@ PACKER_GARBAGE: resource_38d's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x38, 0x66, 0x10	@ PACKER_GARBAGE: resource_38d's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x38, 0x66, 0x10	@ PACKER_GARBAGE: resource_38d's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x66, 0x10	@ PACKER_GARBAGE: resource_38d's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay38F
Resource_Overlay38F:
	.incbin "overlays/resource_38f.lz"
	.ifdef TBS_EDITION_EN
	.byte 0xb0	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay390
Resource_Overlay390:
	.incbin "overlays/resource_390.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x51, 0x16, 0x40	@ PACKER_GARBAGE: resource_38f's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x16, 0x40	@ PACKER_GARBAGE: resource_38f's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x16, 0x40	@ PACKER_GARBAGE: resource_38f's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x16, 0x40	@ PACKER_GARBAGE: resource_38f's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x16, 0x40	@ PACKER_GARBAGE: resource_38f's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x16, 0x40	@ PACKER_GARBAGE: resource_38f's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay391
Resource_Overlay391:
	.incbin "overlays/resource_391.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x43	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x04, 0x14	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x22, 0x04	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x80, 0x22	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x80, 0x22	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x80, 0x22	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay392
Resource_Overlay392:
	.incbin "overlays/resource_392.lz"
	.ifdef TBS_EDITION_DE
	.byte 0x3c	@ PACKER_GARBAGE: resource_391's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x3c	@ PACKER_GARBAGE: resource_391's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay393
Resource_Overlay393:
	.incbin "overlays/resource_393.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x3c	@ PACKER_GARBAGE: resource_392's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x7c, 0x00	@ PACKER_GARBAGE: resource_392's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x3c	@ PACKER_GARBAGE: resource_391's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x00, 0x3c	@ PACKER_GARBAGE: stale buffer bytes, source stream not identified
	.endif
	.balign 4, 0

	.global Resource_Overlay394
Resource_Overlay394:
	.incbin "overlays/resource_394.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x85	@ PACKER_GARBAGE: resource_393's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x91, 0xf8, 0x00	@ PACKER_GARBAGE: resource_393's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x1f, 0x60	@ PACKER_GARBAGE: resource_393's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x1f, 0x60	@ PACKER_GARBAGE: resource_393's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x3e, 0xc0, 0x1c	@ PACKER_GARBAGE: resource_393's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay395
Resource_Overlay395:
	.incbin "overlays/resource_395.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x78, 0x4d, 0x8a	@ PACKER_GARBAGE: resource_391's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xc1, 0x36, 0xae	@ PACKER_GARBAGE: resource_391's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xc1, 0x36, 0xae	@ PACKER_GARBAGE: resource_391's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay396
Resource_Overlay396:
	.incbin "overlays/resource_396.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x96, 0x28	@ PACKER_GARBAGE: resource_391's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x8c, 0x0e, 0xe2	@ PACKER_GARBAGE: resource_391's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x43, 0x81, 0xd0	@ PACKER_GARBAGE: resource_391's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x43, 0x81, 0xd0	@ PACKER_GARBAGE: resource_391's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay397
Resource_Overlay397:
	.incbin "overlays/resource_397.lz"
	.ifdef TBS_EDITION_DE
	.byte 0xe9	@ PACKER_GARBAGE: resource_396's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay398
Resource_Overlay398:
	.incbin "overlays/resource_398.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x70, 0xa6, 0x69	@ PACKER_GARBAGE: resource_396's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x70, 0xa6, 0x69	@ PACKER_GARBAGE: resource_396's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x70, 0xa6, 0x69	@ PACKER_GARBAGE: resource_396's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay399
Resource_Overlay399:
	.incbin "overlays/resource_399.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xf2, 0xfb	@ PACKER_GARBAGE: resource_391's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x5b	@ PACKER_GARBAGE: resource_391's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x14	@ PACKER_GARBAGE: resource_391's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xf7, 0x98	@ PACKER_GARBAGE: resource_391's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xf7, 0x98	@ PACKER_GARBAGE: resource_391's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x98	@ PACKER_GARBAGE: resource_391's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay39A
Resource_Overlay39A:
	.incbin "overlays/resource_39a.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x5f, 0xb3	@ PACKER_GARBAGE: resource_399's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xf4, 0xbe	@ PACKER_GARBAGE: resource_399's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x6b, 0x36, 0x69	@ PACKER_GARBAGE: resource_399's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xe1, 0x20	@ PACKER_GARBAGE: resource_399's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xe1, 0x20	@ PACKER_GARBAGE: resource_399's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xc3, 0x41	@ PACKER_GARBAGE: resource_399's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay39B
Resource_Overlay39B:
	.incbin "overlays/resource_39b.lz"
	.ifdef TBS_EDITION_EN
	.byte 0x46	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0xdb	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x03, 0x38, 0xcd	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x03, 0x38, 0xcd	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x03, 0x38, 0xcd	@ PACKER_GARBAGE: resource_383's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay39C
Resource_Overlay39C:
	.incbin "overlays/resource_39c.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xed, 0x93	@ PACKER_GARBAGE: stale buffer bytes, source stream not identified
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x48	@ PACKER_GARBAGE: stale buffer bytes, source stream not identified
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x4e	@ PACKER_GARBAGE: stale buffer bytes, source stream not identified
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
	.byte 0x5c, 0xba, 0x8b	@ PACKER_GARBAGE: resource_39c's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xe1	@ PACKER_GARBAGE: resource_39c's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x91	@ PACKER_GARBAGE: resource_39c's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay39F
Resource_Overlay39F:
	.incbin "overlays/resource_39f.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x83, 0x38, 0x51	@ PACKER_GARBAGE: resource_39e's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x18, 0x10, 0x06	@ PACKER_GARBAGE: resource_39e's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x18, 0x10, 0x06	@ PACKER_GARBAGE: resource_39e's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x18, 0x10, 0xad	@ PACKER_GARBAGE: resource_39e's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3A0
Resource_Overlay3A0:
	.incbin "overlays/resource_3a0.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x62	@ PACKER_GARBAGE: resource_39f's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x88, 0x12	@ PACKER_GARBAGE: resource_39f's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x79, 0x3c, 0x16	@ PACKER_GARBAGE: resource_39f's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x3c, 0x16	@ PACKER_GARBAGE: resource_39f's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x3c, 0x16	@ PACKER_GARBAGE: resource_39f's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3A1
Resource_Overlay3A1:
	.incbin "overlays/resource_3a1.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xc0	@ PACKER_GARBAGE: resource_3a0's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xc6, 0x94, 0x68	@ PACKER_GARBAGE: resource_3a0's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3A2
Resource_Overlay3A2:
	.incbin "overlays/resource_3a2.lz"
	.ifdef TBS_EDITION_DE
	.byte 0x81, 0xf1	@ PACKER_GARBAGE: resource_3a0's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xee	@ PACKER_GARBAGE: resource_3a0's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xee	@ PACKER_GARBAGE: resource_3a0's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3A3
Resource_Overlay3A3:
	.incbin "overlays/resource_3a3.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xbb, 0xf8, 0x5d	@ PACKER_GARBAGE: resource_3a0's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x5c, 0x8f, 0xff	@ PACKER_GARBAGE: resource_3a0's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x0c, 0xd7, 0xe3	@ PACKER_GARBAGE: resource_3a0's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xd7	@ PACKER_GARBAGE: resource_3a0's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xd7	@ PACKER_GARBAGE: resource_3a0's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3A4
Resource_Overlay3A4:
	.incbin "overlays/resource_3a4.lz"
	.ifdef TBS_EDITION_ES
	.byte 0xdc, 0x37	@ PACKER_GARBAGE: resource_39c's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xdc, 0x37	@ PACKER_GARBAGE: resource_39c's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3A5
Resource_Overlay3A5:
	.incbin "overlays/resource_3a5.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x2e, 0x24	@ PACKER_GARBAGE: resource_3a4's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xe7, 0x42	@ PACKER_GARBAGE: resource_3a4's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x91, 0x8c	@ PACKER_GARBAGE: resource_3a4's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x19	@ PACKER_GARBAGE: resource_3a4's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x19	@ PACKER_GARBAGE: resource_3a4's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x80, 0x29	@ PACKER_GARBAGE: resource_3a4's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3A6
Resource_Overlay3A6:
	.incbin "overlays/resource_3a6.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x2f	@ PACKER_GARBAGE: resource_3a5's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x64	@ PACKER_GARBAGE: resource_3a5's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x07	@ PACKER_GARBAGE: resource_3a5's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x78	@ PACKER_GARBAGE: resource_3a5's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x78	@ PACKER_GARBAGE: resource_3a5's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x4c	@ PACKER_GARBAGE: resource_3a5's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3A7
Resource_Overlay3A7:
	.incbin "overlays/resource_3a7.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x3c, 0xbf	@ PACKER_GARBAGE: resource_3a6's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x2e	@ PACKER_GARBAGE: resource_3a6's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x69, 0x17	@ PACKER_GARBAGE: resource_3a6's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3A8
Resource_Overlay3A8:
	.incbin "overlays/resource_3a8.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x66, 0x05, 0x64	@ PACKER_GARBAGE: resource_3a4's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x1c, 0x41, 0x81	@ PACKER_GARBAGE: resource_3a4's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x16	@ PACKER_GARBAGE: resource_3a4's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x8f, 0xc3, 0x40	@ PACKER_GARBAGE: resource_3a4's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x8f, 0xc3, 0x40	@ PACKER_GARBAGE: resource_3a4's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x03, 0x71, 0x5c	@ PACKER_GARBAGE: resource_3a4's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3A9
Resource_Overlay3A9:
	.incbin "overlays/resource_3a9.lz"
	.ifdef TBS_EDITION_EN
	.byte 0x14, 0x01, 0x4e	@ PACKER_GARBAGE: resource_3a8's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x69, 0x02, 0x14	@ PACKER_GARBAGE: resource_3a8's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3AA
Resource_Overlay3AA:
	.incbin "overlays/resource_3aa.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x45	@ PACKER_GARBAGE: resource_3a8's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x22	@ PACKER_GARBAGE: resource_3a8's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x22, 0x2f, 0xc4	@ PACKER_GARBAGE: resource_3a8's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x19, 0x31, 0x22	@ PACKER_GARBAGE: resource_3a8's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x19, 0x31, 0x22	@ PACKER_GARBAGE: resource_3a8's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x19, 0x31, 0x22	@ PACKER_GARBAGE: resource_3a8's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3AB
Resource_Overlay3AB:
	.incbin "overlays/resource_3ab.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x60, 0x49	@ PACKER_GARBAGE: resource_3a8's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x01, 0x6c	@ PACKER_GARBAGE: resource_3a8's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x62	@ PACKER_GARBAGE: resource_3a8's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x6c	@ PACKER_GARBAGE: resource_3a8's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x40	@ PACKER_GARBAGE: resource_3a8's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3AC
Resource_Overlay3AC:
	.incbin "overlays/resource_3ac.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x83	@ PACKER_GARBAGE: resource_3ab's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x7c, 0xa6, 0x05	@ PACKER_GARBAGE: resource_3ab's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0xf0, 0x99, 0x16	@ PACKER_GARBAGE: resource_3ab's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x7d, 0x41, 0xae	@ PACKER_GARBAGE: resource_3ab's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xf0, 0x99, 0x16	@ PACKER_GARBAGE: resource_3ab's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x7d, 0x41, 0xae	@ PACKER_GARBAGE: resource_3ab's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3AD
Resource_Overlay3AD:
	.incbin "overlays/resource_3ad.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xfe	@ PACKER_GARBAGE: resource_3ab's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x9b	@ PACKER_GARBAGE: resource_3ab's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0xa6	@ PACKER_GARBAGE: resource_3ab's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x0d	@ PACKER_GARBAGE: resource_3ab's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xa6	@ PACKER_GARBAGE: resource_3ab's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x0d	@ PACKER_GARBAGE: resource_3ab's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3AE
Resource_Overlay3AE:
	.incbin "overlays/resource_3ae.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xf8	@ PACKER_GARBAGE: resource_3ab's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xf8, 0xa5	@ PACKER_GARBAGE: resource_3ab's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0xc0	@ PACKER_GARBAGE: resource_3ab's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x15, 0x5f, 0x6f	@ PACKER_GARBAGE: resource_3ab's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xe1, 0x7d, 0xe0	@ PACKER_GARBAGE: resource_3ab's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x6f, 0x5c	@ PACKER_GARBAGE: resource_3ab's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3AF
Resource_Overlay3AF:
	.incbin "overlays/resource_3af.lz"
	.ifdef TBS_EDITION_DE
	.byte 0x92, 0x56	@ PACKER_GARBAGE: resource_39c's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x49, 0xe6, 0xe8	@ PACKER_GARBAGE: resource_39c's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3B0
Resource_Overlay3B0:
	.incbin "overlays/resource_3b0.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x02, 0xb9, 0xed	@ PACKER_GARBAGE: resource_3af's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x94, 0x27	@ PACKER_GARBAGE: resource_3af's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0xbc, 0x47	@ PACKER_GARBAGE: resource_3af's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x51, 0xbc	@ PACKER_GARBAGE: resource_3af's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x51, 0xbc	@ PACKER_GARBAGE: resource_3af's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x47, 0x51, 0xbc	@ PACKER_GARBAGE: resource_3af's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3B1
Resource_Overlay3B1:
	.incbin "overlays/resource_3b1.lz"
	.ifdef TBS_EDITION_DE
	.byte 0xe8, 0xe0, 0x9d	@ PACKER_GARBAGE: resource_39c's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x60, 0x07, 0x10	@ PACKER_GARBAGE: resource_39c's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x60, 0x07, 0x10	@ PACKER_GARBAGE: resource_39c's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x76, 0x00, 0x11	@ PACKER_GARBAGE: resource_39c's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3B2
Resource_Overlay3B2:
	.incbin "overlays/resource_3b2.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x17, 0x00, 0x74	@ PACKER_GARBAGE: resource_3b1's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x33, 0xfe, 0x11	@ PACKER_GARBAGE: resource_3b1's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x18, 0x33, 0xfe	@ PACKER_GARBAGE: resource_3b1's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x18, 0x33, 0xfe	@ PACKER_GARBAGE: resource_3b1's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x18, 0x33, 0xfe	@ PACKER_GARBAGE: resource_3b1's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x18, 0x33, 0xfe	@ PACKER_GARBAGE: resource_3b1's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3B3
Resource_Overlay3B3:
	.incbin "overlays/resource_3b3.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x07	@ PACKER_GARBAGE: resource_3b1's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x11	@ PACKER_GARBAGE: resource_3b1's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x11	@ PACKER_GARBAGE: resource_3b1's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3B4
Resource_Overlay3B4:
	.incbin "overlays/resource_3b4.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xa9, 0xc7, 0xdc	@ PACKER_GARBAGE: resource_3b3's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x21	@ PACKER_GARBAGE: resource_3b3's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x20, 0x44	@ PACKER_GARBAGE: resource_3b3's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x5d, 0xc5, 0xe9	@ PACKER_GARBAGE: resource_3b3's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x5d, 0xc5, 0xe9	@ PACKER_GARBAGE: resource_3b3's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xe9	@ PACKER_GARBAGE: resource_3b3's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3B5
Resource_Overlay3B5:
	.incbin "overlays/resource_3b5.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x7b, 0xcc, 0x43	@ PACKER_GARBAGE: resource_3b4's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x99, 0x07, 0x7c	@ PACKER_GARBAGE: resource_3b4's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x90	@ PACKER_GARBAGE: resource_3b4's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x90	@ PACKER_GARBAGE: resource_3b4's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x99, 0x07, 0x7c	@ PACKER_GARBAGE: resource_3b4's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3B6
Resource_Overlay3B6:
	.incbin "overlays/resource_3b6.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x25	@ PACKER_GARBAGE: resource_3b5's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x0b	@ PACKER_GARBAGE: resource_3b5's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x22, 0xd8	@ PACKER_GARBAGE: resource_3b5's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xf0, 0x43, 0x33	@ PACKER_GARBAGE: resource_3b5's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xd8, 0xa9	@ PACKER_GARBAGE: resource_3b5's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xa5	@ PACKER_GARBAGE: resource_3b5's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3B7
Resource_Overlay3B7:
	.incbin "overlays/resource_3b7.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x05	@ PACKER_GARBAGE: resource_3b4's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xdf	@ PACKER_GARBAGE: resource_3b4's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0xd3, 0x6e, 0x15	@ PACKER_GARBAGE: resource_3b4's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xd3, 0x6e, 0x15	@ PACKER_GARBAGE: resource_3b4's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xd3, 0x6e, 0x15	@ PACKER_GARBAGE: resource_3b4's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x6e, 0x15	@ PACKER_GARBAGE: resource_3b4's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3B8
Resource_Overlay3B8:
	.incbin "overlays/resource_3b8.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x22, 0x79	@ PACKER_GARBAGE: resource_3b3's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xe2	@ PACKER_GARBAGE: resource_3b3's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x3e	@ PACKER_GARBAGE: resource_3b3's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x92	@ PACKER_GARBAGE: resource_3b3's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x92	@ PACKER_GARBAGE: resource_3b3's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xa2	@ PACKER_GARBAGE: resource_3b3's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3B9
Resource_Overlay3B9:
	.incbin "overlays/resource_3b9.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x66	@ PACKER_GARBAGE: resource_3b8's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x8d, 0x10, 0x11	@ PACKER_GARBAGE: resource_3b8's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x90, 0x01, 0x04	@ PACKER_GARBAGE: resource_3b8's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x90, 0x01, 0x04	@ PACKER_GARBAGE: resource_3b8's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x90, 0x01, 0x04	@ PACKER_GARBAGE: resource_3b8's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3BA
Resource_Overlay3BA:
	.incbin "overlays/resource_3ba.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x68, 0x40	@ PACKER_GARBAGE: resource_3b1's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x68	@ PACKER_GARBAGE: resource_3b1's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x3d, 0x4b, 0x1a	@ PACKER_GARBAGE: resource_3b1's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xec, 0x80, 0x21	@ PACKER_GARBAGE: resource_3b1's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xcc, 0x4c, 0xcd	@ PACKER_GARBAGE: resource_3b1's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3BB
Resource_Overlay3BB:
	.incbin "overlays/resource_3bb.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x06, 0x43, 0x38	@ PACKER_GARBAGE: resource_3b1's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x06, 0x43	@ PACKER_GARBAGE: resource_3b1's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x06	@ PACKER_GARBAGE: resource_3b1's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x06	@ PACKER_GARBAGE: resource_3b1's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3BC
Resource_Overlay3BC:
	.incbin "overlays/resource_3bc.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xf4	@ PACKER_GARBAGE: resource_3b1's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0xf4, 0x02	@ PACKER_GARBAGE: resource_3b1's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x07, 0xf4, 0x02	@ PACKER_GARBAGE: resource_3b1's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x57, 0xec, 0x04	@ PACKER_GARBAGE: resource_3b1's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3BD
Resource_Overlay3BD:
	.incbin "overlays/resource_3bd.lz"
	.ifdef TBS_EDITION_EN
	.byte 0x20	@ PACKER_GARBAGE: resource_3bc's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x08	@ PACKER_GARBAGE: resource_3bc's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xc9	@ PACKER_GARBAGE: resource_3bc's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3BE
Resource_Overlay3BE:
	.incbin "overlays/resource_3be.lz"
	.ifdef TBS_EDITION_EN
	.byte 0x12	@ PACKER_GARBAGE: resource_3bd's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0xd0, 0x12	@ PACKER_GARBAGE: resource_3bd's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xc2	@ PACKER_GARBAGE: resource_3bd's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xc2	@ PACKER_GARBAGE: resource_3bd's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3BF
Resource_Overlay3BF:
	.incbin "overlays/resource_3bf.lz"
	.ifdef TBS_EDITION_EN
	.byte 0x19	@ PACKER_GARBAGE: resource_3b1's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x80, 0x37, 0x06	@ PACKER_GARBAGE: resource_3b1's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x06	@ PACKER_GARBAGE: resource_3b1's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3C0
Resource_Overlay3C0:
	.incbin "overlays/resource_3c0.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x36, 0x3b	@ PACKER_GARBAGE: resource_3bf's bytes at this offset
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
	.byte 0xb1, 0x62, 0x41	@ PACKER_GARBAGE: resource_3c0's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x76, 0xce	@ PACKER_GARBAGE: resource_3c0's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x76, 0xce	@ PACKER_GARBAGE: resource_3c0's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x76, 0xce	@ PACKER_GARBAGE: resource_3c0's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x76, 0xce	@ PACKER_GARBAGE: resource_3c0's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3C3
Resource_Overlay3C3:
	.incbin "overlays/resource_3c3.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x57, 0x40, 0x18	@ PACKER_GARBAGE: resource_3c2's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xee, 0x9d, 0x02	@ PACKER_GARBAGE: resource_3c2's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x9c, 0x02	@ PACKER_GARBAGE: resource_3c2's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x8b, 0x09, 0xde	@ PACKER_GARBAGE: resource_3c2's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x8b, 0x09, 0xde	@ PACKER_GARBAGE: resource_3c2's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x31, 0xc1, 0x7b	@ PACKER_GARBAGE: resource_3c2's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3C4
Resource_Overlay3C4:
	.incbin "overlays/resource_3c4.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x6e	@ PACKER_GARBAGE: resource_3bf's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x48, 0x9c	@ PACKER_GARBAGE: resource_3bf's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x42, 0x04	@ PACKER_GARBAGE: resource_3bf's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x42, 0x04	@ PACKER_GARBAGE: resource_3bf's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3C5
Resource_Overlay3C5:
	.incbin "overlays/resource_3c5.lz"
	.ifdef TBS_EDITION_DE
	.byte 0xa0, 0x2d	@ PACKER_GARBAGE: resource_3c4's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x45	@ PACKER_GARBAGE: resource_3c4's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x45	@ PACKER_GARBAGE: resource_3c4's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x52, 0xd0	@ PACKER_GARBAGE: resource_3c4's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3C6
Resource_Overlay3C6:
	.incbin "overlays/resource_3c6.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xba, 0x80	@ PACKER_GARBAGE: resource_3c5's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0xac	@ PACKER_GARBAGE: resource_3c5's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xac	@ PACKER_GARBAGE: resource_3c5's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xac	@ PACKER_GARBAGE: resource_3c5's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xac	@ PACKER_GARBAGE: resource_3c5's bytes at this offset
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
	.byte 0x46	@ PACKER_GARBAGE: resource_39c's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xe7, 0x0a, 0xa9	@ PACKER_GARBAGE: resource_39c's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x14	@ PACKER_GARBAGE: resource_39c's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xb8, 0xcc, 0x12	@ PACKER_GARBAGE: resource_39c's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xb8, 0xcc, 0x12	@ PACKER_GARBAGE: resource_39c's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x97, 0x59, 0x62	@ PACKER_GARBAGE: resource_39c's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3C9
Resource_Overlay3C9:
	.incbin "overlays/resource_3c9.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xfb, 0x9a	@ PACKER_GARBAGE: resource_3c8's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xbc	@ PACKER_GARBAGE: resource_3c8's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0xf0, 0x09, 0x3e	@ PACKER_GARBAGE: resource_3c8's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xf2	@ PACKER_GARBAGE: resource_3c8's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0xf2	@ PACKER_GARBAGE: resource_3c8's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x3c	@ PACKER_GARBAGE: resource_3c8's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3CA
Resource_Overlay3CA:
	.incbin "overlays/resource_3ca.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x87, 0x1e, 0x9e	@ PACKER_GARBAGE: resource_3c9's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0xb4, 0xcf, 0x87	@ PACKER_GARBAGE: resource_3c9's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x18	@ PACKER_GARBAGE: resource_3c9's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xb4, 0xcf, 0x87	@ PACKER_GARBAGE: resource_3c9's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3CB
Resource_Overlay3CB:
	.incbin "overlays/resource_3cb.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xa3, 0x1a	@ PACKER_GARBAGE: resource_3c9's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x8a	@ PACKER_GARBAGE: resource_3c9's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x20, 0x70	@ PACKER_GARBAGE: resource_3c9's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xfe, 0x20, 0x70	@ PACKER_GARBAGE: resource_3c9's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x26, 0x1a	@ PACKER_GARBAGE: resource_3c9's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x70	@ PACKER_GARBAGE: resource_3c9's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3CC
Resource_Overlay3CC:
	.incbin "overlays/resource_3cc.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x51	@ PACKER_GARBAGE: resource_3cb's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x51	@ PACKER_GARBAGE: resource_3cb's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x19	@ PACKER_GARBAGE: resource_3cb's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x19	@ PACKER_GARBAGE: resource_3cb's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x19	@ PACKER_GARBAGE: resource_3cb's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0x19	@ PACKER_GARBAGE: resource_3cb's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3CD
Resource_Overlay3CD:
	.incbin "overlays/resource_3cd.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xa9, 0x51, 0x65	@ PACKER_GARBAGE: resource_3cb's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x01, 0x30, 0x27	@ PACKER_GARBAGE: resource_3cb's bytes at this offset
	.endif
	.ifdef TBS_EDITION_DE
	.byte 0x61	@ PACKER_GARBAGE: resource_3cb's bytes at this offset
	.endif
	.ifdef TBS_EDITION_IT
	.byte 0xa8, 0xbd, 0x61	@ PACKER_GARBAGE: resource_3cb's bytes at this offset
	.endif
	.balign 4, 0

	.global Resource_Overlay3CE
Resource_Overlay3CE:
	.incbin "overlays/resource_3ce.lz"
	.ifdef TBS_EDITION_JA
	.byte 0xf1	@ PACKER_GARBAGE: resource_3cb's bytes at this offset
	.endif
	.ifdef TBS_EDITION_EN
	.byte 0x3b, 0x07, 0xf2	@ PACKER_GARBAGE: resource_3cb's bytes at this offset
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0xad, 0x62, 0x6c	@ PACKER_GARBAGE: resource_3cb's bytes at this offset
	.endif
	.ifdef TBS_EDITION_FR
	.byte 0x2a, 0x7d	@ PACKER_GARBAGE: resource_3cb's bytes at this offset
	.endif
	.balign 4, 0
