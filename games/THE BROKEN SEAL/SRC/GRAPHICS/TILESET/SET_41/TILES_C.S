	.section .rodata
	.global Tileset_Set41TilesC
Tileset_Set41TilesC:
	.incbin "GRAPHICS/TILESET/SET_41/TILES_C.4bpp.mtf"
	.byte 0xde	@ PACKER: left in the buffer by Tileset_Set40TilesA+0x2633
