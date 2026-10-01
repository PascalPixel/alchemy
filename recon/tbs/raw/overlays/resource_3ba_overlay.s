	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
.syntax unified
	.thumb
	.section .rodata.x0200be44,"a",%progbits
	.global Korosseo_PortraitPaletteOffsets
Korosseo_PortraitPaletteOffsets:
	.4byte 0x20202000
	.2byte 0x4060
	.byte 64
	.ifeq EDITION_INTERNATIONAL
	.byte 64
	.else
	.ifdef TBS_EDITION_EN
	.byte 64
	.else
	.byte 160
	.endif
	.endif
	.2byte 0x0080
	.global KorosseoKawa_ModeRecordTwo
KorosseoKawa_ModeRecordTwo:
	.2byte 0x1000
	.4byte 0x00010200
	.4byte 0x00002000
	.4byte 0x10000001
	.4byte 0x00060100
	.4byte 0x00102000
	.4byte 0x7fff0006
	.4byte 0x2000001e
	.4byte 0x001e0000
	.4byte 0x001e7fff
	.2byte 0xffff
	.global KorosseoKawa_SpanB
KorosseoKawa_SpanB:
	.2byte 0x1000
	.4byte 0x00010080
	.4byte 0x00002000
	.4byte 0x10000001
	.4byte 0x00060100
	.4byte 0x00102000
	.4byte 0x7fff0006
	.4byte 0x1000003c
	.4byte 0x00060180
	.4byte 0x00067fff
	.4byte 0x00f01000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060170
	.4byte 0x00067fff
	.4byte 0x00e01000
	.4byte 0x7fff0006
	.4byte 0x20000006
	.4byte 0x001e0000
	.4byte 0x01601000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000600d0
	.4byte 0x00067fff
	.4byte 0x01501000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000600c0
	.4byte 0x00067fff
	.4byte 0x01401000
	.4byte 0x7fff0006
	.4byte 0xffff0006
	.global Korosseo_GaugePalette
Korosseo_GaugePalette:
	.4byte 0x377f10a0
	.4byte 0x121722bb
	.4byte 0x0ccc1172
	.4byte 0x7df07ef7
	.4byte 0x610b7ce7
	.4byte 0x50007c00
	.4byte 0x00147fff
	.4byte 0x2f1f0000
	.global Korosseo_GaugeGraphics
Korosseo_GaugeGraphics:
	.4byte 0x82fc0100
	.4byte 0x70462310
	.4byte 0x201abddc
	.4byte 0x648ad0cc
	.4byte 0xa8a89c76
	.4byte 0x81984754
	.4byte 0x6138edda
	.4byte 0x5f28474a
	.4byte 0xa01027d3
	.4byte 0xa0abb823
	.4byte 0xf0214faa
	.4byte 0x5557fc29
	.4byte 0x1c29f456
	.4byte 0xc0a8882e
	.4byte 0xf029560c
	.4byte 0x39f9b811
	.4byte 0xa3e78fa8
	.4byte 0xd88e8df8
	.4byte 0x60101ddd
	.4byte 0x2101d56c
	.4byte 0xa68033e0
	.4byte 0x1ca188ea
	.4byte 0xb037924e
	.4byte 0x8e40d46f
	.4byte 0x56a22701
	.4byte 0x9957005d
	.4byte 0x6e51113b
	.4byte 0x9f8022e5
	.4byte 0x3a3ae095
	.4byte 0x39edc9ab
	.4byte 0x9ddeeaec
	.4byte 0x378733aa
	.4byte 0xbb23a0e8
	.4byte 0x7b4bb0f7
	.4byte 0x0596cc80
	.4byte 0xc075eccf
	.4byte 0x3fd9b0ef
	.4byte 0xcc032ec8
	.4byte 0x8d5aec81
	.4byte 0xee419f03
	.4byte 0x7c20f81f
	.4byte 0x38be6e09
	.4byte 0xbe2a7c41
	.4byte 0x07316774
	.4byte 0xf7104b82
	.4byte 0xf17c90f8
	.4byte 0x00000001
	.global HexDigits
