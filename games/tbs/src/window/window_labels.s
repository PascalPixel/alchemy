	.include "games/tbs/include/edition.inc"
	.section .rodata
	.ifdef TBS_EDITION_ES
	.global gBattleStatusScreen_NivString
gBattleStatusScreen_NivString:
	.asciz "Niv"
	.else
	.ifdef TBS_EDITION_IT
	.global gBattleStatusScreen_NivString
gBattleStatusScreen_NivString:
	.asciz "Niv"
	.else
	.endif
	.endif
	.ifdef TBS_EDITION_ES
	.global gBattleStatusScreen_NvString
gBattleStatusScreen_NvString:
	.asciz "Nv."
	.else
	.ifdef TBS_EDITION_IT
	.global gBattleStatusScreen_NvString
gBattleStatusScreen_NvString:
	.asciz "Nv."
	.else
	.endif
	.endif
	.ifdef TBS_EDITION_FR
	.global gBattleStatusScreen_LevelString
gBattleStatusScreen_LevelString:
	.asciz "Niv"
	.else
	.global gBattleStatusScreen_LevelString
gBattleStatusScreen_LevelString:
	.asciz "Lv"
	.balign 4, 0
	.endif
	.ifdef TBS_EDITION_ES
	.global gBattleStatusScreen_KpString
gBattleStatusScreen_KpString:
	.asciz "KP"
	.balign 4, 0
	.else
	.ifdef TBS_EDITION_IT
	.global gBattleStatusScreen_KpString
gBattleStatusScreen_KpString:
	.asciz "KP"
	.balign 4, 0
	.else
	.endif
	.endif
	.ifdef TBS_EDITION_ES
	.global gBattleStatusScreen_PvString
gBattleStatusScreen_PvString:
	.asciz "PV"
	.balign 4, 0
	.else
	.ifdef TBS_EDITION_IT
	.global gBattleStatusScreen_PvString
gBattleStatusScreen_PvString:
	.asciz "PV"
	.balign 4, 0
	.else
	.endif
	.endif
	.ifdef TBS_EDITION_DE
	.global gBattleStatusScreen_HpString
gBattleStatusScreen_HpString:
	.asciz "KP"
	.balign 4, 0
	.else
	.ifdef TBS_EDITION_ES
	.global gBattleStatusScreen_HpString
gBattleStatusScreen_HpString:
	.asciz "PS"
	.balign 4, 0
	.else
	.ifdef TBS_EDITION_FR
	.global gBattleStatusScreen_HpString
gBattleStatusScreen_HpString:
	.asciz "PV"
	.balign 4, 0
	.else
	.ifdef TBS_EDITION_IT
	.global gBattleStatusScreen_HpString
gBattleStatusScreen_HpString:
	.asciz "PS"
	.balign 4, 0
	.else
	.global gBattleStatusScreen_HpString
gBattleStatusScreen_HpString:
	.asciz "HP"
	.balign 4, 0
	.endif
	.endif
	.endif
	.endif
	.global gBattleStatusScreen_SlashString
gBattleStatusScreen_SlashString:
	.asciz "/"
	.balign 4, 0
	.if EDITION_INTERNATIONAL
	.global gBattleStatusScreen_PpString
gBattleStatusScreen_PpString:
	.asciz "PP"
	.else
	.global gBattleStatusScreen_PpString
gBattleStatusScreen_PpString:
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
	.global gFileList_ActionByPosition
gFileList_ActionByPosition:
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
