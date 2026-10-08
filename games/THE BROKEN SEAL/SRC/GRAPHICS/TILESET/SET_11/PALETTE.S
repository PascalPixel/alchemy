	.section .rodata
	.global Tileset_Set11Palette
Tileset_Set11Palette:
	.incbin "GRAPHICS/TILESET/SET_11/PALETTE.gbapal.lz"
	/* PACKER: left in the packer's buffer by MAP_013/MAP.o at +0x113 */
	.byte 0x43
