	.section .rodata
	.global Tileset_Set02TilesC
Tileset_Set02TilesC:
	.incbin "games/tla/src/graphics/tileset/set_02/tiles_c.4bpp.mtf"
	.byte 0x08	@ PACKER: left in the buffer by Tileset_Set02TilesB+0x151f
