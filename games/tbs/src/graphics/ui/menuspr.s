	.section .rodata
	.balign 4
	.global UiIcon_ResourceTiles
UiIcon_ResourceTiles:
	.incbin "graphics/ui/menuspr/hand.4bpp16x16"
	.incbin "graphics/ui/menuspr/gem.4bpp16x16"
	.global Menu_SmallDownArrowTiles
Menu_SmallDownArrowTiles:
	.incbin "graphics/ui/menuspr/sdown.4bpp16x16"
	.global Menu_ArrowTiles
Menu_ArrowTiles:
	.incbin "graphics/ui/menuspr/sup.4bpp16x16"
	.incbin "graphics/ui/menuspr/up.4bpp16x16"
	.incbin "graphics/ui/menuspr/down.4bpp16x16"
	.global Menu_PairSpriteATiles
Menu_PairSpriteATiles:
	.incbin "common/menu/djinn/arrowup.4bpp16x16"
	.global Menu_PairSpriteBTiles
Menu_PairSpriteBTiles:
	.incbin "common/menu/djinn/arrowdn.4bpp16x16"
	.incbin "graphics/ui/menuspr/digits.4bpp"
	.incbin "graphics/ui/menuspr/price.4bpp"
	.global Menu_QuantityTiles
Menu_QuantityTiles:
	.incbin "graphics/ui/menuspr/quantity.4bpp"
	.incbin "graphics/ui/menuspr/sprite_c.4bpp16x16"
