	.include "games/tbs/include/game/ed_asm.h"
	.section .rodata
	.balign 4
	.global gWindowSprite_CursorFrameTiles
gWindowSprite_CursorFrameTiles:
	.incbin "games/tbs/src/graphics/window/cursor_frame.8bpp8x16"
@ The width of each main font glyph in pixels, from the space (0x20)
	.global gFont_GlyphWidths
gFont_GlyphWidths:
	.include "games/tbs/src/graphics/window/glyph_widths.inc"
	.global gFont_ColorPens
gFont_ColorPens:
	.byte 0, 3
	.byte 13, 13
	.byte 14, 4
	.byte 4, 15
	.byte 15, 1
	.byte 1, 13
	.byte 13, 2
	.byte 2, 1
	.global gWindow_PartyColumnOffsets
gWindow_PartyColumnOffsets:
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
	.global gSummaryScreen_LevelLetterString
gSummaryScreen_LevelLetterString:
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
	.global gNamingScreen_VoicedMarkRanges
gNamingScreen_VoicedMarkRanges:
	.byte 0xb6, 0xc4, 0xca, 0xce, 0x96, 0x9f, 0xe0, 0xe4, 0xea, 0xee, 0x93, 0x93, 0xb3, 0xb3, 0x00
	.global gNamingScreen_SemivoicedMarkRanges
gNamingScreen_SemivoicedMarkRanges:
	.byte 0xca, 0xce, 0xea, 0xee, 0x00
	.global gNamingScreen_CaretWave
gNamingScreen_CaretWave:
	.byte 0, 1, 2, 3, 3, 2, 1, 0
	.ifdef TBS_EDITION_DE
	.global gNamingScreen_DeleteKeyLabel
gNamingScreen_DeleteKeyLabel:
	.asciz "ENT"
	.else
	.ifdef TBS_EDITION_ES
	.global gNamingScreen_DeleteKeyLabel
gNamingScreen_DeleteKeyLabel:
	.asciz "SUP"
	.else
	.ifdef TBS_EDITION_FR
	.global gNamingScreen_DeleteKeyLabel
gNamingScreen_DeleteKeyLabel:
	.asciz "EFF"
	.else
	.endif
	.endif
	.endif
	.ifdef TBS_EDITION_DE
	.global gNamingScreen_EndKeyLabel
gNamingScreen_EndKeyLabel:
	.asciz "ENDE"
	.space 1
	.else
	.ifdef TBS_EDITION_ES
	.global gNamingScreen_EndKeyLabel
gNamingScreen_EndKeyLabel:
	.asciz "FIN"
	.else
	.ifdef TBS_EDITION_FR
	.global gNamingScreen_EndKeyLabel
gNamingScreen_EndKeyLabel:
	.asciz "FIN"
	.else
	.endif
	.endif
	.endif
@ The sprite of each element's Djinni (Venus, Mercury, Mars, Jupiter)
	.global gDjinn_ElementSprites
gDjinn_ElementSprites:
	.2byte 90, 91, 92, 93
@ The sprite of each party member
	.global gCharacter_SpriteTable
gCharacter_SpriteTable:
	.2byte 0, 1, 2, 3, 35, 5, 6, 7
@ The sprite of each party member as a child (while flag 32 is set)
	.global gCharacter_ChildSpriteTable
gCharacter_ChildSpriteTable:
	.2byte 18, 19, 2, 3, 36, 48, 6, 7	@ child Isaac, child Garet, ...
	.global gMenuTitleBobOffsets
gMenuTitleBobOffsets:
	.ifdef TBS_EDITION_ES
	.byte 0, 0, 1, 2, 3, 3, 2, 1
	.else
	.ifdef TBS_EDITION_FR
	.byte 0, 0, 1, 2, 3, 3, 2, 1
	.else
	.byte 0, 0, 1, 2, 3, 3, 2, 1, 0, 0
	.endif
	.endif
	.balign 4
	.global gWindowSprite_PulseScales
gWindowSprite_PulseScales:
	.4byte 0x11999, 0x12147, 0x128f5, 0x130a3
	.4byte 0x13851, 0x130a3, 0x128f5, 0x12147
@ The 4bpp pixel rows of a growing bar
	.global gMenu_BarPatternRows
gMenu_BarPatternRows:
	.4byte 0x00055000, 0x00000000
	.4byte 0x05055050, 0x00000000
	.4byte 0x05055050, 0x00000005
	.4byte 0x05055050, 0x00000505
	.4byte 0x05055050, 0x00050505
	.4byte 0x05055050, 0x05050505
	.global gPageArrowTiles
gPageArrowTiles:
	.incbin "games/tbs/src/graphics/window/shiftpr.4bpp"
	.global gGraphics_ExpandNibbleTable
gGraphics_ExpandNibbleTable:
	.2byte 0, 10, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
	.2byte 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
	.global gLink_TimeLabelString
gLink_TimeLabelString:
	.ifdef TBS_EDITION_ES
	.asciz "TMPO."
	.balign 4, 0
	.else
	.ifdef TBS_EDITION_FR
	.asciz "TEMPS"
	.balign 4, 0
	.else
	.asciz "TIME"
	.balign 4, 0
	.endif
	.endif
	.global gDjinnMenu_BlinkTile
gDjinnMenu_BlinkTile:
	.incbin "games/tbs/src/menu/djinn/blink.4bpp"
