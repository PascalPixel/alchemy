	.section .rodata
	.global Tileset_Map181TilesA
Tileset_Map181TilesA:
	.incbin "games/tla/src/graphics/tileset/map_181/tiles_a.4bpp.mtf"
	.byte 0xa7, 0xe6	@ PACKER: left in the buffer by Tileset_Set81TilesD+0x1f0a
