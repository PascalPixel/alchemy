	.section .rodata
	.global Tileset_Set07Palette
Tileset_Set07Palette:
	.incbin "GRAPHICS/TILESET/SET_07/PALETTE.gbapal.lz"
	/* PACKER: left in the packer's buffer by MAP_008/MAP.o at +0xe2 */
	.byte 0x13, 0x3e
