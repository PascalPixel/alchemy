	.section .rodata
	.balign 4
	.global gMenuCursor_HandTiles
gMenuCursor_HandTiles:
	.incbin "games/tbs/src/graphics/ui/menu_sprites/hand.4bpp16x16"
	.global gShop_GemTiles
gShop_GemTiles:
	.incbin "games/tbs/src/graphics/ui/menu_sprites/gem.4bpp16x16"
	.global gShop_SmallDownArrowTiles
gShop_SmallDownArrowTiles:
	.incbin "games/tbs/src/graphics/ui/menu_sprites/sdown.4bpp16x16"
	.global gShop_SmallUpArrowTiles
gShop_SmallUpArrowTiles:
	.incbin "games/tbs/src/graphics/ui/menu_sprites/sup.4bpp16x16"
	.global gShop_UpArrowTiles
gShop_UpArrowTiles:
	.incbin "games/tbs/src/graphics/ui/menu_sprites/up.4bpp16x16"
	.global gShop_DownArrowTiles
gShop_DownArrowTiles:
	.incbin "games/tbs/src/graphics/ui/menu_sprites/down.4bpp16x16"
	.incbin "games/tbs/src/graphics/ui/djinn_arrow_up.4bpp16x16"
	.incbin "games/tbs/src/graphics/ui/djinn_arrow_down.4bpp16x16"
	.global gShop_GlyphBytes
gShop_GlyphBytes:
	.incbin "games/tbs/src/graphics/ui/menu_sprites/digits.4bpp"
	.global gShop_PriceTiles
gShop_PriceTiles:
	.incbin "games/tbs/src/graphics/ui/menu_sprites/price.4bpp"
	.global gShop_QuantityTiles
gShop_QuantityTiles:
	.incbin "games/tbs/src/graphics/ui/menu_sprites/quantity.4bpp"
	.incbin "games/tbs/src/graphics/ui/menu_sprites/sprite_c.4bpp16x16"