HexDigits:
	.4byte 0x33323130
	.4byte 0x37363534
	.4byte 0x42413938
	.4byte 0x46454443
	.4byte 0x00000000
	.global KorosseoKawa_ScriptA
KorosseoKawa_ScriptA:
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xffffc000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x0000001b
	.global KorosseoKawa_ScriptB
KorosseoKawa_ScriptB:
	.4byte 0x00000015
	.4byte 0x00000026
	.4byte SceneEffect_SpawnKind285AtRandomChance
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00016000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00016000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00012000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00012000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00016000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00016000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00012000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00012000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0xc0010000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000010
	.global KorosseoKawa_DirectionSteps
KorosseoKawa_DirectionSteps:
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x00100000
	.global KorosseoKawa_SceneTableA
KorosseoKawa_SceneTableA:
	.4byte 0xffff0000
	.4byte 0x00000398
	.4byte 0x400000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000138
	.4byte 0x40000068
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global KorosseoKawa_SceneTableB
KorosseoKawa_SceneTableB:
	.4byte 0x0000008f
	.4byte 0x00a0108f
	.4byte 0x00b0c08c
	.4byte 0x0041508c
	.4byte 0x0056208a
	.4byte 0x000001ff
	.global KorosseoKawa_SceneTableC
KorosseoKawa_SceneTableC:
	.4byte 0xffff0000
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff003e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff00e5
	.4byte 0x00000007
	.4byte 0x02590000
	.4byte 0x00000000
	.4byte 0x00920000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000007
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x00c20000
	.4byte 0x00024000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x03b00000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00028000
	.4byte 0xffff00ec
	.4byte 0x00000007
	.4byte 0x04180000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00024000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x03f80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00014000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x04380000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x05180000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x05180000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global KorosseoKawa_Countdown
KorosseoKawa_Countdown:
	.4byte 0x00000000
	.global KorosseoKawa_SceneTableD
KorosseoKawa_SceneTableD:
	.4byte 0x00000202
	.4byte 0xffff005a
	.4byte FieldScene_RunBranchedStep
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte FieldScene_RunTwoCallSequence
	.4byte 0x00000202
	.4byte 0xffff000b
	.4byte FieldScene_RunTwoCallSequence
	.4byte 0x00008602
	.4byte 0x0302000c
	.4byte KorosseoKawa_RaisePipes
	.4byte 0x0000c602
	.4byte 0xffff0014
	.4byte SceneActor_ShiftActorSeventeenByLeaderRow
	.4byte 0x00004602
	.4byte 0xffff0014
	.4byte SceneActor_ShiftActorSeventeenByLeaderRow
	.4byte 0x00008602
	.4byte 0xffff0015
	.4byte SceneActor_ShiftActorEighteenByInputAndLeaderColumn
	.4byte 0x00000602
	.4byte 0xffff0015
	.4byte SceneActor_ShiftActorEighteenByInputAndLeaderColumn
	.4byte 0x00000002
	.4byte 0x02100032
	.4byte KorosseoKawa_RunStageStart
	.4byte 0x00000013
	.4byte 0x03080064
	.4byte 0x001000b5
	.4byte 0x00000013
	.4byte 0x03090065
	.4byte 0x001000ee
	.4byte 0x00008413
	.4byte 0x030a0067
	.4byte 0x001000b5
	.4byte 0x00008c15
	.4byte 0xffff0009
	.4byte SceneState_ApplyRectsForActorsNineAndTen
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte SceneState_ApplyRectsForActorsNineAndTen
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte SceneState_ApplyRectsForActorsNineAndTen
	.4byte 0x00000c15
	.4byte 0x0303000f
	.4byte SceneState_ApplyRectAndSend303
	.4byte 0x00002115
	.4byte 0x0301000d
	.4byte FieldScene_RunScene3ba_02000270
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte RunPartyCountInteraction
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte KorosseoKawa_RunStageIntro
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte Korosseo_RunPipeworksIntro
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte FieldScene_RunScene3ba_020015e0
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte Scene_RunSceneFourCoordinator
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgKorosseoKawaThisYearsFinalsAreIncredibleThis
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgKorosseoKawaMovingSuchAHugeRockMust
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgKorosseoKawaTheresOnlyOneFinalAndThis
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgKorosseoKawaDidYouSeeHisMovesOut
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte MsgKorosseoKawaHowManyWarriorsHaveSwallowedTheir
	.4byte 0x00000006
	.4byte 0xffff0063
	.4byte FieldScene_RunNearestActor165Scene
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Korosseo_PortraitSlot
Korosseo_PortraitSlot:
	.2byte 0xffff
	.global KorosseoKawa_RoundSpans
