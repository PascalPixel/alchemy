	.section .rodata
	.global Tileset_Set03Palette
Tileset_Set03Palette:
	.incbin "GRAPHICS/TILESET/SET_03/PALETTE.gbapal.lz"
	/* PACKER: left in the packer's buffer by MAP_004/MAP.o at +0x191 */
	.byte 0xc1, 0xac, 0xce
