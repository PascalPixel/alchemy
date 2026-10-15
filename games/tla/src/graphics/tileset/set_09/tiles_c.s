	.section .rodata
	.global Tileset_Set09TilesC
Tileset_Set09TilesC:
	.incbin "games/tla/src/graphics/tileset/set_09/tiles_c.4bpp.mtf"
	.byte 0xd0, 0x88, 0xf9	@ PACKER: left in the buffer by Tileset_Set09TilesA+0xca5
