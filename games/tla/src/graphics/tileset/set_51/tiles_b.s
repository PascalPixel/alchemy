	.section .rodata
	.global Tileset_Set51TilesB
Tileset_Set51TilesB:
	.incbin "games/tla/src/graphics/tileset/set_51/tiles_b.4bpp.mtf"
	.byte 0x8e, 0x16, 0x18	@ PACKER: left in the buffer by Tileset_Set51TilesA+0x2221
