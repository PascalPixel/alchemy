	.section .rodata
	.4byte 0
	.global PointingHandTiles
PointingHandTiles:
	.incbin "games/tla/src/graphics/ui/menu_sprites/hand.4bpp16x16"
	.global DjinnMenuArrowUpTiles
DjinnMenuArrowUpTiles:
	.incbin "games/tla/src/graphics/ui/djinn_arrow_up.4bpp16x16"
	.global DjinnMenuArrowDownTiles
DjinnMenuArrowDownTiles:
	.incbin "games/tla/src/graphics/ui/djinn_arrow_down.4bpp16x16"
