	.section .rodata
	.global Tileset_Set02TilesA
Tileset_Set02TilesA:
	.incbin "GRAPHICS/TILESET/SET_02/TILES_A.4bpp.mtf"
	/* PACKER: left in the packer's buffer by MAP_003/MAP.o at +0x27b9 */
	.byte 0xfc, 0x89, 0x00
