	.section .rodata
	.global Tileset_Set113TilesC
Tileset_Set113TilesC:
	.incbin "games/tla/src/graphics/tileset/set_113/tiles_c.4bpp.mtf"
	.byte 0x06, 0x6b, 0x35	@ PACKER: left in the buffer by Tileset_Set113TilesB+0xbe5
