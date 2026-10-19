	.include "games/tla/include/game/ed_asm.h"
	.section .rodata
	.global gUiText_GlyphWidths
gUiText_GlyphWidths:
	.include "games/tla/src/graphics/window/glyph_widths.inc"
@ {background, foreground} pens of the eight text colours
	.global gUiText_ColourPens
gUiText_ColourPens:
	.byte 0, 3
	.byte 13, 13
	.byte 14, 15
	.byte 15, 15
	.byte 15, 1
	.byte 1, 13
	.byte 13, 2
	.byte 2, 1
	.global gUiWindow_PartyColumnOffsets
gUiWindow_PartyColumnOffsets:
	.byte 0, 6, 12, 18, 24, -1, 1, 7, 13, 19, -1
	.ifndef TLA_EDITION_FR
	.ifdef TLA_EDITION_ES
	.byte 29, 30, 0, 31, 5, 0
	.else
	.ifdef TLA_EDITION_IT
	.byte 29, 30, 0, 31, 5, 0
	.else
	.byte 29, 30, 0, 31, 30, 0
	.endif
	.endif
	.endif
@ The progress bar's eight tiles, then its seven rows of twelve, by step
	.global gUi_ProgressBarTiles
gUi_ProgressBarTiles:
	.incbin "games/tla/src/graphics/window/progress.4bpp.lz"
	.global gUi_ProgressBarRows
gUi_ProgressBarRows:
	.byte 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2
	.byte 3, 5, 4, 1, 1, 1, 1, 1, 1, 1, 1, 2
	.byte 3, 5, 5, 5, 4, 1, 1, 1, 1, 1, 1, 2
	.byte 3, 5, 5, 5, 5, 5, 4, 1, 1, 1, 1, 2
	.byte 3, 5, 5, 5, 5, 5, 5, 5, 4, 1, 1, 2
	.byte 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 2
	.byte 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7
	.balign 4, 0
	.global gStatusMenu_LevelLetterString
gStatusMenu_LevelLetterString:
	.ifdef TLA_EDITION_DE
	.asciz "L"
	.balign 4, 0
	.else
	.ifdef TLA_EDITION_ES
	.asciz "Nv."
	.else
	.ifdef TLA_EDITION_FR
	.asciz "Niv"
	.else
	.asciz "L"
	.endif
	.endif
	.endif
@ The kana that take a dakuten or handakuten, as ranges
	.global gNameEntry_VoicedMarkRanges
gNameEntry_VoicedMarkRanges:
	.byte 0xb6, 0xc4, 0xca, 0xce, 0x96, 0x9f, 0xe0, 0xe4, 0xea, 0xee, 0x93, 0x93, 0xb3, 0xb3, 0x00
	.global gNameEntry_SemivoicedMarkRanges
gNameEntry_SemivoicedMarkRanges:
	.byte 0xca, 0xce, 0xea, 0xee, 0x00
	.global gNameEntry_CaretWave
gNameEntry_CaretWave:
	.byte 0, 1, 2, 3, 3, 2, 1, 0
	.ifdef TLA_EDITION_DE
	.global gNameEntry_DeleteKeyLabel
gNameEntry_DeleteKeyLabel:
	.asciz "ENT"
	.global gNameEntry_EndKeyLabel
gNameEntry_EndKeyLabel:
	.asciz "ENDE"
	.space 1
	.endif
	.ifdef TLA_EDITION_ES
	.global gNameEntry_DeleteKeyLabel
gNameEntry_DeleteKeyLabel:
	.asciz "SUP"
	.global gNameEntry_EndKeyLabel
gNameEntry_EndKeyLabel:
	.asciz "FIN"
	.endif
	.ifdef TLA_EDITION_FR
	.global gNameEntry_DeleteKeyLabel
gNameEntry_DeleteKeyLabel:
	.asciz "EFF"
	.global gNameEntry_EndKeyLabel
gNameEntry_EndKeyLabel:
	.asciz "FIN"
	.endif
@ The Djinn sprite of each element
	.global gDjinn_ElementSprites
gDjinn_ElementSprites:
	.2byte 0xf3, 0xf4, 0xf5, 0xf6
@ Each party member's sprite
	.global gCharacter_SpriteTable
gCharacter_SpriteTable:
	.2byte 0, 1, 2, 3, 4, 5, 6, 7
