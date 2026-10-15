	.section .rodata
	.global Tileset_Set88TilesC
Tileset_Set88TilesC:
	.incbin "games/tla/src/graphics/tileset/set_88/tiles_c.4bpp.mtf"
	.byte 0x84	@ PACKER: left in the buffer by Tileset_Set88TilesB+0x1d5b
