	.section .rodata
	.global Tileset_Set21TilesC
Tileset_Set21TilesC:
	.incbin "games/tla/src/graphics/tileset/set_21/tiles_c.4bpp.mtf"
	.byte 0xd4	@ PACKER: left in the buffer by Tileset_Set21TilesB+0x2df
