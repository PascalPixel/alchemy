	.section .rodata
	.balign 4
	.global UiIcon_ResourceTiles
UiIcon_ResourceTiles:
	.incbin "games/tbs/src/graphics/ui/menuspr/hand.4bpp16x16"
	.incbin "games/tbs/src/graphics/ui/menuspr/gem.4bpp16x16"
	.global Menu_SmallDownArrowTiles
Menu_SmallDownArrowTiles:
	.incbin "games/tbs/src/graphics/ui/menuspr/sdown.4bpp16x16"
	.global Menu_ArrowTiles
Menu_ArrowTiles:
	.incbin "games/tbs/src/graphics/ui/menuspr/sup.4bpp16x16"
	.incbin "games/tbs/src/graphics/ui/menuspr/up.4bpp16x16"
	.incbin "games/tbs/src/graphics/ui/menuspr/down.4bpp16x16"
	.global Menu_PairSpriteATiles
Menu_PairSpriteATiles:
	.incbin "games/common/src/menu/djinn/arrowup.4bpp16x16"
	.global Menu_PairSpriteBTiles
Menu_PairSpriteBTiles:
	.incbin "games/common/src/menu/djinn/arrowdn.4bpp16x16"
	.incbin "games/tbs/src/graphics/ui/menuspr/digits.4bpp"
	.incbin "games/tbs/src/graphics/ui/menuspr/price.4bpp"
	.global Menu_QuantityTiles
Menu_QuantityTiles:
	.incbin "games/tbs/src/graphics/ui/menuspr/quantity.4bpp"
	.incbin "games/tbs/src/graphics/ui/menuspr/sprite_c.4bpp16x16"
