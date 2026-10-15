	.section .rodata
	.global Tileset_Set22TilesC
Tileset_Set22TilesC:
	.incbin "games/tla/src/graphics/tileset/set_22/tiles_c.4bpp.mtf"
	.byte 0x41, 0xf2	@ PACKER: left in the buffer by Tileset_Set22TilesB+0xf9e
