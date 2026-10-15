	.section .rodata
	.balign 4
	.global gUiIcon_ResourceTiles
gUiIcon_ResourceTiles:
	.incbin "games/common/src/graphics/ui/menuspr/hand.4bpp16x16"
	.incbin "games/common/src/graphics/ui/menuspr/gem.4bpp16x16"
	.global gMenu_SmallDownArrowTiles
gMenu_SmallDownArrowTiles:
	.incbin "games/common/src/graphics/ui/menuspr/sdown.4bpp16x16"
	.global gMenu_ArrowTiles
gMenu_ArrowTiles:
	.incbin "games/common/src/graphics/ui/menuspr/sup.4bpp16x16"
	.incbin "games/common/src/graphics/ui/menuspr/up.4bpp16x16"
	.incbin "games/common/src/graphics/ui/menuspr/down.4bpp16x16"
	.global gMenu_PairSpriteATiles
gMenu_PairSpriteATiles:
	.incbin "games/common/src/menu/djinn/arrowup.4bpp16x16"
	.global gMenu_PairSpriteBTiles
gMenu_PairSpriteBTiles:
	.incbin "games/common/src/menu/djinn/arrowdn.4bpp16x16"
	.incbin "games/common/src/graphics/ui/menuspr/digits.4bpp"
	.incbin "games/common/src/graphics/ui/menuspr/price.4bpp"
	.global gMenu_QuantityTiles
gMenu_QuantityTiles:
	.incbin "games/common/src/graphics/ui/menuspr/quantity.4bpp"
	.incbin "games/common/src/graphics/ui/menuspr/sprite_c.4bpp16x16"
