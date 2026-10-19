	.section .rodata
	.4byte 0
	.global gResource_FixedBlockBTiles
gResource_FixedBlockBTiles:
	.incbin "games/tbs/src/window/fixed_block_b.4bpp16x16"
	.global gDjinnMenu_ArrowUpTiles
gDjinnMenu_ArrowUpTiles:
	.incbin "games/tbs/src/menu/djinn/arrowup.4bpp16x16"
	.global gDjinnMenu_ArrowDownTiles
gDjinnMenu_ArrowDownTiles:
	.incbin "games/tbs/src/menu/djinn/arrowdn.4bpp16x16"
	.incbin "games/tbs/src/window/glyphbar.4bpp"
	.global gResource_FixedBlockATiles
gResource_FixedBlockATiles:
	.incbin "games/tbs/src/window/fixed_block_a.4bpp16x16"
