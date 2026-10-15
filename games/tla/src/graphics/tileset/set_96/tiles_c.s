	.section .rodata
	.global Tileset_Set96TilesC
Tileset_Set96TilesC:
	.incbin "games/tla/src/graphics/tileset/set_96/tiles_c.4bpp.mtf"
	.byte 0x16	@ PACKER: left in the buffer by Tileset_Set96TilesB+0xc87
