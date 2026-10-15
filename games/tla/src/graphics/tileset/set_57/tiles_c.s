	.section .rodata
	.global Tileset_Set57TilesC
Tileset_Set57TilesC:
	.incbin "games/tla/src/graphics/tileset/set_57/tiles_c.4bpp.mtf"
	.byte 0xff	@ PACKER: left in the buffer by Tileset_Set57TilesB+0x293
