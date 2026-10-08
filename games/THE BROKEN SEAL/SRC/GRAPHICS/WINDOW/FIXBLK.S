	.section .rodata
	.4byte 0
	.global Resource_FixedBlockBTiles
Resource_FixedBlockBTiles:
	.incbin "GRAPHICS/WINDOW/FIXBLKB.4bpp16x16"
	.global DjinnMenu_ArrowUpTiles
DjinnMenu_ArrowUpTiles:
	.incbin "MENU/DJINN/ARROWUP.4bpp16x16"
	.global DjinnMenu_ArrowDownTiles
DjinnMenu_ArrowDownTiles:
	.incbin "MENU/DJINN/ARROWDN.4bpp16x16"
	.incbin "MENU/DJINN/GLYPHBAR.4bpp"
	.global Resource_FixedBlockATiles
Resource_FixedBlockATiles:
	.incbin "GRAPHICS/WINDOW/FIXBLKA.4bpp16x16"
