	.section .rodata
	.global Tileset_Set52TilesC
Tileset_Set52TilesC:
	.incbin "games/tla/src/graphics/tileset/set_52/tiles_c.4bpp.mtf"
	.byte 0xad	@ PACKER: left in the buffer by Tileset_Set52TilesB+0x101f
