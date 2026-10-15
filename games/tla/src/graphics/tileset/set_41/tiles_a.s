	.section .rodata
	.global Tileset_Set41TilesA
Tileset_Set41TilesA:
	.incbin "games/tla/src/graphics/tileset/set_41/tiles_a.4bpp.mtf"
	.byte 0x80, 0x00	@ PACKER: left in the buffer by Field_Map086+0xd3e
