	.section .rodata
	.global Tileset_BlankTilesC
Tileset_BlankTilesC:
	.incbin "GRAPHICS/TILESET/BLANK/TILES_B.4bpp.mtf"
	/* PACKER: left in the packer's buffer by TILESET/BLANK/TILES_A.o at +0x13f */
	.byte 0x07
