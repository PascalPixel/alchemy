	.section .rodata
	.global Tileset_Set08Palette
Tileset_Set08Palette:
	.incbin "GRAPHICS/TILESET/SET_08/PALETTE.gbapal.lz"
	/* PACKER: left in the packer's buffer by MAP_009/MAP.o at +0x159 */
	.byte 0xcf, 0x83, 0x07
