	.section .rodata
	.global Tileset_Set09TilesD
Tileset_Set09TilesD:
	.incbin "games/tla/src/graphics/tileset/set_09/tiles_d.4bpp.mtf"
	.byte 0x72, 0x70, 0xf9	@ PACKER: left in the buffer by Tileset_Set09TilesA+0x15cd
