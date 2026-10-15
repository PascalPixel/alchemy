	.section .rodata
	.global Tileset_Set02TilesB
Tileset_Set02TilesB:
	.incbin "games/tla/src/graphics/tileset/set_02/tiles_b.4bpp.mtf"
	.byte 0xb0	@ PACKER: left in the buffer by Tileset_Set02TilesA+0x27c7
