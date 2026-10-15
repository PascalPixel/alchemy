	.section .rodata
	.global Tileset_Set65TilesA
Tileset_Set65TilesA:
	.incbin "games/tla/src/graphics/tileset/set_65/tiles_a.4bpp.mtf"
	.byte 0x4c, 0x4f, 0x3a	@ PACKER: left in the buffer by Field_Map134+0x2091
