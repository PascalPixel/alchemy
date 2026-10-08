	.section .rodata
	.global Tileset_Set07TilesC
Tileset_Set07TilesC:
	.incbin "COMMON/GRAPHICS/TILESET/SET_07/TILES_C.4bpp.mtf"
	.byte 0x3e	@ PACKER: left in the buffer by Tileset_Set07TilesB+0x50b
