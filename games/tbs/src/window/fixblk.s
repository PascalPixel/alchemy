	.section .rodata
	.4byte 0
	.global Resource_FixedBlockBTiles
Resource_FixedBlockBTiles:
	.incbin "games/tbs/src/window/fixblkb.4bpp16x16"
	.global DjinnMenu_ArrowUpTiles
DjinnMenu_ArrowUpTiles:
	.incbin "games/common/src/menu/djinn/arrowup.4bpp16x16"
	.global DjinnMenu_ArrowDownTiles
DjinnMenu_ArrowDownTiles:
	.incbin "games/common/src/menu/djinn/arrowdn.4bpp16x16"
	.incbin "games/tbs/src/window/glyphbar.4bpp"
	.global Resource_FixedBlockATiles
Resource_FixedBlockATiles:
	.incbin "games/tbs/src/window/fixblka.4bpp16x16"
