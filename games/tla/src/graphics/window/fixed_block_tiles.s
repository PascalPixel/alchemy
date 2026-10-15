	.section .rodata
	.4byte 0
	.global gResource_FixedBlockBTiles
gResource_FixedBlockBTiles:
	.incbin "games/common/src/graphics/ui/menuspr/hand.4bpp16x16"
	.global gDjinnMenu_ArrowUpTiles
gDjinnMenu_ArrowUpTiles:
	.incbin "games/common/src/menu/djinn/arrowup.4bpp16x16"
	.global gDjinnMenu_ArrowDownTiles
gDjinnMenu_ArrowDownTiles:
	.incbin "games/common/src/menu/djinn/arrowdn.4bpp16x16"
