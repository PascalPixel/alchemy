	.section .rodata
	.global Tileset_Set61TilesD
Tileset_Set61TilesD:
	.incbin "games/tla/src/graphics/tileset/set_61/tiles_d.4bpp.mtf"
	.byte 0xf3	@ PACKER: left in the buffer by Tileset_Set61TilesB+0x27d3
