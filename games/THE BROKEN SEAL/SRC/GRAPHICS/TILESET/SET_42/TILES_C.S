	.section .rodata
	.global Tileset_Set42TilesC
Tileset_Set42TilesC:
	.incbin "GRAPHICS/TILESET/SET_42/TILES_C.4bpp.mtf"
	.byte 0xc3, 0x73, 0x89	@ PACKER: left in the buffer by Tileset_Set42TilesB+0x1c59
