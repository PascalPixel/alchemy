	.include "games/common/include/game/ed_asm.h"
	.section .rodata
@ 文字幅 (0x20 から)
	.global UiText_GlyphWidths
UiText_GlyphWidths:
	.if EDITION_INTERNATIONAL
	.byte 5, 4, 7, 7, 8, 8, 8, 4, 5, 5, 5, 6, 4, 5, 4, 6
	.byte 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 5, 5, 6, 5, 6
	.byte 8, 6, 6, 6, 6, 6, 6, 6, 6, 4, 6, 6, 6, 8, 6, 6
	.byte 6, 6, 6, 6, 6, 6, 6, 8, 6, 6, 6, 5, 6, 5, 6, 6
	.byte 6, 6, 6, 6, 6, 6, 5, 6, 6, 4, 4, 5, 4, 8, 6, 6
	.byte 6, 6, 6, 6, 5, 6, 6, 8, 6, 6, 6, 5, 4, 5, 7, 1
	.byte 8, 2, 8, 2, 8, 6, 8, 6, 8, 4, 8, 4, 8, 7, 7, 7
	.byte 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	.byte 1, 4, 1, 1, 1, 1, 1, 1, 6, 1, 6, 1, 1, 1, 1, 1
	.byte 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 1, 1, 8, 8, 6
	.byte 6, 6, 6, 6, 6, 6, 8, 6, 6, 6, 6, 6, 4, 4, 4, 4
	.byte 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6
	.byte 6, 6, 6, 6, 6, 6, 8, 6, 6, 6, 6, 6, 4, 4, 4, 4
	.byte 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6
	.else
	.byte 7, 4, 7, 7, 8, 8, 8, 4, 5, 5, 5, 6, 4, 6, 4, 6
	.byte 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 4, 4, 5, 6, 5, 7
	.byte 8, 8, 8, 8, 8, 8, 8, 8, 8, 5, 6, 8, 8, 8, 8, 8
	.byte 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 8, 5, 6, 5, 7, 7
	.byte 6, 7, 7, 7, 7, 7, 5, 7, 7, 4, 4, 6, 4, 8, 7, 7
	.byte 7, 7, 7, 7, 5, 7, 6, 8, 6, 6, 6, 5, 4, 5, 7, 1
	.endif
	.global UiText_ColourPens
UiText_ColourPens:
	.byte 0, 3
	.byte 13, 13
	.byte 14, 4
	.byte 4, 15
	.byte 15, 1
	.byte 1, 13
	.byte 13, 2
	.byte 2, 1
	.global UiWindow_PartyColumnOffsets
UiWindow_PartyColumnOffsets:
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
	.global StatusMenu_LevelLetterString
StatusMenu_LevelLetterString:
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
@ 濁点, 半濁点をつけられる文字の範囲
	.global NameEntry_DakutenRanges
NameEntry_DakutenRanges:
	.byte 0xb6, 0xc4, 0xca, 0xce, 0x96, 0x9f, 0xe0, 0xe4, 0xea, 0xee, 0x93, 0x93, 0xb3, 0xb3, 0x00
	.global NameEntry_HandakutenRanges
NameEntry_HandakutenRanges:
	.byte 0xca, 0xce, 0xea, 0xee, 0x00
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
@ 属性ごとのジンの種類
	.global Djinn_ElementSprites
Djinn_ElementSprites:
	.2byte 90, 91, 92, 93
@ 仲間ごとのキャラ
	.global Character_SpriteTable
Character_SpriteTable:
	.2byte 0, 1, 2, 3, 35, 5, 6, 7
@ 子供のころ (フラグ 32)
	.global Character_ChildSpriteTable
Character_ChildSpriteTable:
	.2byte 18, 19, 2, 3, 36, 48, 6, 7	@ ロビン(こ), ジェラルド(こ), ...
	.global Ui_PairBobOffsets
Ui_PairBobOffsets:
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
	.global Ui_ObjectPulseScales
Ui_ObjectPulseScales:
	.4byte 0x11999, 0x12147, 0x128f5, 0x130a3
	.4byte 0x13851, 0x130a3, 0x128f5, 0x12147
@ 伸びる棒の 4bpp 行
	.global Menu_BarPatternRows
Menu_BarPatternRows:
	.4byte 0x00055000, 0x00000000
	.4byte 0x05055050, 0x00000000
	.4byte 0x05055050, 0x00000005
	.4byte 0x05055050, 0x00000505
	.4byte 0x05055050, 0x00050505
	.4byte 0x05055050, 0x05050505
	.global gRomShiftedTilePair
gRomShiftedTilePair:
	.incbin "window/shiftpr.4bpp"
	.global Graphics_ExpandNibbleTable
Graphics_ExpandNibbleTable:
	.2byte 0, 10, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
	.2byte 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
	.global Link_TimeLabelString
Link_TimeLabelString:
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
	.global DjinnMenu_BlinkTile
DjinnMenu_BlinkTile:
	.incbin "menu/djinn/blink.4bpp"
@ 項目, 列, 行, 強調列, 強調行, 強調幅
	.global OwnerStatus_StatCursors
OwnerStatus_StatCursors:
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
	.global OwnerStatus_EquipmentCursors
OwnerStatus_EquipmentCursors:
	.byte 10, 19, 1, 0
	.byte 11, 19, 2, 0
	.byte 12, 19, 3, 0
	.byte 13, 19, 4, 0
	.global OwnerStatus_ConditionCursors
