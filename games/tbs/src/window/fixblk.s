	.section .rodata
	.4byte 0
	.global Resource_FixedBlockBTiles
Resource_FixedBlockBTiles:
	.incbin "window/fixblkb.4bpp16x16"
	.global DjinnMenu_ArrowUpTiles
DjinnMenu_ArrowUpTiles:
	.incbin "common/menu/djinn/arrowup.4bpp16x16"
	.global DjinnMenu_ArrowDownTiles
DjinnMenu_ArrowDownTiles:
	.incbin "common/menu/djinn/arrowdn.4bpp16x16"
	.incbin "window/glyphbar.4bpp"
	.global Resource_FixedBlockATiles
Resource_FixedBlockATiles:
	.incbin "window/fixblka.4bpp16x16"
