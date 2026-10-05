.syntax unified
	.thumb
	.section .rodata.x0200911c,"a",%progbits
	.global KorimaMagari_DefaultRecords
KorimaMagari_DefaultRecords:
	.4byte 0x000b00cd
	.4byte 0x00010009
	.4byte 0x00000000
	.4byte 0x000c00cf
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0x001100cf
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0x000d00cd
	.4byte 0x0001000d
	.4byte 0x00000000
	.4byte 0x000a00cf
	.4byte 0x00000012
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global KorimaMagari_PushAnimations
KorimaMagari_PushAnimations:
	.4byte 0x03030202
	.global KorimaMagari_PushStepX
KorimaMagari_PushStepX:
	.4byte 0x00f80008
	.global KorimaMagari_PushStepZ
KorimaMagari_PushStepZ:
	.4byte 0xf8000800
	.global gKorimaMagariEntrances
gKorimaMagariEntrances:
	.4byte 0xffff0000
	.4byte 0x00000197
	.4byte 0x80000114
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00200032
	.4byte 0x00000171
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x002000a2
	.4byte 0x40000054
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKorimaMagariExits
gKorimaMagariExits:
	.4byte 0x0000002b
	.4byte 0x0010202a
	.4byte 0x0020102c
	.4byte 0x000001ff
	.global gKorimaMagariPlacements
gKorimaMagariPlacements:
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00024000
	.4byte 0xffff00d6
	.4byte 0x00000007
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x00024000
	.4byte 0xffff0112
	.4byte 0x00000001
	.4byte 0x015c0000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKorimaMagariEvents
gKorimaMagariEvents:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00004602
	.4byte 0xffff000a
	.4byte KorimaMagari_TryJumpForward
	.4byte 0x00000202
	.4byte 0xffff000b
	.4byte KorimaMagari_RedrawBoard
	.4byte 0x00008602
	.4byte 0xffff000d
	.4byte KorimaMagari_RunReturnSequence
	.4byte 0x00000602
	.4byte 0xffff000c
	.4byte KorimaMagari_RunDepartSequence
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgKorimaDoNotTouchFloodgate
	.4byte 0x00000003
	.4byte 0xffff0009
	.4byte Scene_RunKorimaMagariSequence
	.4byte 0x00000013
	.4byte 0x0f000064
	.4byte 0x0010005b
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.section .bss,"aw",%nobits
	.space 8
	.global gKorimaMagariRecords
gKorimaMagariRecords:
	.space 4
	.global gKorimaMagariReturned
gKorimaMagariReturned:
	.space 4
	.global gKorimaMagariLayout
gKorimaMagariLayout:
	.space 8
	.global KorimaPalette_First
KorimaPalette_First:
	.space 1792
	.global KorimaPalette_Second
KorimaPalette_Second:
	.space 896
	.space 896
	.global KorimaMagari_ShakeScroll
KorimaMagari_ShakeScroll:
	.space 12
	.global KorimaMagari_ShakeChance
KorimaMagari_ShakeChance:
	.space 4
