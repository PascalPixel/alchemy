	.section .rodata
	.global Tileset_Set16TilesC
Tileset_Set16TilesC:
	.incbin "games/tla/src/graphics/tileset/set_16/tiles_c.4bpp.mtf"
	.byte 0x01	@ PACKER: left in the buffer by Field_Map033+0x217f
