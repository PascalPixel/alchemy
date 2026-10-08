	.section .rodata
	.global Tileset_Set46TilesC
Tileset_Set46TilesC:
	.incbin "GRAPHICS/TILESET/SET_46/TILES_C.4bpp.mtf"
	.byte 0xe5, 0xa2	@ PACKER: left in the buffer by Tileset_Set46TilesB+0x706
