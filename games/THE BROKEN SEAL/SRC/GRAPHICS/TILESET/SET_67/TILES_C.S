	.section .rodata
	.global Tileset_Set67TilesC
Tileset_Set67TilesC:
	.incbin "GRAPHICS/TILESET/SET_67/TILES_C.4bpp.mtf"
	.byte 0x0f, 0x7c, 0xfe	@ PACKER: left in the buffer by Tileset_Set67TilesB+0x2a5
