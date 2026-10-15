	.section .rodata
	.global Tileset_Set79TilesB
Tileset_Set79TilesB:
	.incbin "games/tla/src/graphics/tileset/set_79/tiles_b.4bpp.mtf"
	.byte 0xc7, 0x69, 0x1f	@ PACKER: left in the buffer by Tileset_Set79TilesA+0x21ad
