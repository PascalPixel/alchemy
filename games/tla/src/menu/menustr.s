	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.balign 4
	.ifdef TLA_EDITION_ES
	.global Ui_LvString
Ui_LvString:
	.asciz "Nv."
	.else
	.ifdef TLA_EDITION_FR
	.global Ui_LvString
Ui_LvString:
	.asciz "Niv"
	.else
	.global Ui_LvString
Ui_LvString:
	.asciz "Lv"
	.balign 4, 0
	.endif
	.endif
	.if EDITION_INTERNATIONAL
	.ifdef TLA_EDITION_DE
	.global Ui_HpString
Ui_HpString:
	.asciz "KP"
	.else
	.ifdef TLA_EDITION_ES
	.global Ui_HpString
Ui_HpString:
	.asciz "PV"
	.else
	.ifdef TLA_EDITION_FR
	.global Ui_HpString
Ui_HpString:
	.asciz "PV"
	.else
	.ifdef TLA_EDITION_IT
	.global Ui_HpString
Ui_HpString:
	.asciz "PS"
	.else
	.global Ui_HpString
Ui_HpString:
	.asciz "HP"
	.endif
	.endif
	.endif
	.endif
	.balign 4, 0
	.global Ui_SlashString
Ui_SlashString:
	.asciz "/"
	.balign 4, 0
	.global Ui_PpString
Ui_PpString:
	.asciz "PP"
	.balign 4, 0
	.else
	.global Ui_HpRowString
Ui_HpRowString:
	.asciz "HP    /"
	.global Ui_PpRowString
Ui_PpRowString:
	.asciz "EP    /"
	.endif
@ The frame tile the cancel prompt blinks; ⚓️ drops ☀️'s "x" string here
	.global Data_08105948
Data_08105948:
	.incbin "games/common/src/menu/backdrop.4bpp"
	.if EDITION_INTERNATIONAL
	.else
	.global ItemMenu_ArrangeKeysString
ItemMenu_ArrangeKeysString:
	.asciz "L+A:"
	.balign 4, 0
	.global ItemMenu_EquipmentKeyString
ItemMenu_EquipmentKeyString:
	.asciz "R:"
	.balign 4, 0
	.endif
	.global Menu_PointString
Menu_PointString:
	.asciz "."
	.balign 4, 0
	.global Menu_PlusSignString
Menu_PlusSignString:
	.asciz "+"
	.balign 4, 0
	.global Menu_MinusSignString
Menu_MinusSignString:
	.asciz "-"
	.balign 4, 0
	.if EDITION_INTERNATIONAL
	.else
	.global Menu_ShortcutLString
Menu_ShortcutLString:
	.asciz "L"
	.balign 4, 0
	.global Menu_ShortcutLColonString
Menu_ShortcutLColonString:
	.asciz "L:"
	.balign 4, 0
	.global Menu_ShortcutRString
Menu_ShortcutRString:
	.asciz "R"
	.balign 4, 0
	.global Menu_ShortcutRColonString
Menu_ShortcutRColonString:
	.asciz "R:"
	.balign 4, 0
	.endif
	.ifdef TLA_EDITION_ES
	.global Menu_LvString
Menu_LvString:
	.asciz "Nv."
	.else
	.ifdef TLA_EDITION_FR
	.global Menu_LvString
Menu_LvString:
	.asciz "Niv"
	.else
	.global Menu_LvString
Menu_LvString:
	.asciz "Lv"
	.balign 4, 0
	.endif
	.endif
	.global Menu_SlashString
Menu_SlashString:
	.asciz "/"
	.balign 4, 0
	.ifdef TLA_EDITION_DE
	.global Menu_HpString
Menu_HpString:
	.asciz "KP"
	.else
	.ifdef TLA_EDITION_ES
	.global Menu_HpString
Menu_HpString:
	.asciz "PV"
	.else
	.ifdef TLA_EDITION_FR
	.global Menu_HpString
Menu_HpString:
	.asciz "PV"
	.else
	.ifdef TLA_EDITION_IT
	.global Menu_HpString
Menu_HpString:
	.asciz "PS"
	.else
	.global Menu_HpString
Menu_HpString:
	.asciz "HP"
	.endif
	.endif
	.endif
	.endif
	.balign 4, 0
	.if EDITION_INTERNATIONAL
	.global Menu_PpString
Menu_PpString:
	.asciz "PP"
	.else
	.global Menu_PpString
Menu_PpString:
	.asciz "EP"
	.endif
	.balign 4, 0
@ 4bpp rows in colour 5 that grow by one pixel at a time
	.global Menu_PatternRowMasks
Menu_PatternRowMasks:
	.4byte 0x00055000, 0x00000000
	.4byte 0x05055050, 0x00000000
	.4byte 0x05055050, 0x00000005
	.4byte 0x05055050, 0x00000505
	.4byte 0x05055050, 0x00050505
	.4byte 0x05055050, 0x05050505
	.global Menu_BackdropFrameTile
Menu_BackdropFrameTile:
	.incbin "games/common/src/menu/backdrop.4bpp"
	.ifdef TLA_EDITION_ES
	.global DjinnMenu_TextLevel
DjinnMenu_TextLevel:
	.asciz "Nv."
	.else
	.ifdef TLA_EDITION_FR
	.global DjinnMenu_TextLevel
DjinnMenu_TextLevel:
	.asciz "Niv"
	.else
	.global DjinnMenu_TextLevel
DjinnMenu_TextLevel:
	.asciz "Lv"
	.balign 4, 0
	.endif
	.endif
	.global DjinnMenu_TextSlash
DjinnMenu_TextSlash:
	.asciz "/"
	.balign 4, 0
