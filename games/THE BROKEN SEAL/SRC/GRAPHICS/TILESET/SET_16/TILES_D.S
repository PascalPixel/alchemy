	.section .rodata
	.global Tileset_Set16TilesD
Tileset_Set16TilesD:
	.incbin "GRAPHICS/TILESET/SET_16/TILES_D.4bpp.mtf"
	/* PACKER: left in the packer's buffer by TILESET/SET_16/TILES_A.o at +0xda2 */
	.byte 0x82, 0xb0
