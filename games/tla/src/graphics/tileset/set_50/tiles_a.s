	.section .rodata
	.global Tileset_Set50TilesA
Tileset_Set50TilesA:
	.incbin "games/tla/src/graphics/tileset/set_50/tiles_a.4bpp.mtf"
	.byte 0xf6, 0x09, 0x04	@ PACKER: left in the buffer by Field_Map100+0x1985
