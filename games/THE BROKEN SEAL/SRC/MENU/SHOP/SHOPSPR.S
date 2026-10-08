	.section .rodata
	.balign 4
	.global Shop_HandTiles
Shop_HandTiles:
	.incbin "GRAPHICS/UI/MENUSPR/HAND.4bpp16x16"
	.global Shop_GemTiles
Shop_GemTiles:
	.incbin "GRAPHICS/UI/MENUSPR/GEM.4bpp16x16"
	.global Shop_SmallDownArrowTiles
Shop_SmallDownArrowTiles:
	.incbin "GRAPHICS/UI/MENUSPR/SDOWN.4bpp16x16"
	.global Shop_SmallUpArrowTiles
Shop_SmallUpArrowTiles:
	.incbin "GRAPHICS/UI/MENUSPR/SUP.4bpp16x16"
	.global Shop_UpArrowTiles
Shop_UpArrowTiles:
	.incbin "GRAPHICS/UI/MENUSPR/UP.4bpp16x16"
	.global Shop_DownArrowTiles
Shop_DownArrowTiles:
	.incbin "GRAPHICS/UI/MENUSPR/DOWN.4bpp16x16"
	.incbin "GRAPHICS/UI/MENUSPR/SPRITE_A.4bpp16x16"
	.incbin "GRAPHICS/UI/MENUSPR/SPRITE_B.4bpp16x16"
	.global Shop_GlyphBytes
Shop_GlyphBytes:
	.incbin "GRAPHICS/UI/MENUSPR/DIGITS.4bpp"
	.global Shop_PriceTiles
Shop_PriceTiles:
	.incbin "GRAPHICS/UI/MENUSPR/PRICE.4bpp"
	.global Shop_QuantityTiles
Shop_QuantityTiles:
	.incbin "GRAPHICS/UI/MENUSPR/QUANTITY.4bpp"
	.incbin "GRAPHICS/UI/MENUSPR/SPRITE_C.4bpp16x16"
