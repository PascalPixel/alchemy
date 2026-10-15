	.section .rodata
	.global Tileset_Set102TilesC
Tileset_Set102TilesC:
	.incbin "games/tla/src/graphics/tileset/set_102/tiles_c.4bpp.mtf"
	.byte 0x0b, 0x24	@ PACKER: left in the buffer by Tileset_Set102TilesB+0x1632
