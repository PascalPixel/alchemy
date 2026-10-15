	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.balign 4
@ ジンの種類
	.global gMenu_PartySpriteResourceIds
gMenu_PartySpriteResourceIds:
	.4byte 90, 91, 92, 93
@ 名前入力の文字盤
	.global gNameEntry_KeyboardTilemap
gNameEntry_KeyboardTilemap:
	.ifdef TBS_EDITION_JA
	.incbin "games/common/src/graphics/window/keyboard_ja.table.lz"
	.global gNameEntry_AlternateKeyboardTilemap
gNameEntry_AlternateKeyboardTilemap:
	.incbin "games/common/src/graphics/window/keyboard_j2.table.lz"
	.global NameEntry_KeyboardTilemap3
NameEntry_KeyboardTilemap3:
	.incbin "games/common/src/graphics/window/keyboard_j3.table.lz"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "games/common/src/graphics/window/keyboard_es.table.lz"
	.else
	.incbin "games/common/src/graphics/window/keyboard.table.lz"
	.endif
	.endif
	.balign 4, 0
	.global gUi_FixedTileBlocks
gUi_FixedTileBlocks:
	.4byte gResource_FixedBlockBTiles
