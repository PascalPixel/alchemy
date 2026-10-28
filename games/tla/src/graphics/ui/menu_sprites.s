	.section .rodata
	.balign 4
@ The menus' price digits, price sprite and quantity sprite, the shop's sheets
	.incbin "games/tla/src/graphics/ui/menu_sprites/digits.4bpp"
	.incbin "games/tla/src/graphics/ui/menu_sprites/price.4bpp"
	.global gMenu_QuantityTiles
gMenu_QuantityTiles:
	.incbin "games/tla/src/graphics/ui/menu_sprites/quantity.4bpp"
