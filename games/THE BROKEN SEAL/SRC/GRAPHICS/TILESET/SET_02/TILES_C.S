	.section .rodata
	.global Tileset_Set02TilesC
Tileset_Set02TilesC:
	.incbin "GRAPHICS/TILESET/SET_02/TILES_C.4bpp.mtf"
	.byte 0x5c, 0x40	@ PACKER: left in the buffer by Tileset_Set02TilesA+0x26e2
