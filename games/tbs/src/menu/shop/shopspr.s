	.section .rodata
	.balign 4
	.global Shop_HandTiles
Shop_HandTiles:
	.incbin "graphics/ui/menuspr/hand.4bpp16x16"
	.global Shop_GemTiles
Shop_GemTiles:
	.incbin "graphics/ui/menuspr/gem.4bpp16x16"
	.global Shop_SmallDownArrowTiles
Shop_SmallDownArrowTiles:
	.incbin "graphics/ui/menuspr/sdown.4bpp16x16"
	.global Shop_SmallUpArrowTiles
Shop_SmallUpArrowTiles:
	.incbin "graphics/ui/menuspr/sup.4bpp16x16"
	.global Shop_UpArrowTiles
Shop_UpArrowTiles:
	.incbin "graphics/ui/menuspr/up.4bpp16x16"
	.global Shop_DownArrowTiles
Shop_DownArrowTiles:
	.incbin "graphics/ui/menuspr/down.4bpp16x16"
	.incbin "graphics/ui/menuspr/sprite_a.4bpp16x16"
	.incbin "graphics/ui/menuspr/sprite_b.4bpp16x16"
	.global Shop_GlyphBytes
Shop_GlyphBytes:
	.incbin "graphics/ui/menuspr/digits.4bpp"
	.global Shop_PriceTiles
Shop_PriceTiles:
	.incbin "graphics/ui/menuspr/price.4bpp"
	.global Shop_QuantityTiles
Shop_QuantityTiles:
	.incbin "graphics/ui/menuspr/quantity.4bpp"
	.incbin "graphics/ui/menuspr/sprite_c.4bpp16x16"
