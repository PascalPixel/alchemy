	.section .rodata
	.global Tileset_Set24TilesB
Tileset_Set24TilesB:
	.incbin "games/tla/src/graphics/tileset/set_24/tiles_b.4bpp.mtf"
	.byte 0x3c, 0x00	@ PACKER: left in the buffer by Field_Map041+0x1802
