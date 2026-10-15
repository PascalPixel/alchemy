	.section .rodata
	.global Tileset_Set93TilesD
Tileset_Set93TilesD:
	.incbin "games/tla/src/graphics/tileset/set_93/tiles_d.4bpp.mtf"
	.byte 0xc2, 0x8d	@ PACKER: left in the buffer by Tileset_Set93TilesB+0x120e
