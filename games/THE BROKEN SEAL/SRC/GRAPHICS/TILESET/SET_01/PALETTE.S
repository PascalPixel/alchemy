	.section .rodata
	.global Tileset_Set01Palette
Tileset_Set01Palette:
	.incbin "GRAPHICS/TILESET/SET_01/PALETTE.gbapal.lz"
	/* PACKER: left in the packer's buffer by MAP_002/MAP.o at +0x193 */
	.byte 0xb3
