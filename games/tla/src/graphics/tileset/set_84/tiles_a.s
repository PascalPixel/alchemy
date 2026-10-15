	.section .rodata
	.global Tileset_Set84TilesA
Tileset_Set84TilesA:
	.incbin "games/tla/src/graphics/tileset/set_84/tiles_a.4bpp.mtf"
	.byte 0x0a, 0x16	@ PACKER: left in the buffer by Field_Map192+0x203e
