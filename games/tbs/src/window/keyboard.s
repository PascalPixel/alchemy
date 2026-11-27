	.include "games/tbs/include/edition.inc"
	.include "enums/sprite_ids.inc"
	.section .rodata
	.balign 4
@ The sprite of each element's Djinni (Venus, Mercury, Mars, Jupiter)
	.global FileListDjinnSpriteIds
FileListDjinnSpriteIds:
	.4byte SPRITE_VENUS_DJINNI, SPRITE_MERCURY_DJINNI, SPRITE_MARS_DJINNI
	.4byte SPRITE_JUPITER_DJINNI
@ The name entry screen's keyboard tilemaps
	.global NameEntryKeyboardTilemap
NameEntryKeyboardTilemap:
	.ifdef TBS_EDITION_JA
	.incbin "games/tbs/src/window/keyboard_ja.tilemap.lz"
	.global NameEntryAlternateKeyboardTilemap
NameEntryAlternateKeyboardTilemap:
	.incbin "games/tbs/src/window/keyboard_ja_latin.tilemap.lz"
	.global NameEntryUnusedKeyboardTilemap
NameEntryUnusedKeyboardTilemap:
	.incbin "games/tbs/src/window/keyboard_ja_latin_unused.tilemap.lz"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "games/tbs/src/window/keyboard_es.tilemap.lz"
	.else
	.incbin "games/tbs/src/window/keyboard.tilemap.lz"
	.endif
	.endif
	.balign 4, 0
	.global WindowSpritePointerTiles
WindowSpritePointerTiles:
	.4byte PointingHandTiles
