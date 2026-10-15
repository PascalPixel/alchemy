	.section .rodata
	.global Tileset_Set80TilesC
Tileset_Set80TilesC:
	.incbin "games/tla/src/graphics/tileset/set_80/tiles_c.4bpp.mtf"
	.byte 0xf8	@ PACKER: left in the buffer by Tileset_Set80TilesB+0xfb7
