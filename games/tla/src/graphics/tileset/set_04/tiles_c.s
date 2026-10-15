	.section .rodata
	.global Tileset_Set04TilesC
Tileset_Set04TilesC:
	.incbin "games/tla/src/graphics/tileset/set_04/tiles_c.4bpp.mtf"
	.byte 0x27, 0xe9, 0x0b	@ PACKER: left in the buffer by Tileset_Set04TilesA+0x2249
