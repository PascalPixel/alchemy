	.section .rodata
	.global Tileset_Set72TilesC
Tileset_Set72TilesC:
	.incbin "games/tla/src/graphics/tileset/set_72/tiles_c.4bpp.mtf"
	.byte 0x05, 0xb0, 0x73	@ PACKER: left in the buffer by Tileset_Set72TilesA+0x1cf5
