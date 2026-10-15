	.section .rodata
	.global Tileset_Set80TilesB
Tileset_Set80TilesB:
	.incbin "games/tla/src/graphics/tileset/set_80/tiles_b.4bpp.mtf"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map179+0x1ed7
