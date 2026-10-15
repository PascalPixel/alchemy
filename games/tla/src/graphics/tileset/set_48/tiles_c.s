	.section .rodata
	.global Tileset_Set48TilesC
Tileset_Set48TilesC:
	.incbin "games/tla/src/graphics/tileset/set_48/tiles_c.4bpp.mtf"
	.byte 0x8f, 0xa4	@ PACKER: left in the buffer by Tileset_Set48TilesB+0xdf2
