	.section .rodata
	.global Tileset_Set40TilesC
Tileset_Set40TilesC:
	.incbin "games/tla/src/graphics/tileset/set_40/tiles_c.4bpp.mtf"
	.byte 0x04, 0x40, 0xd8	@ PACKER: left in the buffer by Field_Map078+0x1fa9
