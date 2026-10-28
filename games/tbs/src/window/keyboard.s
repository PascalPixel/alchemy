	.include "games/tbs/include/ed_asm.h"
	.section .rodata
	.balign 4
@ The sprite of each element's Djinni (Venus, Mercury, Mars, Jupiter)
	.global gMenu_PartySpriteResourceIds
gMenu_PartySpriteResourceIds:
	.4byte 90, 91, 92, 93
@ The name entry screen's keyboard tilemaps
	.global gNameEntry_KeyboardTilemap
gNameEntry_KeyboardTilemap:
	.ifdef TBS_EDITION_JA
	.incbin "games/tbs/src/window/keyboard_ja.table.lz"
	.global gNameEntry_AlternateKeyboardTilemap
gNameEntry_AlternateKeyboardTilemap:
	.incbin "games/tbs/src/window/keyboard_j2.table.lz"
	.global NameEntry_KeyboardTilemap3
NameEntry_KeyboardTilemap3:
	.incbin "games/tbs/src/window/keyboard_j3.table.lz"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "games/tbs/src/window/keyboard_es.table.lz"
	.else
	.incbin "games/tbs/src/window/keyboard.table.lz"
	.endif
	.endif
	.balign 4, 0
	.global gWindowSprite_PointerTiles
gWindowSprite_PointerTiles:
	.4byte gPointingHandTiles
