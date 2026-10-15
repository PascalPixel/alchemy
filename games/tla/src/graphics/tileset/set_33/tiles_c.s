	.section .rodata
	.global Tileset_Set33TilesC
Tileset_Set33TilesC:
	.incbin "games/tla/src/graphics/tileset/set_33/tiles_c.4bpp.mtf"
	.byte 0x6e	@ PACKER: left in the buffer by Tileset_Set33TilesB+0x110b
