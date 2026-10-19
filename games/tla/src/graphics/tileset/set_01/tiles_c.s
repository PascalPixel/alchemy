	.section .rodata
	.global Tileset_Set01TilesC
Tileset_Set01TilesC:
	.incbin "games/tla/src/graphics/tileset/set_01/tiles_c.4bpp.mtf"
	.byte 0x3e	@ PACKER: left in the buffer by Tileset_Set01TilesB+0x50b
