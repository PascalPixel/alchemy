	.section .rodata
	.balign 4
@ The menus' price digits, price sprite and quantity sprite, the shop's sheets
	.incbin "games/common/src/graphics/ui/menuspr/digits.4bpp"
	.incbin "games/common/src/graphics/ui/menuspr/price.4bpp"
	.global gMenu_QuantityTiles
gMenu_QuantityTiles:
	.incbin "games/common/src/graphics/ui/menuspr/quantity.4bpp"
