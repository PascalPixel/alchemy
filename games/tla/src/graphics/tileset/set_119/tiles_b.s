	.section .rodata
	.global Tileset_Set119TilesB
Tileset_Set119TilesB:
	.incbin "games/tla/src/graphics/tileset/set_119/tiles_b.4bpp.mtf"
	.byte 0x0f, 0xd8	@ PACKER: left in the buffer by Tileset_Set119TilesA+0x1772
