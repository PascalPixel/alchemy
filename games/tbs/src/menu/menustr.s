	.include "games/tbs/include/game/ed_asm.h"
	.section .rodata
	.balign 4
	.ifdef TBS_EDITION_ES
	.global gStatusPanel_LevelString
gStatusPanel_LevelString:
	.asciz "Nv."
	.else
	.ifdef TBS_EDITION_FR
	.global gStatusPanel_LevelString
gStatusPanel_LevelString:
	.asciz "Niv"
	.else
	.global gStatusPanel_LevelString
gStatusPanel_LevelString:
	.asciz "Lv"
	.balign 4, 0
	.endif
	.endif
	.if EDITION_INTERNATIONAL
	.else
	.global gStatusPanel_HpRowString
gStatusPanel_HpRowString:
	.asciz "HP    /"
	.endif
	.if EDITION_INTERNATIONAL
	.else
	.global gStatusPanel_PpRowString
gStatusPanel_PpRowString:
	.asciz "EP    /"
	.endif
	.ifdef TBS_EDITION_JA
	.else
	.ifdef TBS_EDITION_DE
	.global gStatusPanel_HpString
gStatusPanel_HpString:
	.asciz "KP"
	.balign 4, 0
	.else
	.ifdef TBS_EDITION_ES
	.global gStatusPanel_HpString
gStatusPanel_HpString:
	.asciz "PV"
	.balign 4, 0
	.else
	.ifdef TBS_EDITION_FR
	.global gStatusPanel_HpString
gStatusPanel_HpString:
	.asciz "PV"
	.balign 4, 0
	.else
	.ifdef TBS_EDITION_IT
	.global gStatusPanel_HpString
gStatusPanel_HpString:
	.asciz "PS"
	.balign 4, 0
	.else
	.global gStatusPanel_HpString
gStatusPanel_HpString:
	.asciz "HP"
	.balign 4, 0
	.endif
	.endif
	.endif
	.endif
	.endif
	.if EDITION_INTERNATIONAL
	.global gStatusPanel_SlashString
gStatusPanel_SlashString:
	.asciz "/"
	.balign 4, 0
	.else
	.endif
	.if EDITION_INTERNATIONAL
	.global gStatusPanel_PpString
gStatusPanel_PpString:
	.asciz "PP"
	.balign 4, 0
	.else
	.endif
	.if EDITION_INTERNATIONAL
	.global gMenu_TimesString
gMenu_TimesString:
	.asciz "x"
	.balign 4, 0
	.else
	.endif
	.if EDITION_INTERNATIONAL
	.else
	.global gItemMenu_ArrangeKeysString
gItemMenu_ArrangeKeysString:
	.asciz "L+A:"
	.balign 4, 0
	.endif
	.if EDITION_INTERNATIONAL
	.else
	.global gItemMenu_EquipmentKeyString
gItemMenu_EquipmentKeyString:
	.asciz "R:"
	.balign 4, 0
	.endif
	.global gMenu_PointString
gMenu_PointString:
	.asciz "."
	.balign 4, 0
	.global gMenu_PlusSignString
gMenu_PlusSignString:
	.asciz "+"
	.balign 4, 0
	.global gMenu_MinusSignString
gMenu_MinusSignString:
	.asciz "-"
	.balign 4, 0
	.if EDITION_INTERNATIONAL
	.else
	.global gMenu_ShortcutLString
gMenu_ShortcutLString:
	.asciz "L"
	.balign 4, 0
	.endif
	.if EDITION_INTERNATIONAL
	.else
	.global gMenu_ShortcutLColonString
gMenu_ShortcutLColonString:
	.asciz "L:"
	.balign 4, 0
	.endif
	.if EDITION_INTERNATIONAL
	.else
	.global gMenu_ShortcutRString
gMenu_ShortcutRString:
	.asciz "R"
	.balign 4, 0
	.endif
	.if EDITION_INTERNATIONAL
	.else
	.global gMenu_ShortcutRColonString
gMenu_ShortcutRColonString:
	.asciz "R:"
	.balign 4, 0
	.endif
	.ifdef TBS_EDITION_ES
	.global gMenu_LvString
gMenu_LvString:
	.asciz "Nv."
	.else
	.ifdef TBS_EDITION_FR
	.global gMenu_LvString
gMenu_LvString:
	.asciz "Niv"
	.else
	.global gMenu_LvString
gMenu_LvString:
	.asciz "Lv"
	.balign 4, 0
	.endif
	.endif
	.global gMenu_SlashString
gMenu_SlashString:
	.asciz "/"
	.balign 4, 0
	.ifdef TBS_EDITION_DE
	.global gMenu_HpString
gMenu_HpString:
	.asciz "KP"
	.balign 4, 0
	.else
	.ifdef TBS_EDITION_ES
	.global gMenu_HpString
gMenu_HpString:
	.asciz "PV"
	.balign 4, 0
	.else
	.ifdef TBS_EDITION_FR
	.global gMenu_HpString
gMenu_HpString:
	.asciz "PV"
	.balign 4, 0
	.else
	.ifdef TBS_EDITION_IT
	.global gMenu_HpString
gMenu_HpString:
	.asciz "PS"
	.balign 4, 0
	.else
	.global gMenu_HpString
gMenu_HpString:
	.asciz "HP"
	.balign 4, 0
	.endif
	.endif
	.endif
	.endif
	.if EDITION_INTERNATIONAL
	.global gMenu_PpString
gMenu_PpString:
	.asciz "PP"
	.balign 4, 0
	.else
	.global gMenu_PpString
gMenu_PpString:
	.asciz "EP"
	.balign 4, 0
	.endif
@ 4bpp row that grows one pixel at a time in color 5
	.global gMenu_PatternRowMasks
gMenu_PatternRowMasks:
	.4byte 0x00055000, 0x00000000
	.4byte 0x05055050, 0x00000000
	.4byte 0x05055050, 0x00000005
	.4byte 0x05055050, 0x00000505
	.4byte 0x05055050, 0x00050505
	.4byte 0x05055050, 0x05050505
	.global gMenu_BackdropFrameTile
gMenu_BackdropFrameTile:
	.incbin "games/tbs/src/menu/backdrop.4bpp"
	.ifdef TBS_EDITION_ES
	.global gDjinnMenu_TextLevel
gDjinnMenu_TextLevel:
	.asciz "Nv."
	.else
	.ifdef TBS_EDITION_FR
	.global gDjinnMenu_TextLevel
gDjinnMenu_TextLevel:
	.asciz "Niv"
	.else
	.global gDjinnMenu_TextLevel
gDjinnMenu_TextLevel:
	.asciz "Lv"
	.balign 4, 0
	.endif
	.endif
	.global gDjinnMenu_TextSlash
gDjinnMenu_TextSlash:
	.asciz "/"
	.balign 4, 0
