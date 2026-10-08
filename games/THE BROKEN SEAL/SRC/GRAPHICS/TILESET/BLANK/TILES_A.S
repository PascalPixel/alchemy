	.section .rodata
	.global Tileset_BlankTilesA
Tileset_BlankTilesA:
	.incbin "GRAPHICS/TILESET/BLANK/TILES_A.4bpp.mtf"
	/* PACKER: left in the packer's buffer by MAP_001/MAP.o at +0x14e */
	.byte 0xff, 0x00
