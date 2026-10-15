	.section .rodata
	.global Tileset_Set107TilesC
Tileset_Set107TilesC:
	.incbin "games/tla/src/graphics/tileset/set_107/tiles_c.4bpp.mtf"
	.byte 0x65, 0xab, 0x31	@ PACKER: left in the buffer by Tileset_Set107TilesA+0x26bd
