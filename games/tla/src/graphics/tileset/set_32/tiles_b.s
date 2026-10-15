	.section .rodata
	.global Tileset_Set32TilesB
Tileset_Set32TilesB:
	.incbin "games/tla/src/graphics/tileset/set_32/tiles_b.4bpp.mtf"
	.byte 0x13, 0xf2	@ PACKER: left in the buffer by Tileset_Set32TilesA+0xaea
