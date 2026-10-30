	.include "enums/sprite_ids.inc"
	.include "enums/game/character_ids.inc"
	.section .rodata
	.global gBattleParty_TurnEndSideOrder
gBattleParty_TurnEndSideOrder:
	.4byte 1, 2
	.byte 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
	.4byte 1 << 16, 0, 0, 0
	.4byte 1 << 16, 0, 0, 0
	.4byte 1 << 16, 0, 0, 0
@ Star workspace positions.
	.global gElementGlow_StarPictureOffsets
gElementGlow_StarPictureOffsets:
	.4byte 0, 1, 5, 14, 30, 46, 62
	.global gElementGlow_StarSizes
gElementGlow_StarSizes:
	.byte 1, 2, 3, 4, 4, 4, 4
	.balign 4
	.global gElementGlow_RingPictureOffsets
gElementGlow_RingPictureOffsets:
	.4byte 78, 1102, 2126, 3150
	.4byte 0, 10, 20, 0
	.global gBattle_UnusedCharacterIdPairs
gBattle_UnusedCharacterIdPairs:
	.2byte SPRITE_ISAAC_CHILD, PORTRAIT_ISAAC_CHILD
	.2byte SPRITE_GARET, PORTRAIT_GARET
	.2byte SPRITE_GARET_CHILD, PORTRAIT_GARET_CHILD
	.2byte SPRITE_IVAN, PORTRAIT_IVAN
	.2byte SPRITE_MIA, PORTRAIT_MIA
	.2byte SPRITE_004, PORTRAIT_FELIX
	.2byte SPRITE_JENNA, PORTRAIT_JENNA
	.2byte SPRITE_SHEBA, PORTRAIT_SHEBA
	.2byte -1, -1
	.global gBattle_UnusedActorKindIdPairs
gBattle_UnusedActorKindIdPairs:
	.2byte SPRITE_JUPITER_DJINNI, 0
	.2byte SPRITE_VENUS_DJINNI, 1
	.2byte SPRITE_MARS_DJINNI, 2
	.2byte SPRITE_MERCURY_DJINNI, 3
	.2byte SPRITE_KRADEN, 4
	.2byte SPRITE_SATUROS, 5
	.2byte SPRITE_MENARDI, 6
	.2byte SPRITE_ALEX, 7
	.2byte SPRITE_DORA, 8
	.2byte SPRITE_KYLE, 9
	.2byte SPRITE_WOMAN_IN_GREEN_HEADSCARF, 10
	.2byte SPRITE_WOMAN_WITH_ORANGE_PONYTAIL, 11
	.2byte SPRITE_BOY_WITH_ORANGE_HAIR, 12
	.2byte SPRITE_MAN_IN_TEAL_SHIRT, 13
	.2byte SPRITE_MCCOY, 14
	.2byte SPRITE_WOMAN_WITH_PURPLE_HAIR, 15
	.2byte SPRITE_TRET_RAISED, 16
	.2byte SPRITE_LAUREL, 17
	.2byte SPRITE_NYUNPA, 18
	.2byte SPRITE_HAMMET, 19
	.2byte SPRITE_LAYANA, 20
	.2byte SPRITE_FEH, 21
	.2byte SPRITE_FEIZHI, 22
	.2byte SPRITE_HSU, 23
	.2byte SPRITE_DONPA, 24
	.2byte SPRITE_DODONPA, 25
	.2byte SPRITE_BABI, 26
	.2byte SPRITE_IODEM, 27
	.2byte SPRITE_HAMA, 28
	.2byte SPRITE_KAJA, 29
	.2byte SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN, 30
	.2byte SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, 31
	.2byte SPRITE_YOUNG_MAN_WITH_RED_HAIR, 32
	.2byte SPRITE_MAN_IN_TALL_HAT, 33
	.2byte SPRITE_BEARDED_MAN_IN_LONG_ROBE, 34
	.2byte SPRITE_BALD_MAN_IN_RED, 35
	.2byte SPRITE_YOUNG_MAN_IN_GREEN, 36
	.2byte SPRITE_WOMAN_WITH_PONYTAIL, 37
	.2byte SPRITE_WOMAN_IN_PINK_APRON, 38
	.2byte SPRITE_MAN_WITH_DARK_HAIR, 39
	.2byte SPRITE_ISAAC_CHILD, 40
	.2byte SPRITE_GARET_CHILD, 41
	.2byte SPRITE_FELIX_CHILD, 42
	.2byte SPRITE_JENNA_CHILD, 43
	.2byte SPRITE_FELIX_MASKED, 44
	.2byte SPRITE_TRET, 45
	.2byte SPRITE_FELIX, 46
	.2byte SPRITE_INVISIBLE_BABI, 47
	.2byte SPRITE_FARAN, 48
	.2byte -1, -1
	.balign 4
	.global gBattleMessage_AdvanceArrowTiles
gBattleMessage_AdvanceArrowTiles:
	.incbin "games/tbs/src/battle/advance_arrow.8bpp8x16"
