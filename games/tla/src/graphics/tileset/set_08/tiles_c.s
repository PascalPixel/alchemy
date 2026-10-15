	.section .rodata
	.global Tileset_Set08TilesC
Tileset_Set08TilesC:
	.incbin "games/tla/src/graphics/tileset/set_08/tiles_c.4bpp.mtf"
	.byte 0x8e, 0x9f	@ PACKER: left in the buffer by Tileset_Set08TilesB+0xf9a