@ Values shown by party message 0x32, indexed by the low five bits
	.global Data_0805f676
Data_0805f676:
	.2byte 0x17c, 0x184, 0x18c, 0x196, 0x17d, 0x185, 0x18d, 0x197
	.2byte 0x17e, 0x186, 0x18e, 0x198, 0x17f, 0x187, 0x18f, 0x199
	.2byte 0x180, 0x190, 0x19a, 0x188, 0x191, 0x181, 0x19b, 0x189
	.2byte 0x192, 0x18a, 0x19c, 0x182, 0x194, 0, 0, 0
	.byte 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3
	.byte 0, 2, 3, 1, 2, 0, 3, 1, 2, 1, 3, 0, 2, 0, 0, 0
	.global gUi_PairBobOffsets
gUi_PairBobOffsets:
	.ifdef TLA_EDITION_ES
	.byte 0, 0, 1, 2, 3, 3, 2, 1
	.else
	.ifdef TLA_EDITION_FR
	.byte 0, 0, 1, 2, 3, 3, 2, 1
	.else
	.byte 0, 0, 1, 2, 3, 3, 2, 1, 0, 0
	.endif
	.endif
	.balign 4
	.global gUi_ObjectPulseScales
gUi_ObjectPulseScales:
	.4byte 0x11999, 0x12147, 0x128f5, 0x130a3
	.4byte 0x13851, 0x130a3, 0x128f5, 0x12147
@ 4bpp rows in colour 5 that grow by one pixel at a time
	.global gMenu_BarPatternRows
gMenu_BarPatternRows:
	.4byte 0x00055000, 0x00000000
	.4byte 0x05055050, 0x00000000
	.4byte 0x05055050, 0x00000005
	.4byte 0x05055050, 0x00000505
	.4byte 0x05055050, 0x00050505
	.4byte 0x05055050, 0x05050505
	.global gRomShiftedTilePair
gRomShiftedTilePair:
	.incbin "games/tla/src/graphics/window/shiftpr.4bpp"
	.global gGraphics_ExpandNibbleTable
gGraphics_ExpandNibbleTable:
	.2byte 0, 10, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
	.2byte 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
	.global gLink_TimeLabelString
gLink_TimeLabelString:
	.ifdef TLA_EDITION_ES
	.asciz "TMPO."
	.else
	.ifdef TLA_EDITION_FR
	.asciz "TEMPS"
	.else
	.ifdef TLA_EDITION_IT
	.asciz "TEMPO"
	.else
	.asciz "TIME"
	.endif
	.endif
	.endif
	.balign 4, 0
	.global gDjinnMenu_BlinkTile
gDjinnMenu_BlinkTile:
	.incbin "games/tla/src/menu/djinn/blink.4bpp"
@ Item, column, row, highlight column, highlight row, highlight width
	.global gOwnerStatus_StatCursors
gOwnerStatus_StatCursors:
	.byte 0, 2, 0, 0, 0, 12, 0, 0
	.byte 2, 8, 1, 0, 1, 13, 0, 0
	.byte 3, 12, 3, 5, 3, 11, 0, 0
	.byte 4, 12, 4, 5, 4, 11, 0, 0
	.byte 5, 3, 6, 0, 6, 10, 0, 0
	.byte 6, 13, 9, 0, 9, 20, 0, 0
	.byte 7, 13, 10, 0, 10, 20, 0, 0
	.byte 8, 13, 11, 0, 11, 20, 0, 0
	.byte 9, 13, 12, 0, 12, 20, 0, 0
	.byte 0, 2, 0, 0, 0, 12, 0, 0
	.byte 2, 8, 1, 0, 1, 13, 0, 0
	.byte 3, 12, 3, 5, 3, 11, 0, 0
	.byte 4, 12, 4, 5, 4, 11, 0, 0
	.byte 7, 13, 10, 0, 10, 20, 0, 0
	.byte 8, 13, 11, 0, 11, 20, 0, 0
	.byte 9, 13, 12, 0, 12, 20, 0, 0
	.global gOwnerStatus_EquipmentCursors
gOwnerStatus_EquipmentCursors:
	.byte 10, 19, 1, 0
	.byte 11, 19, 2, 0
	.byte 12, 19, 3, 0
	.byte 13, 19, 4, 0
	.global gOwnerStatus_ConditionCursors
