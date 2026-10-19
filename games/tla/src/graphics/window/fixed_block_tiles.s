	.section .rodata
	.4byte 0
	.global gResource_FixedBlockBTiles
gResource_FixedBlockBTiles:
	.incbin "games/tla/src/graphics/ui/menuspr/hand.4bpp16x16"
	.global gDjinnMenu_ArrowUpTiles
gDjinnMenu_ArrowUpTiles:
	.incbin "games/tla/src/menu/djinn/arrowup.4bpp16x16"
	.global gDjinnMenu_ArrowDownTiles
gDjinnMenu_ArrowDownTiles:
	.incbin "games/tla/src/menu/djinn/arrowdn.4bpp16x16"
