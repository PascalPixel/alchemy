	.section .rodata
	.balign 4
	.global gShop_GlyphBytes
gShop_GlyphBytes:
	.incbin "games/tla/src/graphics/ui/menu_sprites/digits.4bpp"
	.global gShop_PriceTiles
gShop_PriceTiles:
	.incbin "games/tla/src/graphics/ui/menu_sprites/price.4bpp"
	.global gShop_QuantityTiles
gShop_QuantityTiles:
	.incbin "games/tla/src/graphics/ui/menu_sprites/quantity.4bpp"
