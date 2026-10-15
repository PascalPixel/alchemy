	.section .rodata
	.global Tileset_Set109TilesC
Tileset_Set109TilesC:
	.incbin "games/tla/src/graphics/tileset/set_109/tiles_c.4bpp.mtf"
	.byte 0x10	@ PACKER: left in the buffer by Tileset_Set109TilesB+0x16fb
