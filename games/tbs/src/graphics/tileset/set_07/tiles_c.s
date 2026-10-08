	.section .rodata
	.global Tileset_Set07TilesC
Tileset_Set07TilesC:
	.incbin "common/graphics/tileset/set_07/tiles_c.4bpp.mtf"
	.byte 0x3e	@ PACKER: left in the buffer by Tileset_Set07TilesB+0x50b