@ Each cursor stop: item, column, row, highlight column, highlight row,
@ highlight width
	.global gBattleSummaryScreen_StatCursors
gBattleSummaryScreen_StatCursors:
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
	.global gBattleSummaryScreen_EquipmentCursors
gBattleSummaryScreen_EquipmentCursors:
	.byte 10, 19, 1, 0
	.byte 11, 19, 2, 0
	.byte 12, 19, 3, 0
	.byte 13, 19, 4, 0
	.global gBattleSummaryScreen_ConditionCursors
gBattleSummaryScreen_ConditionCursors:
	.byte 14, 13, 0, 0
	.byte 15, 15, 0, 0
	.byte 16, 17, 0, 0
	.byte 17, 19, 0, 0
	.byte 18, 21, 0, 0
	.byte 19, 23, 0, 0
	.byte 20, 25, 0, 0
	.byte 21, 27, 0, 0
	.ifdef TBS_EDITION_ES
	.global gBattleSummaryScreen_NivString
gBattleSummaryScreen_NivString:
	.asciz "Niv"
	.else
	.ifdef TBS_EDITION_IT
	.global gBattleSummaryScreen_NivString
gBattleSummaryScreen_NivString:
	.asciz "Niv"
	.else
	.endif
	.endif
	.ifdef TBS_EDITION_ES
	.global gBattleSummaryScreen_NvString
gBattleSummaryScreen_NvString:
	.asciz "Nv."
	.else
	.ifdef TBS_EDITION_IT
	.global gBattleSummaryScreen_NvString
gBattleSummaryScreen_NvString:
	.asciz "Nv."
	.else
	.endif
	.endif
	.ifdef TBS_EDITION_FR
	.global gBattleSummaryScreen_LevelString
gBattleSummaryScreen_LevelString:
	.asciz "Niv"
	.else
	.global gBattleSummaryScreen_LevelString
gBattleSummaryScreen_LevelString:
	.asciz "Lv"
	.balign 4, 0
	.endif
	.ifdef TBS_EDITION_ES
	.global gBattleSummaryScreen_KpString
gBattleSummaryScreen_KpString:
	.asciz "KP"
	.balign 4, 0
	.else
	.ifdef TBS_EDITION_IT
	.global gBattleSummaryScreen_KpString
gBattleSummaryScreen_KpString:
	.asciz "KP"
	.balign 4, 0
	.else
	.endif
	.endif
	.ifdef TBS_EDITION_ES
	.global gBattleSummaryScreen_PvString
gBattleSummaryScreen_PvString:
	.asciz "PV"
	.balign 4, 0
	.else
	.ifdef TBS_EDITION_IT
	.global gBattleSummaryScreen_PvString
gBattleSummaryScreen_PvString:
	.asciz "PV"
	.balign 4, 0
	.else
	.endif
	.endif
	.ifdef TBS_EDITION_DE
	.global gBattleSummaryScreen_HpString
gBattleSummaryScreen_HpString:
	.asciz "KP"
	.balign 4, 0
	.else
	.ifdef TBS_EDITION_ES
	.global gBattleSummaryScreen_HpString
gBattleSummaryScreen_HpString:
	.asciz "PS"
	.balign 4, 0
	.else
	.ifdef TBS_EDITION_FR
	.global gBattleSummaryScreen_HpString
gBattleSummaryScreen_HpString:
	.asciz "PV"
	.balign 4, 0
	.else
	.ifdef TBS_EDITION_IT
	.global gBattleSummaryScreen_HpString
gBattleSummaryScreen_HpString:
	.asciz "PS"
	.balign 4, 0
	.else
	.global gBattleSummaryScreen_HpString
gBattleSummaryScreen_HpString:
	.asciz "HP"
	.balign 4, 0
	.endif
	.endif
	.endif
	.endif
	.global gBattleSummaryScreen_SlashString
gBattleSummaryScreen_SlashString:
	.asciz "/"
	.balign 4, 0
	.if EDITION_INTERNATIONAL
	.global gBattleSummaryScreen_PpString
gBattleSummaryScreen_PpString:
	.asciz "PP"
	.else
	.global gBattleSummaryScreen_PpString
gBattleSummaryScreen_PpString:
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
	.global gFieldMenu_CommandByPosition
gFieldMenu_CommandByPosition:
	.byte 0, 1, 2, 3, 4, 5
	.byte 0, 1, 3, 4, 5, 0
	.global gFieldMenu_PositionByCommand
gFieldMenu_PositionByCommand:
	.byte 0, 1, 2, 3, 4, 5
	.byte 0, 1, 0, 2, 3, 0
	.global gFileMenu_ActionByPosition
gFileMenu_ActionByPosition:
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
	.global gDebugMenu_ItemTitle
gDebugMenu_ItemTitle:
	.asciz "ITEM"
	.balign 4, 0
	.global gDebugMenu_EnergyTitle
gDebugMenu_EnergyTitle:
	.asciz "ENERGY"
	.balign 4, 0
	.global gDebugMenu_StatusTitle
gDebugMenu_StatusTitle:
	.asciz "STATUS"
	.balign 4, 0
	.global gDebugMenu_PageLabel
gDebugMenu_PageLabel:
	.asciz "PAGE:"
	.balign 4, 0
	.global gDebugMenu_RangeDash
gDebugMenu_RangeDash:
	.asciz "-"
	.balign 4, 0
