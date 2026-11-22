	.include "games/tla/include/edition.inc"
	.section .rodata
@ The digits 0 to 8 the party status draws
	.global gPartyStatus_DigitTiles
gPartyStatus_DigitTiles:
	.incbin "games/tla/src/window/digits.4bpp"
@ The name entry keyboards, ☀️'s
	.global gNameEntry_KeyboardTilemap
gNameEntry_KeyboardTilemap:
	.ifdef TLA_EDITION_JA
	.incbin "games/tla/src/window/keyboard_ja.table.lz"
	.global gNameEntry_AlternateKeyboardTilemap
gNameEntry_AlternateKeyboardTilemap:
	.incbin "games/tla/src/window/keyboard_ja_latin.table.lz"
	.global NameEntry_UnusedKeyboardTilemap
NameEntry_UnusedKeyboardTilemap:
	.incbin "games/tla/src/window/keyboard_ja_latin_unused.table.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/window/keyboard_es.table.lz"
	.else
	.incbin "games/tla/src/window/keyboard.table.lz"
	.endif
	.endif
