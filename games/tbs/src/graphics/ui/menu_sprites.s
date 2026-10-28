	.section .rodata
	.balign 4
	.global gIcon_ResourceTiles
gIcon_ResourceTiles:
	.incbin "games/tbs/src/graphics/ui/menu_sprites/hand.4bpp16x16"
	.incbin "games/tbs/src/graphics/ui/menu_sprites/gem.4bpp16x16"
	.global gMenu_SmallDownArrowTiles
gMenu_SmallDownArrowTiles:
	.incbin "games/tbs/src/graphics/ui/menu_sprites/sdown.4bpp16x16"
	.global gMenu_ArrowTiles
gMenu_ArrowTiles:
	.incbin "games/tbs/src/graphics/ui/menu_sprites/sup.4bpp16x16"
	.incbin "games/tbs/src/graphics/ui/menu_sprites/up.4bpp16x16"
	.incbin "games/tbs/src/graphics/ui/menu_sprites/down.4bpp16x16"
	.global gMenu_PairSpriteATiles
gMenu_PairSpriteATiles:
	.incbin "games/tbs/src/graphics/ui/djinn_arrow_up.4bpp16x16"
	.global gMenu_PairSpriteBTiles
gMenu_PairSpriteBTiles:
	.incbin "games/tbs/src/graphics/ui/djinn_arrow_down.4bpp16x16"
	.incbin "games/tbs/src/graphics/ui/menu_sprites/digits.4bpp"
	.incbin "games/tbs/src/graphics/ui/menu_sprites/price.4bpp"
	.global gMenu_QuantityTiles
gMenu_QuantityTiles:
	.incbin "games/tbs/src/graphics/ui/menu_sprites/quantity.4bpp"
	.incbin "games/tbs/src/graphics/ui/menu_sprites/sprite_c.4bpp16x16"
