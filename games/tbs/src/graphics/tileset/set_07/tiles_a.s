	.section .rodata
	.global Tileset_Set07TilesA
Tileset_Set07TilesA:
	.incbin "common/graphics/tileset/set_07/tiles_a.4bpp.mtf"
	.byte 0xf0, 0xa4, 0x22	@ PACKER: left in the buffer by Field_Map008+0x11e9
