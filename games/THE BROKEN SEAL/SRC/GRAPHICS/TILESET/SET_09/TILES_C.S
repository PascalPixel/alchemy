	.section .rodata
	.global Tileset_Set09TilesC
Tileset_Set09TilesC:
	.incbin "GRAPHICS/TILESET/SET_09/TILES_C.4bpp.mtf"
	.byte 0x1c, 0x40	@ PACKER: left in the buffer by Tileset_Set09TilesB+0xade
