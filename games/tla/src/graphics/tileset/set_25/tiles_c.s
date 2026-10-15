	.section .rodata
	.global Tileset_Set25TilesC
Tileset_Set25TilesC:
	.incbin "games/tla/src/graphics/tileset/set_25/tiles_c.4bpp.mtf"
	.byte 0x19	@ PACKER: left in the buffer by Tileset_Set25TilesB+0x60f
