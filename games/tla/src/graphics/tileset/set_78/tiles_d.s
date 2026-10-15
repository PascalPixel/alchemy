	.section .rodata
	.global Tileset_Set78TilesD
Tileset_Set78TilesD:
	.incbin "games/tla/src/graphics/tileset/set_78/tiles_d.4bpp.mtf"
	.byte 0xb9, 0x40, 0x00	@ PACKER: left in the buffer by Tileset_Set78TilesB+0x639
