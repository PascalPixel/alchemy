	.section .rodata
	.global Tileset_Set04TilesB
Tileset_Set04TilesB:
	.incbin "games/tla/src/graphics/tileset/set_04/tiles_b.4bpp.mtf"
	.byte 0x00, 0xa0, 0xbb	@ PACKER: left in the buffer by Tileset_Set04TilesA+0x20dd
