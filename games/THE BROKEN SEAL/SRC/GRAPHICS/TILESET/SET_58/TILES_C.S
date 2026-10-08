	.section .rodata
	.global Tileset_Set58TilesC
Tileset_Set58TilesC:
	.incbin "GRAPHICS/TILESET/SET_58/TILES_C.4bpp.mtf"
	.byte 0xfe	@ PACKER: left in the buffer by Tileset_Set58TilesB+0x153
