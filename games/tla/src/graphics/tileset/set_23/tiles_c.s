	.section .rodata
	.global Tileset_Set23TilesC
Tileset_Set23TilesC:
	.incbin "games/tla/src/graphics/tileset/set_23/tiles_c.4bpp.mtf"
	.byte 0x69, 0xd7, 0x36	@ PACKER: left in the buffer by Tileset_Set23TilesB+0x1ad9
