	.include "games/tbs/include/edition.inc"
	.include "enums/sprite_ids.inc"
	.include "enums/game/character_ids.inc"
	.section .rodata
	.balign 4
	.global WindowSprite_PageArrowTiles
WindowSprite_PageArrowTiles:
	.incbin "games/tbs/src/window/page_arrow.8bpp8x16"
@ The width of each main font glyph in pixels, from the space (code 32)
	.global Font_GlyphWidths
Font_GlyphWidths:
	.include "games/tbs/src/graphics/font/glyph_widths.inc"
	.global Font_ColorPens
Font_ColorPens:
	.byte 0, 3
	.byte 13, 13
	.byte 14, 4
	.byte 4, 15
	.byte 15, 1
	.byte 1, 13
	.byte 13, 2
	.byte 2, 1
	.global Window_PartyColumnOffsets
Window_PartyColumnOffsets:
	.ifdef TBS_EDITION_ES
	.byte 0, 6, 12, 18, 24, -1, 1, 7, 13, 19, -1, 29, 30, 0
	.byte 31, 5, 0, 0, 0, 0, 0, 0, 1, 2, 3, 3, 2, 1
	.else
	.ifdef TBS_EDITION_FR
	.byte 0, 6, 12, 18, 24, -1, 1, 7, 13, 19, -1, 0, 0, 0
	.byte 1, 2, 3, 3, 2, 1
	.else
	.ifdef TBS_EDITION_IT
	.byte 0, 6, 12, 18, 24, -1, 1, 7, 13, 19, -1, 29, 30, 0
	.byte 31, 5, 0, 0, 0, 0, 0, 0, 1, 2, 3, 3, 2, 1
	.else
	.byte 0, 6, 12, 18, 24, -1, 1, 7, 13, 19, -1, 29, 30, 0
	.byte 31, 30, 0, 0, 0, 0, 0, 0, 1, 2, 3, 3, 2, 1
	.endif
	.endif
	.endif
	.global StatusScreen_LevelLetterString
StatusScreen_LevelLetterString:
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
	.global NameEntry_VoicedMarkRanges
NameEntry_VoicedMarkRanges:
	.byte 182, 196, 202, 206, 150, 159, 224, 228, 234, 238, 147, 147, 179, 179, 0
	.global NameEntry_SemivoicedMarkRanges
NameEntry_SemivoicedMarkRanges:
	.byte 202, 206, 234, 238, 0
	.global NameEntry_CaretWave
NameEntry_CaretWave:
	.byte 0, 1, 2, 3, 3, 2, 1, 0
	.ifdef TBS_EDITION_DE
	.global NameEntry_DeleteKeyLabel
NameEntry_DeleteKeyLabel:
	.asciz "ENT"
	.else
	.ifdef TBS_EDITION_ES
	.global NameEntry_DeleteKeyLabel
NameEntry_DeleteKeyLabel:
	.asciz "SUP"
	.else
	.ifdef TBS_EDITION_FR
	.global NameEntry_DeleteKeyLabel
NameEntry_DeleteKeyLabel:
	.asciz "EFF"
	.else
	.endif
	.endif
	.endif
	.ifdef TBS_EDITION_DE
	.global NameEntry_EndKeyLabel
NameEntry_EndKeyLabel:
	.asciz "ENDE"
	.space 1
	.else
	.ifdef TBS_EDITION_ES
	.global NameEntry_EndKeyLabel
NameEntry_EndKeyLabel:
	.asciz "FIN"
	.else
	.ifdef TBS_EDITION_FR
	.global NameEntry_EndKeyLabel
NameEntry_EndKeyLabel:
	.asciz "FIN"
	.else
	.endif
	.endif
	.endif
@ The sprite of each element's Djinni (Venus, Mercury, Mars, Jupiter)
	.global Djinn_ElementSprites
Djinn_ElementSprites:
	.2byte SPRITE_VENUS_DJINNI, SPRITE_MERCURY_DJINNI, SPRITE_MARS_DJINNI
	.2byte SPRITE_JUPITER_DJINNI
@ The sprite of each party member; Piers, who has none, keeps his character
@ number, an empty row of the sprites.
	.global Character_SpriteTable
Character_SpriteTable:
	.2byte SPRITE_ISAAC, SPRITE_GARET, SPRITE_IVAN, SPRITE_MIA, SPRITE_FELIX
	.2byte SPRITE_JENNA, SPRITE_SHEBA, PIERS
@ The sprite of each party member as a child (while FLAG_CHILDHOOD is set)
	.global Character_ChildSpriteTable
Character_ChildSpriteTable:
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
