	.section .rodata
	.global Tileset_Set82TilesC
Tileset_Set82TilesC:
	.incbin "games/tla/src/graphics/tileset/set_82/tiles_c.4bpp.mtf"
	.byte 0x24	@ PACKER: left in the buffer by Tileset_Set82TilesB+0x1ce7
