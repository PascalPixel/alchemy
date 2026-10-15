	.section .rodata
	.global Tileset_Set87TilesD
Tileset_Set87TilesD:
	.incbin "games/tla/src/graphics/tileset/set_87/tiles_d.4bpp.mtf"
	.byte 0x38, 0x5a, 0x61	@ PACKER: left in the buffer by Tileset_Set87TilesC+0xb29
