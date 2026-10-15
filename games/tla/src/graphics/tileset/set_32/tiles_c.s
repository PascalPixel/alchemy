	.section .rodata
	.global Tileset_Set32TilesC
Tileset_Set32TilesC:
	.incbin "games/tla/src/graphics/tileset/set_32/tiles_c.4bpp.mtf"
	.byte 0xe4, 0x78, 0x1a	@ PACKER: left in the buffer by Tileset_Set32TilesB+0x675
