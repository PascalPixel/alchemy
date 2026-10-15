	.section .rodata
	.global Tileset_Set24TilesA
Tileset_Set24TilesA:
	.incbin "games/tla/src/graphics/tileset/set_24/tiles_a.4bpp.mtf"
	.byte 0x08, 0x0a	@ PACKER: left in the buffer by Field_Map041+0x14be
