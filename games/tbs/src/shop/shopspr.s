	.section .rodata
	.balign 4
	.global Shop_HandTiles
Shop_HandTiles:
	.incbin "games/common/src/graphics/ui/menuspr/hand.4bpp16x16"
	.global Shop_GemTiles
Shop_GemTiles:
	.incbin "games/common/src/graphics/ui/menuspr/gem.4bpp16x16"
	.global Shop_SmallDownArrowTiles
Shop_SmallDownArrowTiles:
	.incbin "games/common/src/graphics/ui/menuspr/sdown.4bpp16x16"
	.global Shop_SmallUpArrowTiles
Shop_SmallUpArrowTiles:
	.incbin "games/common/src/graphics/ui/menuspr/sup.4bpp16x16"
	.global Shop_UpArrowTiles
Shop_UpArrowTiles:
	.incbin "games/common/src/graphics/ui/menuspr/up.4bpp16x16"
	.global Shop_DownArrowTiles
Shop_DownArrowTiles:
	.incbin "games/common/src/graphics/ui/menuspr/down.4bpp16x16"
	.incbin "games/common/src/menu/djinn/arrowup.4bpp16x16"
	.incbin "games/common/src/menu/djinn/arrowdn.4bpp16x16"
	.global Shop_GlyphBytes
Shop_GlyphBytes:
	.incbin "games/tbs/src/graphics/ui/menuspr/digits.4bpp"
	.global Shop_PriceTiles
Shop_PriceTiles:
	.incbin "games/tbs/src/graphics/ui/menuspr/price.4bpp"
	.global Shop_QuantityTiles
Shop_QuantityTiles:
	.incbin "games/tbs/src/graphics/ui/menuspr/quantity.4bpp"
	.incbin "games/common/src/graphics/ui/menuspr/sprite_c.4bpp16x16"