KorosseoKawa_RoundSpans:
	.2byte 0x4000
	.4byte 0x0800ff44
	.2byte 0x1000
	.ifeq EDITION_INTERNATIONAL
	.2byte 0x0100
	.else
	.2byte 0x0180
	.endif
	.4byte 0x20000001
	.4byte 0x00010010
	.4byte 0x000e7fff
	.4byte 0x00003000
	.4byte 0x7fff0014
	.4byte 0x3000003c
	.4byte 0x00140800
	.4byte 0x003c7fff
	.2byte 0xffff
	.global KorosseoKawa_ModeRecordFour
KorosseoKawa_ModeRecordFour:
	.2byte 0x1000
	.ifdef TBS_EDITION_ES
	.2byte 0x0180
	.else
	.ifdef TBS_EDITION_IT
	.2byte 0x0180
	.else
	.2byte 0x0200
	.endif
	.endif
	.2byte 1
	.4byte 0x00002000
	.4byte 0x10000001
	.ifdef TBS_EDITION_FR
	.4byte 0x00060180
	.else
	.4byte 0x00060100
	.endif
	.4byte 0x00102000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.ifdef TBS_EDITION_FR
	.4byte 0x00060200
	.else
	.4byte 0x00060180
	.endif
	.4byte 0x00067fff
	.ifdef TBS_EDITION_FR
	.4byte 0x01801000
	.else
	.4byte 0x01001000
	.endif
	.4byte 0x7fff0006
	.4byte 0x10000006
	.ifdef TBS_EDITION_FR
	.4byte 0x00060200
	.else
	.4byte 0x00060180
	.endif
	.4byte 0x00067fff
	.ifdef TBS_EDITION_FR
	.4byte 0x01801000
	.else
	.4byte 0x01001000
	.endif
	.4byte 0x7fff0006
	.4byte 0x20000006
	.4byte 0x001e0000
	.ifdef TBS_EDITION_FR
	.4byte 0x02001000
	.else
	.4byte 0x01801000
	.endif
	.4byte 0x7fff0006
	.4byte 0x10000006
	.ifdef TBS_EDITION_FR
	.4byte 0x00060180
	.else
	.4byte 0x00060100
	.endif
	.4byte 0x00067fff
	.ifdef TBS_EDITION_FR
	.4byte 0x02001000
	.else
	.4byte 0x01801000
	.endif
	.4byte 0x7fff0006
	.4byte 0x10000006
	.ifdef TBS_EDITION_FR
	.4byte 0x00060180
	.else
	.4byte 0x00060100
	.endif
	.4byte 0x00067fff
	.ifdef TBS_EDITION_FR
	.4byte 0x02001000
	.else
	.4byte 0x01801000
	.endif
	.4byte 0x7fff0006
	.4byte 0xffff0006
	.global KorosseoKawa_SpanA
KorosseoKawa_SpanA:
	.4byte 0x00801000
	.4byte 0x20000001
	.4byte 0x00010000
	.4byte 0x01001000
	.4byte 0x20000006
	.4byte 0x00060010
	.4byte 0x003c7fff
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060110
	.4byte 0x00067fff
	.4byte 0x01901000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060120
	.4byte 0x00067fff
	.4byte 0x00002000
	.4byte 0x1000001e
	.4byte 0x000601a0
	.4byte 0x00067fff
	.4byte 0x01301000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000601b0
	.4byte 0x00067fff
	.4byte 0x01401000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000601c0
	.4byte 0x00067fff
	.2byte 0xffff
	.global Korosseo_MarkerSlot
