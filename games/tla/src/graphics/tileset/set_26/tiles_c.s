	.section .rodata
	.global Tileset_Set26TilesC
Tileset_Set26TilesC:
	.incbin "games/tla/src/graphics/tileset/set_26/tiles_c.4bpp.mtf"
	.byte 0xa4	@ PACKER: left in the buffer by Tileset_Set26TilesB+0x60f
