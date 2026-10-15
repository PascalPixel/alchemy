	.section .rodata
	.global Tileset_Set91TilesB
Tileset_Set91TilesB:
	.incbin "games/tla/src/graphics/tileset/set_91/tiles_b.4bpp.mtf"
	.byte 0xf5, 0x13	@ PACKER: left in the buffer by Tileset_Set91TilesA+0x161e
