	.section .rodata
	.global Tileset_Set42TilesA
Tileset_Set42TilesA:
	.incbin "games/tla/src/graphics/tileset/set_42/tiles_a.4bpp.mtf"
	.byte 0x70, 0x0e	@ PACKER: left in the buffer by Field_Map089+0x15e6
