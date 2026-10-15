	.section .rodata
	.global Tileset_Set69TilesA
Tileset_Set69TilesA:
	.incbin "games/tla/src/graphics/tileset/set_69/tiles_a.4bpp.mtf"
	.byte 0x3e, 0xe0, 0xde	@ PACKER: left in the buffer by Field_Map146+0x1989
