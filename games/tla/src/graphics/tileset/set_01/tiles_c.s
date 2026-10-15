	.section .rodata
	.global Tileset_Set01TilesC
Tileset_Set01TilesC:
	.incbin "games/common/src/graphics/tileset/set_07/tiles_c.4bpp.mtf"
	.byte 0x3e	@ PACKER: left in the buffer by Tileset_Set01TilesB+0x50b
