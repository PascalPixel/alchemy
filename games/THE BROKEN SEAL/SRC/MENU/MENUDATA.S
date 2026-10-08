	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
@ カーソルの揺れ
	.global UiMenu_CursorBobX
UiMenu_CursorBobX:
	.byte 0, 1, 1, 2, 2, 1, 1, 0, 0
	.global UiMenu_CursorBobY
UiMenu_CursorBobY:
	.byte 0, -1, -1, -2, -2, -1, -1, 0, 0
@ 一覧の並び (0xff で終わり)
	.global Menu_ListOrderDefault
Menu_ListOrderDefault:
	.byte 0x00, 0x01, 0x04, 0x03, 0x02, 0x05, 0x06, 0x07, 0x08, 0x09
	.byte 0xff
	.global Menu_ListOrderMode2
Menu_ListOrderMode2:
	.byte 0x01, 0x04, 0x03, 0x02, 0x05, 0x06, 0x07, 0x08, 0x09, 0x00
	.byte 0xff
	.global Menu_ListOrderMode1
Menu_ListOrderMode1:
	.byte 0x81, 0x84, 0x83, 0x82, 0x85, 0x86, 0x87, 0x88, 0x89, 0x01
	.byte 0x04, 0x03, 0x02, 0x05, 0x06, 0x07, 0x08, 0x09, 0x00, 0xff
	.global Menu_ListOrderMode0
Menu_ListOrderMode0:
	.byte 0x81, 0x84, 0x83, 0x82, 0x85, 0x86, 0x87, 0x88, 0x89, 0x00
	.byte 0x01, 0x04, 0x03, 0x02, 0x05, 0x06, 0x07, 0x08, 0x09, 0xff
	.if EDITION_INTERNATIONAL
	.balign 4
	.global ItemMenu_CommandColumnXTable
ItemMenu_CommandColumnXTable:
	.4byte 86, 117, 166
	.4byte 86, 117, 166
	.else
	.endif
@ 命令名の幅
	.ifdef TBS_EDITION_JA
	.global CharacterMenu_CursorWidths
CharacterMenu_CursorWidths:
	.byte 9, 4, 6, 5, 7
	.else
	.ifdef TBS_EDITION_DE
	.global CharacterMenu_CursorWidths
CharacterMenu_CursorWidths:
	.byte 7, 5, 7, 6, 5
	.else
	.global CharacterMenu_CursorWidths
CharacterMenu_CursorWidths:
	.byte 6, 7, 7, 6, 6
	.endif
	.endif
	.balign 4
	.global FourObjectMotion_ResourceIds
FourObjectMotion_ResourceIds:
	.4byte 0x5a, 0x5b, 0x5c, 0x5d
