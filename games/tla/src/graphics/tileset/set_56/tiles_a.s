	.section .rodata
	.global Tileset_Set56TilesA
Tileset_Set56TilesA:
	.incbin "games/tla/src/graphics/tileset/set_56/tiles_a.4bpp.mtf"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map106+0x1e57
