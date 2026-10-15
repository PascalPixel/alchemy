	.section .rodata
	.global Tileset_Set104TilesB
Tileset_Set104TilesB:
	.incbin "games/tla/src/graphics/tileset/set_104/tiles_b.4bpp.mtf"
	.byte 0x80, 0x62, 0xc6	@ PACKER: left in the buffer by Tileset_Set104TilesA+0x1471
