	.section .rodata
	.global Tileset_Set82TilesA
Tileset_Set82TilesA:
	.incbin "games/tla/src/graphics/tileset/set_82/tiles_a.4bpp.mtf"
	.byte 0xfa, 0x10	@ PACKER: left in the buffer by Field_Map189+0x2c1a
