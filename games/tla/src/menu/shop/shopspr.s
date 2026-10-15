	.section .rodata
	.balign 4
	.global gShop_GlyphBytes
gShop_GlyphBytes:
	.incbin "games/common/src/graphics/ui/menuspr/digits.4bpp"
	.global gShop_PriceTiles
gShop_PriceTiles:
	.incbin "games/common/src/graphics/ui/menuspr/price.4bpp"
	.global gShop_QuantityTiles
gShop_QuantityTiles:
	.incbin "games/common/src/graphics/ui/menuspr/quantity.4bpp"
