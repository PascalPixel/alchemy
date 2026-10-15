	.section .rodata
	.global Tileset_Set88TilesB
Tileset_Set88TilesB:
	.incbin "games/tla/src/graphics/tileset/set_88/tiles_b.4bpp.mtf"
	.byte 0x9d, 0x69	@ PACKER: left in the buffer by Tileset_Set88TilesA+0x1e02
