	.section .rodata
	.global Tileset_Set39TilesC
Tileset_Set39TilesC:
	.incbin "games/tla/src/graphics/tileset/set_39/tiles_c.4bpp.mtf"
	.byte 0xe7, 0xe3, 0x0d	@ PACKER: left in the buffer by Tileset_Set39TilesB+0x2681
