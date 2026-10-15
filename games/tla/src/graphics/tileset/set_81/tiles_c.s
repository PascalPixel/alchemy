	.section .rodata
	.global Tileset_Set81TilesC
Tileset_Set81TilesC:
	.incbin "games/tla/src/graphics/tileset/set_81/tiles_c.4bpp.mtf"
	.byte 0xd7, 0x23	@ PACKER: left in the buffer by Tileset_Set81TilesB+0x15c2
