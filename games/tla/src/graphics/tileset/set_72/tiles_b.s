	.section .rodata
	.global Tileset_Set72TilesB
Tileset_Set72TilesB:
	.incbin "games/tla/src/graphics/tileset/set_72/tiles_b.4bpp.mtf"
	.byte 0x3a	@ PACKER: left in the buffer by Tileset_Set72TilesA+0x1be3
