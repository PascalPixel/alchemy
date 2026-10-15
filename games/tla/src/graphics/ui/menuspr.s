	.section .rodata
@ The menus' 16x16 sprites in 16 colours, each packed on its own as a
@ palette-LZ stream: The Broken Seal's menu sprite pictures, which it keeps
@ unpacked (its menuspr.s). Resource 1F9 holds the first eight streams one
@ after another, twice, and then the ninth, with no rounding between them;
@ no loader of it was found.
@ Streams are rounded to four bytes; PACKER rows name the buffer's last
@ writer, and zero fill where no earlier stream has the bytes.
	.global Resource_Data1F9
Resource_Data1F9:
	.rept 2
	.incbin "games/common/src/graphics/ui/menuspr/hand.4bpp16x16.plz"
	.incbin "games/common/src/graphics/ui/menuspr/gem.4bpp16x16.plz"
	.incbin "games/common/src/graphics/ui/menuspr/sdown.4bpp16x16.plz"
	.incbin "games/common/src/graphics/ui/menuspr/sup.4bpp16x16.plz"
	.incbin "games/common/src/graphics/ui/menuspr/up.4bpp16x16.plz"
	.incbin "games/common/src/graphics/ui/menuspr/down.4bpp16x16.plz"
	.incbin "games/common/src/menu/djinn/arrowup.4bpp16x16.plz"
	.incbin "games/common/src/menu/djinn/arrowdn.4bpp16x16.plz"
	.endr
	.incbin "games/common/src/graphics/ui/menuspr/sprite_c.4bpp16x16.plz"
	.byte 0x7d, 0x06	@ PACKER: left in the buffer by Ui_CommandIcons+0x55e
	.global Resource_Data1FA
Resource_Data1FA:
	.incbin "games/common/src/graphics/ui/menuspr/hand.4bpp16x16.plz"
	.byte 0x55, 0x00, 0x05	@ PACKER: left in the buffer by Resource_Data1F9+0x71
	.global Resource_Data1FB
Resource_Data1FB:
	.incbin "games/common/src/graphics/ui/menuspr/gem.4bpp16x16.plz"
	.global Resource_Data1FC
Resource_Data1FC:
	.incbin "games/common/src/graphics/ui/menuspr/sdown.4bpp16x16.plz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Resource_Data1FD
Resource_Data1FD:
	.incbin "games/common/src/graphics/ui/menuspr/sup.4bpp16x16.plz"
	.byte 0x00	@ PACKER: left in the buffer by Resource_Data1FC+0x4f
	.global Resource_Data1FE
Resource_Data1FE:
	.incbin "games/common/src/graphics/ui/menuspr/up.4bpp16x16.plz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Resource_Data1FF
Resource_Data1FF:
	.incbin "games/common/src/graphics/ui/menuspr/down.4bpp16x16.plz"
	.byte 0x00	@ PACKER: left in the buffer by Resource_Data1FE+0x57
	.global Resource_Data200
Resource_Data200:
	.incbin "games/common/src/menu/djinn/arrowup.4bpp16x16.plz"
	.byte 0x03, 0xf5	@ PACKER: left in the buffer by Resource_Data1FF+0x3e
	.global Resource_Data201
Resource_Data201:
	.incbin "games/common/src/menu/djinn/arrowdn.4bpp16x16.plz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Resource_Data202
Resource_Data202:
	.incbin "games/common/src/graphics/ui/menuspr/sprite_c.4bpp16x16.plz"
