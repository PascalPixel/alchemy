	.section .rodata
	.global Tileset_Set59TilesC
Tileset_Set59TilesC:
	.incbin "games/tla/src/graphics/tileset/set_59/tiles_c.4bpp.mtf"
	.byte 0x38, 0xa4	@ PACKER: left in the buffer by Tileset_Set59TilesB+0x104a
