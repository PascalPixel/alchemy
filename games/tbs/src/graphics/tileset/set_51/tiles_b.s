	.section .rodata
	.global Tileset_Set51TilesB
Tileset_Set51TilesB:
	.incbin "graphics/tileset/set_51/tiles_b.4bpp.mtf"
	.byte 0xd1, 0xbc	@ PACKER: left in the buffer by Field_Map092+0x2776
