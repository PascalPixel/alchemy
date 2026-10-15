	.section .rodata
	.global Tileset_Set87TilesB
Tileset_Set87TilesB:
	.incbin "games/tla/src/graphics/tileset/set_87/tiles_b.4bpp.mtf"
	.byte 0x2b	@ PACKER: left in the buffer by Tileset_Set87TilesA+0x1bcf
