	.section .rodata
	.global Tileset_Set27Palette
Tileset_Set27Palette:
	.incbin "GRAPHICS/TILESET/SET_27/PALETTE.gbapal.lz"
	/* PACKER: left in the packer's buffer by MAP_035/MAP.o at +0x1a9 */
	.byte 0x8c, 0xda, 0xd6
