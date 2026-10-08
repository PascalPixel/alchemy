	.section .rodata
	.global Tileset_Set62TilesC
Tileset_Set62TilesC:
	.incbin "GRAPHICS/TILESET/SET_62/TILES_C.4bpp.mtf"
	.byte 0x3c, 0xe0, 0x13	@ PACKER: left in the buffer by Tileset_Set62TilesB+0x1445
