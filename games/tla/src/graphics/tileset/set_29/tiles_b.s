	.section .rodata
	.global Tileset_Set29TilesB
Tileset_Set29TilesB:
	.incbin "games/tla/src/graphics/tileset/set_29/tiles_b.4bpp.mtf"
	.byte 0xcf, 0xde	@ PACKER: left in the buffer by Tileset_Set29TilesA+0x27be
