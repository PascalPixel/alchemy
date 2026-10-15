	.section .rodata
	.global Tileset_Set110TilesB
Tileset_Set110TilesB:
	.incbin "games/tla/src/graphics/tileset/set_110/tiles_b.4bpp.mtf"
	.byte 0xc1, 0xc3	@ PACKER: left in the buffer by Tileset_Set110TilesA+0x6e6
