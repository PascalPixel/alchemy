	.section .rodata
	.global Tileset_Set22TilesC
Tileset_Set22TilesC:
	.incbin "GRAPHICS/TILESET/SET_22/TILES_C.4bpp.mtf"
	.byte 0xf0	@ PACKER: left in the buffer by Tileset_Set22TilesB+0x13f
