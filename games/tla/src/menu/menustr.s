	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.balign 4
	.ifdef TLA_EDITION_ES
	.global gUi_LvString
gUi_LvString:
	.asciz "Nv."
	.else
	.ifdef TLA_EDITION_FR
	.global gUi_LvString
gUi_LvString:
	.asciz "Niv"
	.else
	.global gUi_LvString
gUi_LvString:
	.asciz "Lv"
	.balign 4, 0
	.endif
	.endif
	.if EDITION_INTERNATIONAL
	.ifdef TLA_EDITION_DE
	.global gUi_HpString
gUi_HpString:
	.asciz "KP"
	.else
	.ifdef TLA_EDITION_ES
	.global gUi_HpString
gUi_HpString:
	.asciz "PV"
	.else
	.ifdef TLA_EDITION_FR
	.global gUi_HpString
gUi_HpString:
	.asciz "PV"
	.else
	.ifdef TLA_EDITION_IT
	.global gUi_HpString
gUi_HpString:
	.asciz "PS"
	.else
	.global gUi_HpString
gUi_HpString:
	.asciz "HP"
	.endif
	.endif
	.endif
	.endif
	.balign 4, 0
	.global gUi_SlashString
gUi_SlashString:
	.asciz "/"
	.balign 4, 0
	.global gUi_PpString
gUi_PpString:
	.asciz "PP"
	.balign 4, 0
	.else
	.global gUi_HpRowString
gUi_HpRowString:
	.asciz "HP    /"
	.global gUi_PpRowString
gUi_PpRowString:
	.asciz "EP    /"
	.endif
@ The frame tile the cancel prompt blinks; ⚓️ drops ☀️'s "x" string here
	.global gData_08105948
gData_08105948:
	.incbin "games/common/src/menu/backdrop.4bpp"
	.if EDITION_INTERNATIONAL
	.else
	.global gItemMenu_ArrangeKeysString
gItemMenu_ArrangeKeysString:
	.asciz "L+A:"
	.balign 4, 0
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
	.global gMenu_ShortcutLColonString
gMenu_ShortcutLColonString:
	.asciz "L:"
	.balign 4, 0
	.global gMenu_ShortcutRString
gMenu_ShortcutRString:
	.asciz "R"
	.balign 4, 0
	.global gMenu_ShortcutRColonString
gMenu_ShortcutRColonString:
	.asciz "R:"
	.balign 4, 0
	.endif
	.ifdef TLA_EDITION_ES
	.global gMenu_LvString
gMenu_LvString:
	.asciz "Nv."
	.else
	.ifdef TLA_EDITION_FR
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
	.ifdef TLA_EDITION_DE
	.global gMenu_HpString
gMenu_HpString:
	.asciz "KP"
	.else
	.ifdef TLA_EDITION_ES
	.global gMenu_HpString
gMenu_HpString:
	.asciz "PV"
	.else
	.ifdef TLA_EDITION_FR
	.global gMenu_HpString
gMenu_HpString:
	.asciz "PV"
	.else
	.ifdef TLA_EDITION_IT
	.global gMenu_HpString
gMenu_HpString:
	.asciz "PS"
	.else
	.global gMenu_HpString
gMenu_HpString:
	.asciz "HP"
	.endif
	.endif
	.endif
	.endif
	.balign 4, 0
	.if EDITION_INTERNATIONAL
	.global gMenu_PpString
gMenu_PpString:
	.asciz "PP"
	.else
	.global gMenu_PpString
gMenu_PpString:
	.asciz "EP"
	.endif
	.balign 4, 0
@ 4bpp rows in colour 5 that grow by one pixel at a time
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
	.incbin "games/common/src/menu/backdrop.4bpp"
	.ifdef TLA_EDITION_ES
	.global gDjinnMenu_TextLevel
gDjinnMenu_TextLevel:
	.asciz "Nv."
	.else
	.ifdef TLA_EDITION_FR
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
