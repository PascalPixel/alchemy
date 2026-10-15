	.section .rodata
	.global Tileset_Set74TilesC
Tileset_Set74TilesC:
	.incbin "games/tla/src/graphics/tileset/set_74/tiles_c.4bpp.mtf"
	.byte 0x7a	@ PACKER: left in the buffer by Tileset_Set74TilesB+0xc8f
