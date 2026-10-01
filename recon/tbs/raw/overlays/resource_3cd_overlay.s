	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
.syntax unified
	.thumb
	.section .rodata.x020088d0,"a",%progbits
	.global gItemLevelLvLabel
gItemLevelLvLabel:
	.4byte 0x0000764c
	.global gItemLevelItemPrompt
gItemLevelItemPrompt:
	.4byte 0x6d657449
	.4byte 0x3a6f4e20
	.4byte 0x00000000
	.global gItemLevelItemHelp
gItemLevelItemHelp:
	.4byte 0x65473a41
	.4byte 0x74492074
	.4byte 0x20206d65
	.4byte 0x65523a42
	.4byte 0x6e727574
	.4byte 0x00000000
	.global gItemLevelItemFull
gItemLevelItemFull:
	.4byte 0x4d455449
	.4byte 0x4c554620
	.4byte 0x2e2e2e4c
	.4byte 0x2e2e2e2e
	.4byte 0x0000002e
	.if EDITION_INTERNATIONAL
	.global gItemLevelPsyPrompt
gItemLevelPsyPrompt:
	.4byte 0x20797350
	.4byte 0x003a6f4e
	.global gItemLevelPsyHelp
gItemLevelPsyHelp:
	.4byte 0x65523a42
	.4byte 0x6e727574
	.4byte 0x00000000
	.global gItemLevelGlyphsUpper
gItemLevelGlyphsUpper:
	.4byte 0x20422041
	.4byte 0x20442043
	.4byte 0x20462045
	.4byte 0x20482047
	.4byte 0x204b204a
	.4byte 0x204d204c
	.4byte 0x004f204e
	.global gItemLevelGlyphsLower
gItemLevelGlyphsLower:
	.4byte 0x20512050
	.4byte 0x20532052
	.4byte 0x20552054
	.4byte 0x20572056
	.4byte 0x20592058
	.4byte 0x2061205a
	.4byte 0x00632062
	.global gItemLevelGlyphsMarks
gItemLevelGlyphsMarks:
	.4byte 0x203f2021
	.4byte 0x20242023
	.4byte 0x00000025
	.endif
	.global gItemLevelEntrances
gItemLevelEntrances:
	.4byte 0xffff0000
	.4byte 0x00000338
	.4byte 0xc0000360
	.4byte 0x01180000
	.4byte 0x039001f8
	.4byte 0x00000380
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gItemLevelExits
gItemLevelExits:
	.4byte 0x000001ff
	.global gItemLevelPlacements
gItemLevelPlacements:
	.4byte 0xffff01f4
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x03600000
	.4byte 0x00002000
	.4byte 0xffff0020
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x01e00000
	.4byte 0x00002000
	.4byte 0xffff0021
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00002000
	.4byte 0xffff0088
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x03200000
	.ifeq EDITION_INTERNATIONAL
	.4byte 0x00002000
	.else
	.4byte 0x00000000
	.endif
	.if EDITION_INTERNATIONAL
	.4byte 0xffff008f
	.4byte 0x00000001
	.ifndef TBS_EDITION_EN
	.if EDITION_INTERNATIONAL
	.ifndef TBS_EDITION_DE
	.4byte 0x00500000
	.else
	.4byte 0x03500000
	.endif
	.else
	.4byte 0x03500000
	.endif
	.else
	.4byte 0x03500000
	.endif
	.4byte 0x00000000
	.4byte 0x03600000
	.4byte 0x00015000
	.4byte 0xffff0015
	.4byte 0x00000001
	.4byte 0x03500000
	.4byte 0x00000000
	.4byte 0x02b00000
	.4byte 0x00025000
	.4byte 0xffff0015
	.4byte 0x00000001
	.4byte 0x03300000
	.4byte 0x00000000
	.4byte 0x02b00000
	.4byte 0x00025000
	.endif
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gItemLevelEvents
gItemLevelEvents:
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte ItemLevel_SelectItem
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte SceneState_GetFarResult100c
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte SceneState_GetFarResult1020
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte FieldScene_RunCountAdjustPanel
	.if EDITION_INTERNATIONAL
	.ifndef TBS_EDITION_EN
	.if EDITION_INTERNATIONAL
	.ifndef TBS_EDITION_DE
	.else
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte ItemLevel_SelectAbility
	.endif
	.else
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte ItemLevel_SelectAbility
	.endif
	.else
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte ItemLevel_SelectAbility
	.endif
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte FieldScene_RunActor13Mode102Step
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte FieldScene_RunActor13Mode105Step
	.endif
	.4byte 0x00008e15
	.4byte 0xffff000a
	.if EDITION_INTERNATIONAL
	.4byte FieldScene_DrawThreeCaptionWindow
	.else
	.4byte ItemLevel_RunMotionTest
	.endif
	.4byte 0x10008e15
	.4byte 0xffff000a
	.4byte SceneState_SetRecordFlag53
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
