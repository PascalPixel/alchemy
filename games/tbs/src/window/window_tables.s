	.include "games/tbs/include/edition.inc"
	.include "enums/sprite_ids.inc"
	.include "enums/game/character_ids.inc"
	.section .rodata
	.balign 4
	.global WindowSpritePageArrowTiles
WindowSpritePageArrowTiles:
	.incbin "games/tbs/src/window/page_arrow.8bpp8x16"
@ The width of each main font glyph in pixels, from the space (code 32)
	.global FontGlyphWidths
FontGlyphWidths:
	.include "games/tbs/src/graphics/font/glyph_widths.inc"
	.global FontColorPens
FontColorPens:
	.byte 0, 3
	.byte 13, 13
	.byte 14, 4
	.byte 4, 15
	.byte 15, 1
	.byte 1, 13
	.byte 13, 2
	.byte 2, 1
	.global WindowPartyColumnOffsets
WindowPartyColumnOffsets:
	.byte 0, 6, 12, 18, 24, -1, 1, 7, 13, 19, -1
	.ifeq UNUSED_HP_PP_LABEL_TILES_DROPPED
	@ Unused: the HP and PP labels' tile numbers, each with its second tile
	@ and a 0, which nothing reads. The PP label's second tile is 5 where the
	@ label is PV.
	.byte 29, 30, 0, 31
	.ifdef TBS_EDITION_ES
	.byte 5
	.else
	.ifdef TBS_EDITION_IT
	.byte 5
	.else
	.byte 30
	.endif
	.endif
	.byte 0, 0, 0
	.endif
	.byte 0, 0, 0, 1, 2, 3, 3, 2, 1
	.global StatusScreenLevelLetterString
StatusScreenLevelLetterString:
	.ifdef TBS_EDITION_DE
	.asciz "L"
	.balign 4, 0
	.else
	.ifdef TBS_EDITION_ES
	.asciz "Nv."
	.else
	.ifdef TBS_EDITION_FR
	.asciz "Niv"
	.else
	.asciz "L"
	.endif
	.endif
	.endif
@ The ranges of kana (first and last, ending in 0) that take the voiced
@ mark, then those that take the semi-voiced mark
	.global NameEntryVoicedMarkRanges
NameEntryVoicedMarkRanges:
	.byte 182, 196, 202, 206, 150, 159, 224, 228, 234, 238, 147, 147, 179, 179, 0
	.global NameEntrySemivoicedMarkRanges
NameEntrySemivoicedMarkRanges:
	.byte 202, 206, 234, 238, 0
	.global NameEntryCaretWave
NameEntryCaretWave:
	.byte 0, 1, 2, 3, 3, 2, 1, 0
	.ifdef TBS_EDITION_DE
	.global NameEntryDeleteKeyLabel
NameEntryDeleteKeyLabel:
	.asciz "ENT"
	.else
	.ifdef TBS_EDITION_ES
	.global NameEntryDeleteKeyLabel
NameEntryDeleteKeyLabel:
	.asciz "SUP"
	.else
	.ifdef TBS_EDITION_FR
	.global NameEntryDeleteKeyLabel
NameEntryDeleteKeyLabel:
	.asciz "EFF"
	.else
	.endif
	.endif
	.endif
	.ifdef TBS_EDITION_DE
	.global NameEntryEndKeyLabel
NameEntryEndKeyLabel:
	.asciz "ENDE"
	.space 1
	.else
	.ifdef TBS_EDITION_ES
	.global NameEntryEndKeyLabel
NameEntryEndKeyLabel:
	.asciz "FIN"
	.else
	.ifdef TBS_EDITION_FR
	.global NameEntryEndKeyLabel
NameEntryEndKeyLabel:
	.asciz "FIN"
	.else
	.endif
	.endif
	.endif
@ The sprite of each element's Djinni (Venus, Mercury, Mars, Jupiter)
	.global DjinnElementSprites
DjinnElementSprites:
	.2byte SPRITE_VENUS_DJINNI, SPRITE_MERCURY_DJINNI, SPRITE_MARS_DJINNI
	.2byte SPRITE_JUPITER_DJINNI
@ The sprite of each party member; Piers, who has none, keeps his character
@ number, an empty row of the sprites.
	.global CharacterSpriteTable
CharacterSpriteTable:
	.2byte SPRITE_ISAAC, SPRITE_GARET, SPRITE_IVAN, SPRITE_MIA, SPRITE_FELIX
	.2byte SPRITE_JENNA, SPRITE_SHEBA, PIERS
@ The sprite of each party member as a child (while FLAG_CHILDHOOD is set)
	.global CharacterChildSpriteTable
CharacterChildSpriteTable:
	.2byte SPRITE_ISAAC_CHILD, SPRITE_GARET_CHILD, SPRITE_IVAN, SPRITE_MIA
	.2byte SPRITE_FELIX_CHILD, SPRITE_JENNA_CHILD, SPRITE_SHEBA, PIERS
	.global MenuTitleBobOffsets
MenuTitleBobOffsets:
	.ifdef TBS_EDITION_ES
	.byte 0, 0, 1, 2, 3, 3, 2, 1
	.else
	.ifdef TBS_EDITION_FR
	.byte 0, 0, 1, 2, 3, 3, 2, 1
	.else
	.byte 0, 0, 1, 2, 3, 3, 2, 1, 0, 0
	.endif
	.endif
