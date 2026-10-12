	.section .rodata
	.4byte 0
	.global Resource_FixedBlockBTiles
Resource_FixedBlockBTiles:
	.incbin "window/fixblkb.4bpp16x16"
	.global DjinnMenu_ArrowUpTiles
DjinnMenu_ArrowUpTiles:
	.incbin "menu/djinn/arrowup.4bpp16x16"
	.global DjinnMenu_ArrowDownTiles
DjinnMenu_ArrowDownTiles:
	.incbin "menu/djinn/arrowdn.4bpp16x16"
	.incbin "menu/djinn/glyphbar.4bpp"
	.global Resource_FixedBlockATiles
Resource_FixedBlockATiles:
	.incbin "window/fixblka.4bpp16x16"
