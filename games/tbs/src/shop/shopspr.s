	.section .rodata
	.balign 4
	.global gShop_HandTiles
gShop_HandTiles:
	.incbin "games/common/src/graphics/ui/menuspr/hand.4bpp16x16"
	.global gShop_GemTiles
gShop_GemTiles:
	.incbin "games/common/src/graphics/ui/menuspr/gem.4bpp16x16"
	.global gShop_SmallDownArrowTiles
gShop_SmallDownArrowTiles:
	.incbin "games/common/src/graphics/ui/menuspr/sdown.4bpp16x16"
	.global gShop_SmallUpArrowTiles
gShop_SmallUpArrowTiles:
	.incbin "games/common/src/graphics/ui/menuspr/sup.4bpp16x16"
	.global gShop_UpArrowTiles
gShop_UpArrowTiles:
	.incbin "games/common/src/graphics/ui/menuspr/up.4bpp16x16"
	.global gShop_DownArrowTiles
gShop_DownArrowTiles:
	.incbin "games/common/src/graphics/ui/menuspr/down.4bpp16x16"
	.incbin "games/common/src/menu/djinn/arrowup.4bpp16x16"
	.incbin "games/common/src/menu/djinn/arrowdn.4bpp16x16"
	.global gShop_GlyphBytes
gShop_GlyphBytes:
	.incbin "games/common/src/graphics/ui/menuspr/digits.4bpp"
	.global gShop_PriceTiles
gShop_PriceTiles:
	.incbin "games/common/src/graphics/ui/menuspr/price.4bpp"
	.global gShop_QuantityTiles
gShop_QuantityTiles:
	.incbin "games/common/src/graphics/ui/menuspr/quantity.4bpp"
	.incbin "games/common/src/graphics/ui/menuspr/sprite_c.4bpp16x16"