OwnerStatus_ConditionCursors:
	.byte 14, 13, 0, 0
	.byte 15, 15, 0, 0
	.byte 16, 17, 0, 0
	.byte 17, 19, 0, 0
	.byte 18, 21, 0, 0
	.byte 19, 23, 0, 0
	.byte 20, 25, 0, 0
	.byte 21, 27, 0, 0
	.ifdef TBS_EDITION_ES
	.global OwnerStatus_NivString
OwnerStatus_NivString:
	.asciz "Niv"
	.else
	.ifdef TBS_EDITION_IT
	.global OwnerStatus_NivString
OwnerStatus_NivString:
	.asciz "Niv"
	.else
	.endif
	.endif
	.ifdef TBS_EDITION_ES
	.global OwnerStatus_NvString
OwnerStatus_NvString:
	.asciz "Nv."
	.else
	.ifdef TBS_EDITION_IT
	.global OwnerStatus_NvString
OwnerStatus_NvString:
	.asciz "Nv."
	.else
	.endif
	.endif
	.ifdef TBS_EDITION_FR
	.global OwnerStatus_LevelString
OwnerStatus_LevelString:
	.asciz "Niv"
	.else
	.global OwnerStatus_LevelString
OwnerStatus_LevelString:
	.asciz "Lv"
	.balign 4, 0
	.endif
	.ifdef TBS_EDITION_ES
	.global OwnerStatus_KpString
OwnerStatus_KpString:
	.asciz "KP"
	.balign 4, 0
	.else
	.ifdef TBS_EDITION_IT
	.global OwnerStatus_KpString
OwnerStatus_KpString:
	.asciz "KP"
	.balign 4, 0
	.else
	.endif
	.endif
	.ifdef TBS_EDITION_ES
	.global OwnerStatus_PvString
OwnerStatus_PvString:
	.asciz "PV"
	.balign 4, 0
	.else
	.ifdef TBS_EDITION_IT
	.global OwnerStatus_PvString
OwnerStatus_PvString:
	.asciz "PV"
	.balign 4, 0
	.else
	.endif
	.endif
	.ifdef TBS_EDITION_DE
	.global OwnerStatus_HpString
OwnerStatus_HpString:
	.asciz "KP"
	.balign 4, 0
	.else
	.ifdef TBS_EDITION_ES
	.global OwnerStatus_HpString
OwnerStatus_HpString:
	.asciz "PS"
	.balign 4, 0
	.else
	.ifdef TBS_EDITION_FR
	.global OwnerStatus_HpString
OwnerStatus_HpString:
	.asciz "PV"
	.balign 4, 0
	.else
	.ifdef TBS_EDITION_IT
	.global OwnerStatus_HpString
OwnerStatus_HpString:
	.asciz "PS"
	.balign 4, 0
	.else
	.global OwnerStatus_HpString
OwnerStatus_HpString:
	.asciz "HP"
	.balign 4, 0
	.endif
	.endif
	.endif
	.endif
	.global OwnerStatus_SlashString
OwnerStatus_SlashString:
	.asciz "/"
	.balign 4, 0
	.if EDITION_INTERNATIONAL
	.global OwnerStatus_PpString
OwnerStatus_PpString:
	.asciz "PP"
	.else
	.global OwnerStatus_PpString
OwnerStatus_PpString:
	.asciz "EP"
	.endif
	.global DjinnMenu_ElementXOffsets
DjinnMenu_ElementXOffsets:
	.byte 0, 2, 0, 2
	.global DjinnMenu_ElementYOffsets
DjinnMenu_ElementYOffsets:
	.byte 0, 0, 1, 1
	.global Menu_SelectionStepDelays
Menu_SelectionStepDelays:
	.byte 46, 20, 28, 24, 30, 44, 29, 32
	.global Menu_TopEntryCommandByPosition
Menu_TopEntryCommandByPosition:
	.byte 0, 1, 2, 3, 4, 5
	.byte 0, 1, 3, 4, 5, 0
	.global Menu_TopEntryPositionByCommand
Menu_TopEntryPositionByCommand:
	.byte 0, 1, 2, 3, 4, 5
	.byte 0, 1, 0, 2, 3, 0
	.global Menu_SaveSlotActionByPosition
Menu_SaveSlotActionByPosition:
	.byte 0, 1, 2, 3, 4
	.byte 5, 1, 3, 4, 5
	.byte 1, 1, 3, 4, 5
	.byte 1, 1, 1, 0, 2
	.byte 3, 4, 5, 1, 0
	.global Menu_ColonString
Menu_ColonString:
	.asciz ":"
	.balign 4, 0
	.global Menu_HexDigitsString
Menu_HexDigitsString:
	.asciz "0123456789ABCDEF"
	.balign 4, 0
	.global Workspace_ItemTitle
Workspace_ItemTitle:
	.asciz "ITEM"
	.balign 4, 0
	.global Workspace_EnergyTitle
Workspace_EnergyTitle:
	.asciz "ENERGY"
	.balign 4, 0
	.global Workspace_StatusTitle
Workspace_StatusTitle:
	.asciz "STATUS"
	.balign 4, 0
	.global Workspace_PageLabel
Workspace_PageLabel:
	.asciz "PAGE:"
	.balign 4, 0
	.global Workspace_RangeDash
Workspace_RangeDash:
	.asciz "-"
	.balign 4, 0
