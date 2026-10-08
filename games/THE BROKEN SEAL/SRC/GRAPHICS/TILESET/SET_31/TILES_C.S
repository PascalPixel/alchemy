	.section .rodata
	.global Tileset_Set31TilesC
Tileset_Set31TilesC:
	.incbin "GRAPHICS/TILESET/SET_31/TILES_C.4bpp.mtf"
	.byte 0xfa, 0x80	@ PACKER: left in the buffer by Tileset_Set31TilesB+0x1042