gOwnerStatus_ConditionCursors:
	.byte 14, 13, 0, 0
	.byte 15, 15, 0, 0
	.byte 16, 17, 0, 0
	.byte 17, 19, 0, 0
	.byte 18, 21, 0, 0
	.byte 19, 23, 0, 0
	.byte 20, 25, 0, 0
	.byte 21, 27, 0, 0
@ The status screen's labels: the German, Spanish, French and Italian
@ editions carry every language's level and HP abbreviations
	.if EDITION_INTERNATIONAL
	.ifndef TLA_EDITION_EN
	.global gOwnerStatus_NivString
gOwnerStatus_NivString:
	.asciz "Niv"
	.global gOwnerStatus_NvString
gOwnerStatus_NvString:
	.asciz "Nv."
	.endif
	.endif
	.global gOwnerStatus_LevelString
gOwnerStatus_LevelString:
	.asciz "Lv"
	.balign 4, 0
	.if EDITION_INTERNATIONAL
	.ifndef TLA_EDITION_EN
	.global gOwnerStatus_KpString
gOwnerStatus_KpString:
	.asciz "KP"
	.balign 4, 0
	.global gOwnerStatus_PvString
gOwnerStatus_PvString:
	.asciz "PV"
	.balign 4, 0
	.global gOwnerStatus_HpString
gOwnerStatus_HpString:
	.asciz "PS"
	.balign 4, 0
	.else
	.global gOwnerStatus_HpString
gOwnerStatus_HpString:
	.asciz "HP"
	.balign 4, 0
	.endif
	.else
	.global gOwnerStatus_HpString
gOwnerStatus_HpString:
	.asciz "HP"
	.balign 4, 0
	.endif
	.global gOwnerStatus_SlashString
gOwnerStatus_SlashString:
	.asciz "/"
	.balign 4, 0
	.if EDITION_INTERNATIONAL
	.global gOwnerStatus_PpString
gOwnerStatus_PpString:
	.asciz "PP"
	.else
	.global gOwnerStatus_PpString
gOwnerStatus_PpString:
	.asciz "EP"
	.endif
	.global gDjinnMenu_ElementXOffsets
gDjinnMenu_ElementXOffsets:
	.byte 0, 2, 0, 2
	.global gDjinnMenu_ElementYOffsets
gDjinnMenu_ElementYOffsets:
	.byte 0, 0, 1, 1
	.global gMenu_SelectionStepDelays
gMenu_SelectionStepDelays:
	.byte 46, 20, 28, 24, 30, 44, 29, 32
	.global gMenu_TopEntryCommandByPosition
gMenu_TopEntryCommandByPosition:
	.byte 0, 1, 2, 3, 4, 5
	.byte 0, 1, 3, 4, 5, 0
	.global gMenu_TopEntryPositionByCommand
gMenu_TopEntryPositionByCommand:
	.byte 0, 1, 2, 3, 4, 5
	.byte 0, 1, 0, 2, 3, 0
	.global gMenu_SaveSlotActionByPosition
gMenu_SaveSlotActionByPosition:
	.byte 0, 1, 2, 3, 4
	.byte 5, 1, 3, 4, 5
	.byte 1, 1, 3, 4, 5
	.byte 1, 1, 1, 0, 2
	.byte 3, 4, 5, 1, 0
	.global gMenu_ColonString
gMenu_ColonString:
	.asciz ":"
	.balign 4, 0
	.global gMenu_HexDigitsString
gMenu_HexDigitsString:
	.asciz "0123456789ABCDEF"
	.balign 4, 0
	.global gWorkspace_ItemTitle
gWorkspace_ItemTitle:
	.asciz "ITEM"
	.balign 4, 0
	.global gWorkspace_EnergyTitle
gWorkspace_EnergyTitle:
	.asciz "ENERGY"
	.balign 4, 0
	.global gWorkspace_StatusTitle
gWorkspace_StatusTitle:
	.asciz "STATUS"
	.balign 4, 0
	.global gWorkspace_PageLabel
gWorkspace_PageLabel:
	.asciz "PAGE:"
	.balign 4, 0
	.global gWorkspace_RangeDash
gWorkspace_RangeDash:
	.asciz "-"
	.balign 4, 0
