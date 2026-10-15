	.section .rodata
	.balign 4
	.global Shop_GlyphBytes
Shop_GlyphBytes:
	.incbin "games/common/src/graphics/ui/menuspr/digits.4bpp"
	.global Shop_PriceTiles
Shop_PriceTiles:
	.incbin "games/common/src/graphics/ui/menuspr/price.4bpp"
	.global Shop_QuantityTiles
Shop_QuantityTiles:
	.incbin "games/common/src/graphics/ui/menuspr/quantity.4bpp"
