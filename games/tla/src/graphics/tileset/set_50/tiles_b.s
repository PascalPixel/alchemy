	.section .rodata
	.global Tileset_Set50TilesB
Tileset_Set50TilesB:
	.incbin "games/tla/src/graphics/tileset/set_50/tiles_b.4bpp.mtf"
	.byte 0x1d, 0x94, 0x7f	@ PACKER: left in the buffer by Tileset_Set50TilesA+0x148d
