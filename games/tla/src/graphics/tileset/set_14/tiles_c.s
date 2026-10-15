	.section .rodata
	.global Tileset_Set14TilesC
Tileset_Set14TilesC:
	.incbin "games/tla/src/graphics/tileset/set_14/tiles_c.4bpp.mtf"
	.byte 0xd8, 0x18	@ PACKER: left in the buffer by Tileset_Set14TilesB+0x1e86
