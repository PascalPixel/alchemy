	.section .rodata
	.global Tileset_Set62TilesA
Tileset_Set62TilesA:
	.incbin "games/tla/src/graphics/tileset/set_62/tiles_a.4bpp.mtf"
	.byte 0x06, 0x0d, 0x54	@ PACKER: left in the buffer by Field_Map129+0x1505
