	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.balign 4
@ ジンの種類
	.global Menu_PartySpriteResourceIds
Menu_PartySpriteResourceIds:
	.4byte 90, 91, 92, 93
@ 名前入力の文字盤
	.global NameEntry_KeyboardTilemap
NameEntry_KeyboardTilemap:
	.ifdef TBS_EDITION_JA
	.incbin "graphics/window/keybd_ja.table.lz"
	.global NameEntry_AlternateKeyboardTilemap
NameEntry_AlternateKeyboardTilemap:
	.incbin "graphics/window/keybd_j2.table.lz"
	.global NameEntry_KeyboardTilemap3
NameEntry_KeyboardTilemap3:
	.incbin "graphics/window/keybd_j3.table.lz"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "graphics/window/keybd_es.table.lz"
	.else
	.incbin "graphics/window/keyboard.table.lz"
	.endif
	.endif
	.balign 4, 0
	.global Ui_FixedTileBlocks
Ui_FixedTileBlocks:
	.4byte Resource_FixedBlockBTiles
