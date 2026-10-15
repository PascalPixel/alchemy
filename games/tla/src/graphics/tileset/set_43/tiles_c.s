	.section .rodata
	.global Tileset_Set43TilesC
Tileset_Set43TilesC:
	.incbin "games/tla/src/graphics/tileset/set_43/tiles_c.4bpp.mtf"
	.byte 0x8a, 0xbc, 0xc1	@ PACKER: left in the buffer by Tileset_Set43TilesB+0xd89
