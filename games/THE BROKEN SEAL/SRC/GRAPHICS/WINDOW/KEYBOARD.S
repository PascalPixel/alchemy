	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.balign 4
@ ジンの種類
	.global Menu_PartySpriteResourceIds
Menu_PartySpriteResourceIds:
	.4byte 90, 91, 92, 93
@ 名前入力の文字盤
	.global NameEntry_KeyboardTilemap
NameEntry_KeyboardTilemap:
	.ifdef TBS_EDITION_ES
	.incbin "GRAPHICS/WINDOW/KEYBD_ES.table.lz"
	.else
	.incbin "GRAPHICS/WINDOW/KEYBOARD.table.lz"
	.endif
	.balign 4, 0
	.global Ui_FixedTileBlocks
Ui_FixedTileBlocks:
	.4byte Resource_FixedBlockBTiles
