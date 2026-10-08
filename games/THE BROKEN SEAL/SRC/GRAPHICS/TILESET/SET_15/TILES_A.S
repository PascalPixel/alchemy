	.section .rodata
	.global Tileset_Set15TilesA
Tileset_Set15TilesA:
	.incbin "GRAPHICS/TILESET/SET_15/TILES_A.4bpp.mtf"
	/* PACKER: left in the packer's buffer by MAP_018/MAP.o at +0x1c29 */
	.byte 0x80, 0x36, 0x05
