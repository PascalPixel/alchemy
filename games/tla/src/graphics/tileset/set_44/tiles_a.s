	.section .rodata
	.global Tileset_Set44TilesA
Tileset_Set44TilesA:
	.incbin "games/tla/src/graphics/tileset/set_44/tiles_a.4bpp.mtf"
	.byte 0xa0, 0x32, 0x14	@ PACKER: left in the buffer by Tileset_Set43TilesB+0x245d
