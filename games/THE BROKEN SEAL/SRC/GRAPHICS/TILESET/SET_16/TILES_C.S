	.section .rodata
	.global Tileset_Set16TilesC
Tileset_Set16TilesC:
	.incbin "GRAPHICS/TILESET/SET_16/TILES_C.4bpp.mtf"
	/* PACKER: left in the packer's buffer by TILESET/SET_16/TILES_B.o at +0x186 */
	.byte 0x1d, 0x65
