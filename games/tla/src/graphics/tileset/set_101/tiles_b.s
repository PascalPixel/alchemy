	.section .rodata
	.global Tileset_Set101TilesB
Tileset_Set101TilesB:
	.incbin "games/tla/src/graphics/tileset/set_101/tiles_b.4bpp.mtf"
	.byte 0x16	@ PACKER: left in the buffer by Tileset_Set101TilesA+0x107f
