	.section .rodata
	.4byte 0
	.global Resource_FixedBlockBTiles
Resource_FixedBlockBTiles:
	.incbin "games/common/src/graphics/ui/menuspr/hand.4bpp16x16"
	.global DjinnMenu_ArrowUpTiles
DjinnMenu_ArrowUpTiles:
	.incbin "games/common/src/menu/djinn/arrowup.4bpp16x16"
	.global DjinnMenu_ArrowDownTiles
DjinnMenu_ArrowDownTiles:
	.incbin "games/common/src/menu/djinn/arrowdn.4bpp16x16"
