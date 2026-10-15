	.section .rodata
	.global Tileset_Set25TilesB
Tileset_Set25TilesB:
	.incbin "games/tla/src/graphics/tileset/set_25/tiles_b.4bpp.mtf"
	.byte 0x22, 0x0e, 0xf8	@ PACKER: left in the buffer by Tileset_Set25TilesA+0x1bd5
