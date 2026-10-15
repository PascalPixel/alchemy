	.section .rodata
	.global Tileset_Set45TilesA
Tileset_Set45TilesA:
	.incbin "games/tla/src/graphics/tileset/set_45/tiles_a.4bpp.mtf"
	.byte 0xdd, 0x56	@ PACKER: left in the buffer by Field_Map075+0x2c8a