Korosseo_MarkerSlot:
	.2byte 0xffff
	.global Korosseo_RivalFinishScript
Korosseo_RivalFinishScript:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000022
	.4byte SceneActor_PlaceLinkedActorAbove
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x0000001c
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000096
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global KorosseoKawa_ApproachScript
KorosseoKawa_ApproachScript:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000022
	.4byte SceneActor_PlaceLinkedActorAbove
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x0000001c
	.4byte 0x0000001c
	.4byte 0x00000000
	.4byte 0x00000096
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.section .bss,"aw",%nobits
	.global Korosseo_MarkerStep
Korosseo_MarkerStep:
	.space 4
	.global Korosseo_ModeScaleDuration
Korosseo_ModeScaleDuration:
	.space 4
	.global Korosseo_MarkerPriority
Korosseo_MarkerPriority:
	.space 4
	.global Korosseo_CompetitorStartZ
Korosseo_CompetitorStartZ:
	.space 4
	.global Korosseo_MarkerEndX
Korosseo_MarkerEndX:
	.space 4
	.global Korosseo_ModeTaskParam
Korosseo_ModeTaskParam:
	.space 4
	.global Korosseo_ModeScaleStart
Korosseo_ModeScaleStart:
	.space 4
	.global Korosseo_ModeMoveDuration
Korosseo_ModeMoveDuration:
	.space 4
	.global Korosseo_ModeTaskPosition
Korosseo_ModeTaskPosition:
	.space 4
	.global Korosseo_MarkerBlink
Korosseo_MarkerBlink:
	.space 4
	.global Korosseo_ModeScaleTarget
Korosseo_ModeScaleTarget:
	.space 4
	.global Korosseo_ModeMoveStep
Korosseo_ModeMoveStep:
	.space 4
	.global Korosseo_MarkerY
Korosseo_MarkerY:
	.space 4
	.global Korosseo_ModeBlendStep
Korosseo_ModeBlendStep:
	.space 4
	.global Korosseo_CompetitorStartAngle
Korosseo_CompetitorStartAngle:
	.space 4
	.global Korosseo_MarkerSteps
Korosseo_MarkerSteps:
	.space 4
	.global Korosseo_ModeTaskMode
Korosseo_ModeTaskMode:
	.space 4
	.global Korosseo_ModeBlendTarget
Korosseo_ModeBlendTarget:
	.space 4
	.global Korosseo_ModeBlendStart
Korosseo_ModeBlendStart:
	.space 4
	.global Korosseo_ModeTaskTimer
Korosseo_ModeTaskTimer:
	.space 4
	.global Korosseo_ModeTaskScript
Korosseo_ModeTaskScript:
	.space 4
	.global Korosseo_MarkerStartX
Korosseo_MarkerStartX:
	.space 4
	.global Korosseo_ModeBlendDuration
Korosseo_ModeBlendDuration:
	.space 8
	.global Korosseo_MarkerOam
Korosseo_MarkerOam:
	.space 12
	.global Korosseo_MarkerStartY
Korosseo_MarkerStartY:
	.space 4
	.global Korosseo_ModeTaskSprites
Korosseo_ModeTaskSprites:
	.space 48
	.global Korosseo_ModeMoveStart
Korosseo_ModeMoveStart:
	.space 4
	.global Korosseo_MarkerX
Korosseo_MarkerX:
	.space 4
	.global Korosseo_ModeMoveTarget
Korosseo_ModeMoveTarget:
	.space 4
	.global Korosseo_ModeScaleStep
Korosseo_ModeScaleStep:
	.space 4
	.global Korosseo_MarkerEndY
Korosseo_MarkerEndY:
	.space 4
	.global Korosseo_CompetitorStartX
Korosseo_CompetitorStartX:
	.space 4
