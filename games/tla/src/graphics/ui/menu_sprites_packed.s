	.section .rodata
@ The menus' 16x16 sprites in 16 colours, each packed on its own as a
@ palette-LZ stream: The Broken Seal's menu sprite pictures, which it keeps
@ unpacked (its menu_sprites.s). MenuSprites_Unused holds the first eight streams one
@ after another, twice, and then the ninth, with no rounding between them;
@ no loader of it was found.
@ Streams are rounded to four bytes; PACKER rows name the buffer's last
@ writer, and zero fill where no earlier stream has the bytes.
	.global MenuSprites_Unused
MenuSprites_Unused:
	.rept 2
	.incbin "games/tla/src/graphics/ui/menu_sprites/hand.4bpp16x16.plz"
	.incbin "games/tla/src/graphics/ui/menu_sprites/gem.4bpp16x16.plz"
	.incbin "games/tla/src/graphics/ui/menu_sprites/sdown.4bpp16x16.plz"
	.incbin "games/tla/src/graphics/ui/menu_sprites/sup.4bpp16x16.plz"
	.incbin "games/tla/src/graphics/ui/menu_sprites/up.4bpp16x16.plz"
	.incbin "games/tla/src/graphics/ui/menu_sprites/down.4bpp16x16.plz"
	.incbin "games/tla/src/graphics/ui/djinn_arrow_up.4bpp16x16.plz"
	.incbin "games/tla/src/graphics/ui/djinn_arrow_down.4bpp16x16.plz"
	.endr
	.incbin "games/tla/src/graphics/ui/menu_sprites/sprite_c.4bpp16x16.plz"
	.byte 0x7d, 0x06	@ PACKER: left in the buffer by Ui_CommandIcons+0x55e
	.global MenuSprites_UiIconTiles
MenuSprites_UiIconTiles:
	.incbin "games/tla/src/graphics/ui/menu_sprites/hand.4bpp16x16.plz"
	.byte 0x55, 0x00, 0x05	@ PACKER: left in the buffer by MenuSprites_Unused+0x71
	.global MenuSprites_ShopGemTiles
MenuSprites_ShopGemTiles:
	.incbin "games/tla/src/graphics/ui/menu_sprites/gem.4bpp16x16.plz"
	.global MenuSprites_SmallDownArrowTiles
MenuSprites_SmallDownArrowTiles:
	.incbin "games/tla/src/graphics/ui/menu_sprites/sdown.4bpp16x16.plz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global MenuSprites_ArrowTiles
MenuSprites_ArrowTiles:
	.incbin "games/tla/src/graphics/ui/menu_sprites/sup.4bpp16x16.plz"
	.byte 0x00	@ PACKER: left in the buffer by MenuSprites_SmallDownArrowTiles+0x4f
	.global MenuSprites_ShopUpArrowTiles
MenuSprites_ShopUpArrowTiles:
	.incbin "games/tla/src/graphics/ui/menu_sprites/up.4bpp16x16.plz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global MenuSprites_ShopDownArrowTiles
MenuSprites_ShopDownArrowTiles:
	.incbin "games/tla/src/graphics/ui/menu_sprites/down.4bpp16x16.plz"
	.byte 0x00	@ PACKER: left in the buffer by MenuSprites_ShopUpArrowTiles+0x57
	.global MenuSprites_ItemMenuTilesA
MenuSprites_ItemMenuTilesA:
	.incbin "games/tla/src/graphics/ui/djinn_arrow_up.4bpp16x16.plz"
	.byte 0x03, 0xf5	@ PACKER: left in the buffer by MenuSprites_ShopDownArrowTiles+0x3e
	.global MenuSprites_ItemMenuTilesB
MenuSprites_ItemMenuTilesB:
	.incbin "games/tla/src/graphics/ui/djinn_arrow_down.4bpp16x16.plz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global MenuSprites_NameCaretTiles
MenuSprites_NameCaretTiles:
	.incbin "games/tla/src/graphics/ui/menu_sprites/sprite_c.4bpp16x16.plz"
