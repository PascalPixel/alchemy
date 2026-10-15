	.section .rodata
	.global Tileset_Set09TilesB
Tileset_Set09TilesB:
	.incbin "games/tla/src/graphics/tileset/set_09/tiles_b.4bpp.mtf"
	.byte 0xdc	@ PACKER: left in the buffer by Tileset_Set09TilesA+0x13f
