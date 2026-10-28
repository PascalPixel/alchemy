	.section .rodata
	.4byte 0
	.global gResource_FixedBlockBTiles
gResource_FixedBlockBTiles:
	.incbin "games/tla/src/graphics/ui/menu_sprites/hand.4bpp16x16"
	.global gDjinnMenu_ArrowUpTiles
gDjinnMenu_ArrowUpTiles:
	.incbin "games/tla/src/graphics/ui/djinn_arrow_up.4bpp16x16"
	.global gDjinnMenu_ArrowDownTiles
gDjinnMenu_ArrowDownTiles:
	.incbin "games/tla/src/graphics/ui/djinn_arrow_down.4bpp16x16"
