	.section .rodata
	.global Tileset_Set120TilesB
Tileset_Set120TilesB:
	.incbin "games/tla/src/graphics/tileset/set_120/tiles_b.4bpp.mtf"
	.byte 0xe0	@ PACKER: left in the buffer by Tileset_Set120TilesA+0x13f
