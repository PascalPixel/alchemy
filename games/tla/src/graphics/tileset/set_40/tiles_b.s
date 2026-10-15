	.section .rodata
	.global Tileset_Set40TilesB
Tileset_Set40TilesB:
	.incbin "games/tla/src/graphics/tileset/set_40/tiles_b.4bpp.mtf"
	.byte 0x7f, 0x04	@ PACKER: left in the buffer by Tileset_Set40TilesA+0x15a6
