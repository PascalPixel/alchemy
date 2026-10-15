	.section .rodata
	.global Tileset_Set47TilesB
Tileset_Set47TilesB:
	.incbin "games/tla/src/graphics/tileset/set_47/tiles_b.4bpp.mtf"
	.byte 0x71, 0x76, 0xf0	@ PACKER: left in the buffer by Tileset_Set47TilesA+0x1969
